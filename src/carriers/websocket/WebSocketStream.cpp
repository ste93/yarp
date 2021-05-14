 /*
  * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
  * All rights reserved.
  *
  * This software may be modified and distributed under the terms of the
  * BSD-3-Clause license. See the accompanying LICENSE file for details.
  */

#include <yarp/conf/system.h>

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/NetType.h>
#include <yarp/os/SystemClock.h>

#include "WebSocketStream.h"
#include <bitset>
#include <cerrno>
#include <cstring>
#include <fcntl.h> /* For O_* constants */
#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/un.h>
#include <unistd.h>

 using namespace yarp::os;

 YARP_LOG_COMPONENT(WEBSOCK_STREAM,
                   "yarp.stream.websocket",
                   yarp::os::Log::TraceType,
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)
  
WebSocketStream::WebSocketStream(yarp::os::TwoWayStream *delegate):
                    delegate(delegate)
{
}

//  bool WebSocketStream::open(bool sender)
//  {
//      openedAsReader = !sender;
//      struct sockaddr_un addr;
//      if ((reader_fd = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
//          yCError(WEBSOCK_STREAM, "%d, %s", errno, strerror(errno));
//          return false;
//      }
  
//      memset(&addr, 0, sizeof(addr));
//      addr.sun_family = AF_UNIX;
  
//      if (socketPath.empty()) {
//          *addr.sun_path = '\0';
//          strncpy(addr.sun_path + 1, socketPath.c_str() + 1, sizeof(addr.sun_path) - 2);
//      } else {
//          strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);
//          if (!sender) {
//              ::unlink(socketPath.c_str());
//          }
//      }
  
//      if (sender) {
//          size_t attempts = 0;
//          // try connection 5 times, waiting that the receiver bind the socket
//          while (attempts < maxAttempts) {
//              int result = ::connect(reader_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
//              if (result == 0) {
//                  break;
//              }
//              yarp::os::SystemClock::delaySystem(delayBetweenAttempts);
//              attempts++;
//          }
  
//          if (attempts >= maxAttempts) {
//              yCError(WEBSOCK_STREAM, "connect() error, I tried %zu times: %d, %s", maxAttempts, errno, strerror(errno));
//              return false;
//          }
//      } else {
//          if (::bind(reader_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
//              yCError(WEBSOCK_STREAM, "bind() error: %d, %s", errno, strerror(errno));
//              return false;
//          }
  
//          // the socket will listen only 1 client
//          if (::listen(reader_fd, 2) == -1) {
//              yCError(WEBSOCK_STREAM, "listen() error: %d, %s", errno, strerror(errno));
//              return false;
//          }
//          struct sockaddr_un remote;
//          uint lenRemote = sizeof(remote);
  
//          if ((sender_fd = ::accept(reader_fd, reinterpret_cast<struct sockaddr*>(&remote), &lenRemote)) == -1) {
//              yCError(WEBSOCK_STREAM, "accept() error: %d, %s", errno, strerror(errno));
//              return false;
//          }
//      }
  
//      return true;
//  }
  
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
    //  yCDebug(WEBSOCK_STREAM, "Interrupting socket");
     close();
 }
  
 void WebSocketStream::close()
 {
     yCTrace(WEBSOCK_STREAM);
     //here I need to implement the closing handshake
 }
  
 yarp::conf::ssize_t WebSocketStream::read(Bytes& b)
 {
     yCTrace(WEBSOCK_STREAM);
     size_t bytesRead = 0;
     //yCInfo(WEBSOCK_STREAM)<< "-------------read bytes length" <<b.length() << "buffer length" << buffer.length() << "current head" << currentHead;
     while(bytesRead < b.length()) {
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
         //yCInfo(WEBSOCK_STREAM)<< "-------------at the end of while bytes read" <<bytesRead << "to add" << toAdd << "current head" << currentHead;
     }
     //yCInfo(WEBSOCK_STREAM) << "returning the bytes";
     return b.length();
 }
  
 void WebSocketStream::write(const Bytes& b)
 {
    yCTrace(WEBSOCK_STREAM) << b.length();
    //return delegate->getOutputStream().write(b);
    //  if (reader_fd < 0) {
    //      close();
    //      return;
    //  }
    //  int writtenMem = ::write(openedAsReader ? sender_fd : reader_fd, b.get(), b.length());
    //  if (writtenMem < 0) {
    //      yCError(WEBSOCK_STREAM, "write() error: %d, %s", errno, strerror(errno));
    //      if (errno != ETIMEDOUT) {
    //          close();
    //      }
    //      return;
    //  }
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


 WebSocketFrameType WebSocketStream::getFrame(yarp::os::ManagedBytes &payload)
 {
     yCTrace(WEBSOCK_STREAM);
     //return delegate->getInputStream().read(b);
     yarp::os::ManagedBytes header;
     yarp::os::ManagedBytes mask_bytes;
     header.allocate(2);
     delegate->getInputStream().read(header.bytes());
     unsigned char msg_opcode = header.get()[0] & 0x0F;
     unsigned char msg_fin = (header.get()[0] >> 7) & 0x01;
     unsigned char msg_masked = (header.get()[1] >> 7) & 0x01;
     //std::bitset<8> y(static_cast<unsigned char>(header.get()[0]));
     //std::bitset<8> x(header.get()[1]);

     //std::cout << " op_code " << y << x << std::endl ;

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

     if(length_field <= 125) {
         payload_length = length_field;
     }
     else if(length_field == 126) { //msglen is 16bit!
         yarp::os::ManagedBytes additionalLength;
         additionalLength.allocate(2);
         delegate->getInputStream().read(additionalLength.bytes());

         //std::bitset<8> x(additionalLength.get()[0]);
         //std::bitset<8> y(additionalLength.get()[1]);
         //std::cout << "additional length" <<  x << y << std::endl ;

         payload_length = (
             (static_cast<unsigned char>(additionalLength.get()[0]) << 8) |
             (static_cast<unsigned char>(additionalLength.get()[1])) );
         pos += 2;
     }
     else if(length_field == 127) { //msglen is 64bit!
         yarp::os::ManagedBytes additionalLength;
         additionalLength.allocate(8);
         delegate->getInputStream().read(additionalLength.bytes());

         payload_length = (
             (static_cast<unsigned char>(additionalLength.get()[0]) << 56) |
             (static_cast<unsigned char>(additionalLength.get()[1]) << 48) |
             (static_cast<unsigned char>(additionalLength.get()[2]) << 40) |
             (static_cast<unsigned char>(additionalLength.get()[3]) << 32) |
             (static_cast<unsigned char>(additionalLength.get()[4]) << 24) |
             (static_cast<unsigned char>(additionalLength.get()[5]) << 16) |
             (static_cast<unsigned char>(additionalLength.get()[6]) << 8) |
             (static_cast<unsigned char>(additionalLength.get()[7]))
         );
         pos += 8;
     }

     //printf("PAYLOAD_LEN: %08x\n", payload_length);


     if(msg_masked)
     {
         // get the mask
         mask_bytes.allocate(4);
         delegate->getInputStream().read(mask_bytes.bytes());
         //printf("MASK: %08x\n", mask);
         pos += 4;
     }
     payload.allocate(payload_length);
     delegate->getInputStream().read(payload.bytes());

     if(msg_masked)
     {
         // unmask data:
         for(int i=0; i<payload_length; i++) {
             payload.get()[i] = payload.get()[i] ^ mask_bytes.get()[i%4];
         }
     }
     //yCInfo(WEBSOCK_STREAM) << "payload length"<< payload.length();
     /*
     for (size_t t = 0; t< payload.length(); t++ )
     {
         yCInfo(WEBSOCK_STREAM) << (int) payload.get()[t];
     }*/
     return BINARY_FRAME;
 }
