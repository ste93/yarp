 /*
  * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
  * All rights reserved.
  *
  * This software may be modified and distributed under the terms of the
  * BSD-3-Clause license. See the accompanying LICENSE file for details.
  */
  
 #include <yarp/conf/system.h>
  
 #include <yarp/os/LogStream.h>
 #include <yarp/os/NetType.h>
 #include <yarp/os/SystemClock.h>
#include <yarp/os/LogComponent.h>

 #include "WebSocketStream.h"
 #include <cerrno>
 #include <cstring>
 #include <fcntl.h> /* For O_* constants */
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
    return delegate->getInputStream().read(b);
    //  if (closed || !happy) {
    //      return -1;
    //  }
    //  int result;
    //  result = ::read(openedAsReader ? sender_fd : reader_fd, b.get(), b.length());
    //  if (closed || result == 0) {
    //      happy = false;
    //      return -1;
    //  }
    //  if (result < 0) {
    //      yCError(WEBSOCK_STREAM, "read() error: %d, %s", errno, strerror(errno));
    //      return -1;
    //  }
    //  return result;
 }
  
 void WebSocketStream::write(const Bytes& b)
 {
     yCTrace(WEBSOCK_STREAM);
    return delegate->getOutputStream().write(b);
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