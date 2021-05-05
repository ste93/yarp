/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include <yarp/os/impl/TcpCarrier.h>

#include <yarp/os/ConnectionState.h>
#include <yarp/os/LogComponent.h>


#include <yarp/os/LogStream.h>
#include <yarp/os/TwoWayStream.h>

#include <string>

using namespace yarp::os;
YARP_LOG_COMPONENT(TCPCARRIER,
                   "yarp.carrier.tcp",
                   yarp::os::Log::TraceType,
                   yarp::os::Log::LogTypeReserved,
                   yarp::os::Log::printCallback(),
                   nullptr)


yarp::os::impl::TcpCarrier::TcpCarrier(bool requireAckFlag)
{
    yCTrace(TCPCARRIER);
    this->requireAckFlag = requireAckFlag;
}

yarp::os::Carrier* yarp::os::impl::TcpCarrier::create() const
{
    yCTrace(TCPCARRIER);
    return new TcpCarrier(requireAckFlag);
}

std::string yarp::os::impl::TcpCarrier::getName() const
{
    yCTrace(TCPCARRIER);
    return requireAckFlag ? "tcp" : "fast_tcp";
}

int yarp::os::impl::TcpCarrier::getSpecifierCode() const
{
    yCTrace(TCPCARRIER);
    return 3;
}

bool yarp::os::impl::TcpCarrier::checkHeader(const yarp::os::Bytes& header)
{
    yCTrace(TCPCARRIER);
    int spec = getSpecifier(header);
    if (spec % 16 == getSpecifierCode()) {
        if (((spec & 128) != 0) == requireAckFlag) {
            return true;
        }
    }
    return false;
}

void yarp::os::impl::TcpCarrier::getHeader(yarp::os::Bytes& header) const
{
    yCTrace(TCPCARRIER);
    createStandardHeader(getSpecifierCode() + (requireAckFlag ? 128 : 0), header);
}

void yarp::os::impl::TcpCarrier::setParameters(const yarp::os::Bytes& header)
{
    yCTrace(TCPCARRIER);
    YARP_UNUSED(header);
    //int specifier = getSpecifier(header);
    //requireAckFlag = (specifier&128)!=0;
    // Now prefilter by ack flag
}

bool yarp::os::impl::TcpCarrier::requireAck() const
{
    yCTrace(TCPCARRIER);
    return requireAckFlag;
}

bool yarp::os::impl::TcpCarrier::isConnectionless() const
{
    yCTrace(TCPCARRIER);
    return false;
}

bool yarp::os::impl::TcpCarrier::respondToHeader(ConnectionState& proto)
{
    yCTrace(TCPCARRIER);
    int cport = proto.getStreams().getLocalAddress().getPort();
    writeYarpInt(cport, proto);
    return proto.checkStreams();
}

bool yarp::os::impl::TcpCarrier::expectReplyToHeader(ConnectionState& proto)
{
    yCTrace(TCPCARRIER);
    readYarpInt(proto); // ignore result
    return proto.checkStreams();
}
