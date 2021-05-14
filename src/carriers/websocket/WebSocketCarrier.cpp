/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include "WebSocketCarrier.h"
#include "WebSocketStream.h"
#include <yarp/os/Bottle.h>
#include <yarp/os/Route.h>
#include <yarp/os/OutputStream.h>
#include <yarp/os/ManagedBytes.h>
#include <iostream>

#include <regex>

using namespace yarp::os;

YARP_LOG_COMPONENT(WEBSOCKETCARRIER,
                   "yarp.carrier.websocket",
                   yarp::os::Log::TraceType,
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)


WebSocketCarrier::WebSocketCarrier()
{
}

Carrier* WebSocketCarrier::create() const
{
    yCTrace(WEBSOCKETCARRIER);
    return new WebSocketCarrier();
}

std::string WebSocketCarrier::getName() const
{
    yCTrace(WEBSOCKETCARRIER);
    return "websocket";
}

bool WebSocketCarrier::checkHeader(const yarp::os::Bytes& header)
{
    yCTrace(WEBSOCKETCARRIER);
    if (header.length() != header_lenght) {
        return false;
    }
    const char *target = "GET /?ws";
    for (size_t i = 0; i < header_lenght; i++) {
        if (header.get()[i] != target[i]) {
            return false;
        }
    }
    yCTrace(WEBSOCKETCARRIER, "Got header");
    return true;
}

void WebSocketCarrier::getHeader(Bytes& header) const
{
    yCTrace(WEBSOCKETCARRIER);
    const char *target = "GET /?ws";
    if (header.length() == 8) {
        for (int i = 0; i < 8; i++) {
            header.get()[i] = target[i];
        }
    }
}

bool WebSocketCarrier::requireAck() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}

bool WebSocketCarrier::isTextMode() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false; //when use connectionReader.readFromStream is always false
}

bool WebSocketCarrier::supportReply() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}

bool WebSocketCarrier::sendHeader(ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
//     Name n(proto.getRoute().getCarrierName() + "://test");
//     std::string pathValue = n.getCarrierModifier("path");
//     std::string target = "GET /?ws\n\n";
//     if (pathValue!="") {
//         target = "GET /";
//         target += pathValue;
//     }
//     target += " HTTP/1.1\n";
//     Contact host = proto.getRoute().getToContact();
//     if (host.getHost()!="") {
//         target += "Host: ";
//         target += host.getHost();
//         target += "\r\n";
//     }
//     target += "\n";
//     Bytes b((char*)target.c_str(),target.length());
//     proto.os().write(b);
    return true;
}

bool WebSocketCarrier::expectReplyToHeader(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::expectSenderSpecifier(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);

    bool urlDone = false;
    std::string url;
//     bool expectPost;
//     int contentLength;
    std::string result = "";

    Route route = proto.getRoute();
    route.setFromName("web");
    proto.setRoute(route);
    std::string remainder = proto.is().readLine();
    result += remainder;
    result += "\r\n";

    yCInfo(WEBSOCKETCARRIER) << remainder;
    if (!urlDone) {
        for (char i : remainder) {
            if (i != ' ') {
                url += i;
            } else {
                break;
            }
        }
    }

    bool done = false;
    while (!done) {
        std::string line = proto.is().readLine();
        result += line;
        result += "\r\n";
        if (line.empty()) {
            done = true;
        } 

    }
    yCInfo(WEBSOCKETCARRIER)  << result.size() << "all http message " << result.c_str() ;
    auto messagetype = messageHandler.parseHandshake(reinterpret_cast<unsigned char*>(const_cast<char*>(result.c_str())), result.size());
    std::stringstream sstream;
    sstream << std::hex << messagetype;
    yCInfo(WEBSOCKETCARRIER)  << "message type " << sstream.str() ;
    return true;
}

bool WebSocketCarrier::sendIndex(yarp::os::ConnectionState& proto, yarp::os::SizedWriter& writer) {
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::expectIndex(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::sendAck(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::expectAck(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::respondToHeader(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);

    auto& ciccio = proto.os();

    std::string reply = messageHandler.answerHandshake();

// "Sec-WebSocket-Version: <VER>\r\n"
// "Sec-WebSocket-Extensions: <EXT>\r\n"
    reply = std::regex_replace(reply, std::regex("<KEY>"), "acceptKey");
//     reply = std::regex_replace(reply, std::regex("<VER>"), std::to_string(webSocketVersion));
//     reply = std::regex_replace(reply, std::regex("<EXT>"), webSocketExtension);
    yCDebug(WEBSOCKETCARRIER) << "REPLY:" << reply;
    ciccio.writeLine(reply.c_str(), reply.length());
    ciccio.flush();
        // SWITCH TO NEW STREAM TYPE
    WebSocketStream *stream = new WebSocketStream(proto.giveStreams());
    if (stream==NULL) { return false; }
    proto.takeStreams(stream);
    return proto.os().isOk();;
//     std::string result = proto.is().readLine();
//     yCDebug(WEBSOCKETCARRIER) << result.size() << result;
//     std::string x {"\r\n"};
//     ciccio.writeLine(x.c_str(), x.length());

}

bool WebSocketCarrier::write(ConnectionState& proto, yarp::os::SizedWriter& writer)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}


bool WebSocketCarrier::canOffer() const
{
    return true;
}
