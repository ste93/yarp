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
#include <iostream>


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
    std::string result = "";

    Route route = proto.getRoute();
    route.setFromName("web");
    proto.setRoute(route);
    std::string remainder = proto.is().readLine();
    result += remainder;
    result += "\r\n";

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
    while (!done) {
        std::string line = proto.is().readLine();
        result += line;
        result += "\r\n";
        if (line.empty()) {
            done = true;
        } 

    }
    yCDebug(WEBSOCKETCARRIER)  << result.size() << "all http message " << result.c_str() ;
    auto messagetype = messagHandler.parseHandshake(reinterpret_cast<unsigned char*>(const_cast<char*>(result.c_str())), result.size());
    std::stringstream sstream;
    sstream << std::hex << messagetype;
    yCDebug(WEBSOCKETCARRIER)  << "message type " << sstream.str() ;
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

    std::string reply = messagHandler.answerHandshake();

// "Sec-WebSocket-Version: <VER>\r\n"
// "Sec-WebSocket-Extensions: <EXT>\r\n"
    reply = std::regex_replace(reply, std::regex("<KEY>"), "acceptKey");
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
    yarp::os::ManagedBytes header;
    yarp::os::ManagedBytes mask_bytes;
    header.allocate(2);
    proto.is().read(header.bytes());

	unsigned char msg_opcode = header.get()[0] & 0x0F;
	unsigned char msg_fin = (header.get()[0] >> 7) & 0x01;
	unsigned char msg_masked = (header.get()[1] >> 7) & 0x01;

	// *** message decoding 

	yarp::os::NetInt32 payload_length = 0;
	int pos = 2;
	int length_field = header.get()[1] & (0x7F);
	unsigned int mask = 0;
    yCDebug(WEBSOCKETCARRIER) <<length_field;

	if(length_field <= 125) {
		payload_length = length_field;
	}
	else if(length_field == 126) { //msglen is 16bit!
	    yarp::os::ManagedBytes additionalLength;
		additionalLength.allocate(2);
	    proto.is().read(additionalLength.bytes());

        std::bitset<8> x(additionalLength.get()[0]);
        std::bitset<8> y(additionalLength.get()[1]);
        std::cout << x << y ;

		payload_length = (
			((unsigned char)additionalLength.get()[0] << 8) | 
			((unsigned char)additionalLength.get()[1]) );
		pos += 2;
        yCDebug(WEBSOCKETCARRIER) <<payload_length;

	}
	else if(length_field == 127) { //msglen is 64bit!
		yarp::os::ManagedBytes additionalLength;
		additionalLength.allocate(8);
	    proto.is().read(additionalLength.bytes());

		payload_length = (
			((unsigned char)additionalLength.get()[0] << 56) | 
			((unsigned char)additionalLength.get()[1] << 48) | 
			((unsigned char)additionalLength.get()[2] << 40) | 
			((unsigned char)additionalLength.get()[3] << 32) | 
			((unsigned char)additionalLength.get()[4] << 24) | 
			((unsigned char)additionalLength.get()[5] << 16) | 
			((unsigned char)additionalLength.get()[6] << 8) | 
			((unsigned char)additionalLength.get()[7])
		); 
		pos += 8;
	}
		
	//printf("PAYLOAD_LEN: %08x\n", payload_length);

    yCDebug(WEBSOCKETCARRIER) <<payload_length;

	if(msg_masked)
    {
        // get the mask
        mask_bytes.allocate(4);
        proto.is().read(mask_bytes.bytes());
		//printf("MASK: %08x\n", mask);
		pos += 4;
    }
    yCDebug(WEBSOCKETCARRIER) << payload_length;
    yarp::os::ManagedBytes payload;
	payload.allocate(payload_length);
	proto.is().read(payload.bytes());

    if(msg_masked)
    {
		// unmask data:
		for(int i=0; i<payload_length; i++) {
			payload.get()[i] = payload.get()[i] ^ mask_bytes.get()[i%4];
		}
	}
    std::string myString(payload.get());
	yCDebug(WEBSOCKETCARRIER) << myString;
	//printf("TEXT: %s\n", out_buffer);

	// if(msg_opcode == 0x0) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME; // continuation frame ?
	// if(msg_opcode == 0x1) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME;
	// if(msg_opcode == 0x2) return (msg_fin)?BINARY_FRAME:INCOMPLETE_BINARY_FRAME;
	// if(msg_opcode == 0x9) return PING_FRAME;
	// if(msg_opcode == 0xA) return PONG_FRAME;

	// return ERROR_FRAME;
    // yarp::os::ManagedBytes b;
    // yarp::os::ManagedBytes c;
    // b.allocate(2);
    // proto.is().read(b.bytes());

    // bool mask = static_cast<bool>(b.get()[1] & 0b10000000);
    // auto length = static_cast<size_t>(b.get()[1] & 0b0111111);

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
