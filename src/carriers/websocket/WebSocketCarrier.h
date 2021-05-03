/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef WEBSOCKETCARRIER_H
#define WEBSOCKETCARRIER_H

#include <yarp/os/Carrier.h>
#include <yarp/os/LogComponent.h>


#include <yarp/os/LogStream.h>
#include <yarp/os/NetType.h>
#include <yarp/os/ConnectionState.h>

#include <cstring>

YARP_DECLARE_LOG_COMPONENT(WEBSOCKETCARRIER)


class WebSocketCarrier :
        public yarp::os::Carrier
{
private:
    static constexpr size_t header_lenght {8};
    bool isWebSocket{false};
    int webSocketVersion;
    std::string webSocketExtension;
    std::string acceptKey;

    bool firstRound {true};
    bool sender {false};
    std::string envelope;
public:
    WebSocketCarrier() = default;

    Carrier *create() const override;

    std::string getName() const override;

    bool isConnectionless() const override;

    bool canAccept() const override;

    bool canOffer() const override;

    bool isTextMode() const override;

    bool canEscape() const override;

    void handleEnvelope(const std::string& envelope) override;

    bool requireAck() const override;

    bool supportReply() const override;

    bool isLocal() const override;

    bool isPush() const override;

    std::string toString() const override;


    bool checkHeader(const yarp::os::Bytes& header) override;

    void setParameters(const yarp::os::Bytes& header) override;



    bool expectSenderSpecifier(yarp::os::ConnectionState& proto) override;

    bool expectExtraHeader(yarp::os::ConnectionState& proto) override;

    bool respondToHeader(yarp::os::ConnectionState& proto) override;

    bool isActive() const override;


    // Payload time!

    bool write(yarp::os::ConnectionState& proto, yarp::os::SizedWriter& writer) override;

    bool reply(yarp::os::ConnectionState& proto, yarp::os::SizedWriter& writer) override;

    virtual bool sendIndex(yarp::os::ConnectionState& proto, yarp::os::SizedWriter& writer);

    bool expectIndex(yarp::os::ConnectionState& proto) override;

    bool sendAck(yarp::os::ConnectionState& proto) override;

    bool expectAck(yarp::os::ConnectionState& proto) override;

    std::string getBootstrapCarrierName() const override;

// BEGIN UNUSED
    void getHeader(yarp::os::Bytes& header) const override;
    bool prepareSend(yarp::os::ConnectionState& proto) override;
    bool sendHeader(yarp::os::ConnectionState& proto) override;
    bool expectReplyToHeader(yarp::os::ConnectionState& proto) override;
// END UNUSED
};

#endif
