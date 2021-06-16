/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include "WebSocketStream.h"

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/NetType.h>
#include <yarp/os/NetInt64.h>

#include <bitset>
#include <iostream>
#include <sys/un.h>


using namespace yarp::os;


YARP_LOG_COMPONENT(WEBSOCK_STREAM,
                   "yarp.stream.websocket",
                   yarp::os::Log::TraceType,
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)


WebSocketStream::WebSocketStream(yarp::os::TwoWayStream* delegate) :
        delegate(delegate)
{
}


WebSocketStream::~WebSocketStream()
{
    yCTrace(WEBSOCK_STREAM);
    close();
}


InputStream& WebSocketStream::getInputStream()
{
    yCTrace(WEBSOCK_STREAM);
    return *this;
}


OutputStream& WebSocketStream::getOutputStream()
{
    yCTrace(WEBSOCK_STREAM);
    return *this;
}


const Contact& WebSocketStream::getLocalAddress() const
{
    yCTrace(WEBSOCK_STREAM);
    return local;
}


const Contact& WebSocketStream::getRemoteAddress() const
{
    yCTrace(WEBSOCK_STREAM);
    return remote;
}


void WebSocketStream::interrupt()
{
    yCTrace(WEBSOCK_STREAM);
    close();
}


void WebSocketStream::close()
{
    yCTrace(WEBSOCK_STREAM);
    yarp::os::ManagedBytes frame;
    yarp::os::Bytes b;
    makeFrame(CLOSING_OPCODE, b, frame);
    yarp::os::Bytes toWrite(frame.get(), frame.length());
    return delegate->getOutputStream().write(toWrite);
}


yarp::conf::ssize_t WebSocketStream::read(Bytes& b)
{
    yCTrace(WEBSOCK_STREAM);
    size_t bytesRead = 0;
    while (bytesRead < b.length()) {
        // the buffer is empty
        if (buffer.length() == 0 || buffer.length() == currentHead) {
            WebSocketFrameType frameType;
            do {
                frameType = getFrame(buffer);
            } while (frameType != BINARY_FRAME || frameType != TEXT_FRAME);
            currentHead = 0;
        }
        // get the remaining bytes to read from the buffer
        size_t remainedFromBuffer = buffer.length() - currentHead;
        // if the buffer is enough then the size of the bytes is given,
        // otherwise the remaining bytes from the buffer is copied and is read again from the buffer
        size_t toAdd = (remainedFromBuffer >= b.length()) ? b.length() : remainedFromBuffer;
        memcpy(b.get(), buffer.get() + currentHead, toAdd);
        currentHead += toAdd;
        bytesRead += toAdd;
    }
    return b.length();
}

// TODO FIXME STE need to manage oversized messages
// I think is not necessary since size_t is 32 bits
void WebSocketStream::write(const yarp::os::Bytes& b)
{
    yCTrace(WEBSOCK_STREAM) << b.length();
    yarp::os::ManagedBytes frame;
    makeFrame(BINARY_FRAME, b, frame);
    yarp::os::Bytes toWrite(frame.get(), frame.length());
    return delegate->getOutputStream().write(toWrite);
}

bool WebSocketStream::isOk() const
{
    yCTrace(WEBSOCK_STREAM);
    return true;
}

void WebSocketStream::reset()
{
    yCTrace(WEBSOCK_STREAM);
}

void WebSocketStream::beginPacket()
{
    yCTrace(WEBSOCK_STREAM);
}

void WebSocketStream::endPacket()
{
    yCTrace(WEBSOCK_STREAM);
}


// TODO FIXME STE here need to be managed frametype, continuation frame and some other things
// TODO FIXME STE need to check the size of managedbytes (if is higher than 32 bits) 
// since managedbytes.size is 32 bits we need to manage the overload (how?)
WebSocketFrameType WebSocketStream::getFrame(yarp::os::ManagedBytes& payload)
{
    yCInfo(WEBSOCK_STREAM) << "getframe";
    yarp::os::ManagedBytes header;
    yarp::os::ManagedBytes mask_bytes;
    header.allocate(2);
    delegate->getInputStream().read(header.bytes());
    unsigned char msg_opcode = header.get()[0] & 0x0F;
    // unsigned char msg_fin = (header.get()[0] >> 7) & 0x01;
    unsigned char msg_masked = (header.get()[1] >> 7) & 0x01;
    if (msg_opcode == CLOSING_OPCODE)
    {
        // this sends a quit command to the caller through the yarp protocol (so the caller) can quit the connection in a reasonable manner
        unsigned char toreturn[]  = "\0\0\0\0~\0\0\1q";
        payload.allocate(10);
        memcpy(payload.get(), toreturn, 10);
        return CLOSING_OPCODE;
    }
    else if (msg_opcode != BINARY_FRAME || msg_opcode != TEXT_FRAME){
        std::bitset<8> x(msg_opcode);
        std::cout << "msg_opcode" << x << std::endl;
    }
    yarp::os::NetInt64 payload_length = 0;
    int pos = 2;
    yarp::os::NetInt32 length_field = header.get()[1] & (0x7F);

    if (length_field <= 125) {
        payload_length = length_field;
    } else {
        yarp::os::ManagedBytes additionalLength;
        int length_to_add;
        if (length_field == 126) { //msglen is 16bit!
            length_to_add = 2;
        } else if (length_field == 127) { //msglen is 64bit!
            length_to_add = 8;
        }
        additionalLength.allocate(length_to_add);
        delegate->getInputStream().read(additionalLength.bytes());
        unsigned char bits[8];
        // memcpy(bits, &payload_length, 8);
        // for (int i =0; i<8 ; i++){
        //     std::bitset<8> x(bits[i]);
        //     std::cout << "frame size" << x << std::endl;
        // }
        // yCInfo(WEBSOCK_STREAM) << "bytes received";

        // for (int i =0; i<length_to_add ; i++){
        //     std::bitset<8> x(additionalLength.get()[i]);
        //     std::cout << "bytes" << x << std::endl;
        // }
        // yCInfo(WEBSOCK_STREAM) << "copying";
        for (int i =0; i < length_to_add; i++) {
            memcpy((unsigned char *)(&payload_length) + i,(unsigned char *) &additionalLength.get()[(length_to_add-1) - i], 1);
        }
        pos += length_to_add;
        yCInfo(WEBSOCK_STREAM) << "payload length" << payload_length;
        // memcpy(bits, &payload_length, 8);
        // for (int i =0; i<8 ; i++){
        //     std::bitset<8> x(bits[i]);
        //     std::cout << "payload length" << x << std::endl;
        // }
    }

    if (msg_masked) {
        // get the mask
        mask_bytes.allocate(4);
        delegate->getInputStream().read(mask_bytes.bytes());
        pos += 4;
    }
    payload.allocate(payload_length);
    delegate->getInputStream().read(payload.bytes());

    if (msg_masked) {
        // unmask data:
        for (int i = 0; i < payload_length; i++) {
            payload.get()[i] = payload.get()[i] ^ mask_bytes.get()[i % 4];
        }
    }
    // if(msg_opcode == 0x0) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME; 
    // if(msg_opcode == 0x1) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME;
    // if(msg_opcode == 0x2) return (msg_fin)?BINARY_FRAME:INCOMPLETE_BINARY_FRAME;
    if(msg_opcode == 0x9) return PING_FRAME;
    if(msg_opcode == 0xA) return PONG_FRAME;
    if(msg_opcode == 0x0 || msg_opcode == 0x1) return TEXT_FRAME;
    if(msg_opcode == 0x2) return BINARY_FRAME;
    return ERROR_FRAME;
}


// FIXME TODO STE need to add length higher than 2^32 ?? 
// maybe it's not useful because payload.length is a size_t (32 bits)
void WebSocketStream::makeFrame(WebSocketFrameType frame_type,
                                const yarp::os::Bytes& payload,
                                yarp::os::ManagedBytes& frame)
{
    int pos = 0;
    size_t size = payload.length();


    if (size <= 125) {
        frame.allocate(2 + size);
    } else if (size <= 65535) {
        frame.allocate(4 + size);
    } else { // >2^16-1 (65535)
        frame.allocate(10 + size);
    }

    frame.get()[pos] = (unsigned char)frame_type; // text frame
    pos++;

    if (size <= 125) {
        frame.get()[pos++] = size;
    } else if (size <= 65535) {
        frame.get()[pos++] = 126;                //16 bit length follows
        frame.get()[pos++] = (size >> 8) & 0xFF; // leftmost first
        frame.get()[pos++] = size & 0xFF;
    } else {                      // >2^16-1 (65535)
        frame.get()[pos++] = 127; //64 bit length follows

        /* write 8 bytes length (significant first)
         * since msg_length is int it can be no longer than 4 bytes = 2^32-1
         * padd zeroes for the first 4 bytes
         */
        for (int i = 3; i >= 0; i--) {
            frame.get()[pos++] = 0;
        }
        // write the actual 32bit msg_length in the next 4 bytes
        for (int i = 3; i >= 0; i--) {
            frame.get()[pos++] = ((size >> 8 * i) & 0xFF);
        }
    }
    memcpy((void*)(frame.get() + pos), payload.get(), size);
}
