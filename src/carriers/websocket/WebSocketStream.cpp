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
    yCInfo(WEBSOCK_STREAM) << "close";
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
            getFrame(buffer);
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
void WebSocketStream::write(const Bytes& b)
{
    yCTrace(WEBSOCK_STREAM) << b.length();
    yCInfo(WEBSOCK_STREAM) << b.length();
    yCInfo(WEBSOCK_STREAM) << b.get();
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
WebSocketFrameType WebSocketStream::getFrame(yarp::os::ManagedBytes& payload)
{
    yCInfo(WEBSOCK_STREAM) << "getframe";
    yarp::os::ManagedBytes header;
    yarp::os::ManagedBytes mask_bytes;
    header.allocate(2);
    delegate->getInputStream().read(header.bytes());
    unsigned char msg_opcode = header.get()[0] & 0x0F;
    unsigned char msg_fin = (header.get()[0] >> 7) & 0x01;
    unsigned char msg_masked = (header.get()[1] >> 7) & 0x01;
    if (msg_opcode == CLOSING_OPCODE)
    {
        // TODO FIXME STE NEED TO CLOSE THE CONNECTIOn
        unsigned char toreturn[]  = "\0\0\0\0~\0\0\1q";
        payload.allocate(10);
        memcpy(payload.get(), toreturn, 10);
        return CLOSING_OPCODE;
    }

    // *** message decoding
    /*     if(msg_opcode == 0x0) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME; // continuation frame ?
     if(msg_opcode == 0x1) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME;
     if(msg_opcode == 0x2) return (msg_fin)?BINARY_FRAME:INCOMPLETE_BINARY_FRAME;
     if(msg_opcode == 0x9) return PING_FRAME;
     if(msg_opcode == 0xA) return PONG_FRAME;*/


    yarp::os::NetInt32 payload_length = 0;
    int pos = 2;
    int length_field = header.get()[1] & (0x7F);
    unsigned int mask = 0;

    if (length_field <= 125) {
        payload_length = length_field;
    } else if (length_field == 126) { //msglen is 16bit!
        yarp::os::ManagedBytes additionalLength;
        additionalLength.allocate(2);
        delegate->getInputStream().read(additionalLength.bytes());
        // TODO FIXME STE this is not so good, maybe bytes can be used (the left shift is notso good)
        payload_length = ((static_cast<unsigned char>(additionalLength.get()[0]) << 8) |
                          (static_cast<unsigned char>(additionalLength.get()[1])));
        pos += 2;
    } else if (length_field == 127) { //msglen is 64bit!
        yarp::os::ManagedBytes additionalLength;
        additionalLength.allocate(8);
        delegate->getInputStream().read(additionalLength.bytes());

        // TODO FIXME STE this part need to be cheked (payload length is 32 bits)
        payload_length = ((static_cast<unsigned char>(additionalLength.get()[0]) << 56) |
                          (static_cast<unsigned char>(additionalLength.get()[1]) << 48) |
                          (static_cast<unsigned char>(additionalLength.get()[2]) << 40) |
                          (static_cast<unsigned char>(additionalLength.get()[3]) << 32) |
                          (static_cast<unsigned char>(additionalLength.get()[4]) << 24) |
                          (static_cast<unsigned char>(additionalLength.get()[5]) << 16) |
                          (static_cast<unsigned char>(additionalLength.get()[6]) << 8) |
                          (static_cast<unsigned char>(additionalLength.get()[7])));
        pos += 8;
    }

    if (msg_masked) {
        // get the mask
        mask_bytes.allocate(4);
        delegate->getInputStream().read(mask_bytes.bytes());
        //printf("MASK: %08x\n", mask);
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
    return BINARY_FRAME;
}


// FIXME TODO STE need to add length higher than 2^32
void WebSocketStream::makeFrame(WebSocketFrameType frame_type,
                                const yarp::os::Bytes& payload,
                                yarp::os::ManagedBytes& frame)
{
    int pos = 0;
    int size = payload.length();


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
        std::bitset<8> x(frame.get()[pos - 1]);
        //std::bitset<8> y(additionalLength.get()[1]);
        std::cout << "frame size" << x << std::endl;
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
