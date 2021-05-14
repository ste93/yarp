 /*
  * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
  * All rights reserved.
  *
  * This software may be modified and distributed under the terms of the
  * BSD-3-Clause license. See the accompanying LICENSE file for details.
  */
  
 #ifndef YARP_WEBSOCKSTREAM_H
 #define YARP_WEBSOCKSTREAM_H

#include <yarp/os/ManagedBytes.h>
#include <yarp/os/TwoWayStream.h>

#include "WebSocket/WebSocket.h"

 class WebSocketStream :
         public yarp::os::TwoWayStream,
         public yarp::os::InputStream,
         public yarp::os::OutputStream
 {
  
 public:
     WebSocketStream(TwoWayStream *delegate);
  
     ~WebSocketStream() override;
  
     InputStream& getInputStream() override;
  
     OutputStream& getOutputStream() override;
  
     const yarp::os::Contact& getLocalAddress() const override;
  
     const yarp::os::Contact& getRemoteAddress() const override;
  
     void interrupt() override;
  
     void close() override;
  
     using yarp::os::InputStream::read;
     yarp::conf::ssize_t read(yarp::os::Bytes& b) override;
  
     using yarp::os::OutputStream::write;
     void write(const yarp::os::Bytes& b) override;
  
     bool isOk() const override;
  
     void reset() override;
  
     void beginPacket() override;
     void endPacket() override;
  
    // bool open(bool sender = false);
//     void setLocalAddress(yarp::os::Contact& _localAddress);
    // void setRemoteAddress(yarp::os::Contact& _remoteAddress);
  
 private:
    yarp::os::TwoWayStream *delegate;
    yarp::os::Contact local, remote;
    yarp::os::ManagedBytes buffer;
    size_t currentHead{0};
    WebSocketFrameType getFrame(yarp::os::ManagedBytes &payload);
 };
  
 #endif // YARP_WEBSOCKSTREAM_H