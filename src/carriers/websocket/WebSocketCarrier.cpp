/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include "WebSocketCarrier.h"

#include <yarp/os/Bottle.h>
#include <yarp/os/Route.h>
#include <yarp/os/OutputStream.h>
#include <yarp/os/ManagedBytes.h>

#include "sha1.hpp"
#include "base64.h"

#include <regex>

using namespace yarp::os;

YARP_LOG_COMPONENT(WEBSOCKETCARRIER,
                   "yarp.carrier.websocket",
                   yarp::os::Log::TraceType,
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)


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

bool WebSocketCarrier::isConnectionless() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}

bool WebSocketCarrier::canAccept() const
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::canOffer() const
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::isTextMode() const
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::canEscape() const
{
    yCTrace(WEBSOCKETCARRIER);
//     return false;
    return true;
}

void WebSocketCarrier::handleEnvelope(const std::string& envelope)
{
    yCTrace(WEBSOCKETCARRIER);
    this->envelope = envelope;
}

bool WebSocketCarrier::requireAck() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}

bool WebSocketCarrier::supportReply() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}

bool WebSocketCarrier::isLocal() const
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}

// this is important - flips expected flow of messages
bool WebSocketCarrier::isPush() const
{
    yCTrace(WEBSOCKETCARRIER);
//     return false;
    return true;
}

std::string WebSocketCarrier::toString() const
{
    yCTrace(WEBSOCKETCARRIER);
    return "websocket_carrier";
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


void WebSocketCarrier::setParameters(const yarp::os::Bytes& header)
{
    yCTrace(WEBSOCKETCARRIER) << header.length();
    // no parameters - no carrier variants
}




bool WebSocketCarrier::expectSenderSpecifier(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);

    bool urlDone = false;
    std::string url;
//     bool expectPost;
//     int contentLength;

    Route route = proto.getRoute();
    route.setFromName("web");
    proto.setRoute(route);
    std::string remainder = proto.is().readLine();
    yCDebug(WEBSOCKETCARRIER) << remainder;
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
//     expectPost = false;
//     contentLength = 0;

    while (!done) {
        std::string result = proto.is().readLine();
        yCDebug(WEBSOCKETCARRIER) << result;
        if (result.empty()) {
            done = true;
        } else {
            Bottle b;
            b.fromString(result);
            if (b.get(0).asString() == "Upgrade:") {
                isWebSocket = (b.get(1).asString() == "websocket");
                yCInfo(WEBSOCKETCARRIER) << "----> Upgrade: ---- " << b.get(1).asString() << (isWebSocket ? "true" : "false");
            }
            if (b.get(0).asString() == "Sec-WebSocket-Key:") {
                std::string key = b.get(1).asString();
                yCInfo(WEBSOCKETCARRIER) << "----> Sec-WebSocket-Key: ---- " << key;

                Chocobo1::SHA1 sha1;
                constexpr char GUID_RFC4122[] {"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"};
                std::string x = key + GUID_RFC4122;
                yCDebug(WEBSOCKETCARRIER) << x;
                sha1.addData(x.c_str(), x.length());
                sha1.finalize();
                auto bytes = sha1.toArray();
                acceptKey = base64_encode(bytes.data(), bytes.size());
                yCDebug(WEBSOCKETCARRIER) << acceptKey;
            }
            if (b.get(0).asString() == "Sec-WebSocket-Extensions:") {
                webSocketExtension = b.get(1).asString();
                yCInfo(WEBSOCKETCARRIER) << "----> Sec-WebSocket-Extensions: ---- " << webSocketExtension;
            }
            if (b.get(0).asString() == "Sec-WebSocket-Version:") {
                webSocketVersion = b.get(1).asInt32();
                yCInfo(WEBSOCKETCARRIER) << "----> Sec-WebSocket-Version: ---- " << webSocketVersion;
            }



//             if (b.get(0).asString() == "Content-Length:") {
//                 //printf("]]] got length %d\n", b.get(1).asInt32());
//                 contentLength = b.get(1).asInt32();
//             }
//             if (b.get(0).asString() == "Content-Type:") {
//                 //printf("]]] got type %s\n", b.get(1).asString());
//                 if (b.get(1).asString() == "application/x-www-form-urlencoded") {
//                     expectPost = true;
//                 }
//             }
        }
    }

    return true;
}

bool WebSocketCarrier::expectExtraHeader(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::respondToHeader(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);

    auto& ciccio = proto.os();

    std::string reply {
"HTTP/1.1 101 Switching Protocols\r\n"
"Upgrade: websocket\r\n"
"Connection: upgrade\r\n"
"Sec-WebSocket-Accept: <KEY>\r\n"
"\r\n"};

// "Sec-WebSocket-Version: <VER>\r\n"
// "Sec-WebSocket-Extensions: <EXT>\r\n"
    reply = std::regex_replace(reply, std::regex("<KEY>"), acceptKey);
//     reply = std::regex_replace(reply, std::regex("<VER>"), std::to_string(webSocketVersion));
//     reply = std::regex_replace(reply, std::regex("<EXT>"), webSocketExtension);
    yCDebug(WEBSOCKETCARRIER) << "REPLY:" << reply;
    ciccio.writeLine(reply.c_str(), reply.length());


//     std::string result = proto.is().readLine();
//     yCDebug(WEBSOCKETCARRIER) << result.size() << result;
//     std::string x {"\r\n"};
//     ciccio.writeLine(x.c_str(), x.length());

    return true;
}

bool WebSocketCarrier::isActive() const
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::write(ConnectionState& proto, SizedWriter& writer)
{
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::reply(ConnectionState& proto, SizedWriter& writer)
{
    yCTrace(WEBSOCKETCARRIER);
    return false;
}


bool WebSocketCarrier::sendIndex(yarp::os::ConnectionState& proto, yarp::os::SizedWriter& writer) {
    yCTrace(WEBSOCKETCARRIER);
    return true;
}

bool WebSocketCarrier::expectIndex(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);

    yarp::os::ManagedBytes b;
    b.allocate(2);
    proto.is().read(b.bytes());

    bool mask = static_cast<bool>(b.get()[1] & 0b10000000);
    auto length = static_cast<size_t>(b.get()[1] & 0b0111111);

    yCInfo(WEBSOCKETCARRIER) << "length" << length;
    yCInfo(WEBSOCKETCARRIER) << "mask" << mask;

    b.allocate(length);
    proto.is().read(b.bytes());

    yCInfo(WEBSOCKETCARRIER) << b.get();
/*
    bool done = false;
    while (!done) {
        std::string result = proto.is().readLine();
        yCDebug(WEBSOCKETCARRIER) << result;
        if (result.empty()) {
            done = true;
        } else {
//             Bottle b;
//             b.fromString(result);
            yCInfo(WEBSOCKETCARRIER) << result;
        }
    }*/
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

std::string WebSocketCarrier::getBootstrapCarrierName() const
{
    yCTrace(WEBSOCKETCARRIER);
    return {};
}



// BEGIN UNUSED

void WebSocketCarrier::getHeader(yarp::os::Bytes& header) const
{
    yCTrace(WEBSOCKETCARRIER);
    // GET /?action=stream HTTP/1.1
    const char *target = "GET /?ws";
    for (size_t i=0; i<8 && i<header.length(); i++) {
        header.get()[i] = target[i];
    }
}


bool WebSocketCarrier::prepareSend(yarp::os::ConnectionState& proto)
{
    yCTrace(WEBSOCKETCARRIER);
    // nothing special to do
    return true;
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

// END UNUSED
