/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_DEV_FRAMETRANSFORMGET_NWC_YARP_H
#define YARP_DEV_FRAMETRANSFORMGET_NWC_YARP_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IFrameTransformStorage.h>


#include <FrameTransformStorageGetRPC.h>

/*
 * \section FrameTransformGet_nwc_yarp_device_parameters Description of input parameters
 * This device is paired with its server called FrameTransformGet_nws_yarp.
 * It is attached to a device with an IFrameTransformStorageGet interface and then
 * when it receives a call it forwards this call on a port (the interface is declared
 * in thrift and is FrameTransformStorageGetRPC) and it returns all the transforms received
 * from the port to the caller.
 * For how to attach the various devices see FrameTransformClient.

 *   Parameters required by this device are:
 * | Parameter name  | SubParameter            | Type    | Units          | Default Value                   | Required     | Description                            |
 * |:---------------:|:-----------------------:|:-------:|:--------------:|:-------------------------------:|:-----------: |:--------------------------------------:|
 * | rpc_port_client |      -                  | string  | -              |   /frameTransformGet/clientRPC  | No           | port on which rpc calls should be made |
 * | rpc_port_server |      -                  | string  | -              |   /frameTransformGet/serverRPC  | No           | port on which rpc calls should be made |
 *
 * Some example of configuration files:
 *
 * Example of configuration file using .ini format.
 *
 * \code{.unparsed}
 * device FrameTransformGet_nwc_yarp
 * rpc_port_client /frameTransformGet/clientRPC
 * rpc_port_server /frameTransformGet/clientRPC
 * \endcode
 */

class FrameTransformGet_nwc_yarp:
    public yarp::dev::DeviceDriver,
    public yarp::dev::IFrameTransformStorageGet
{
public:
    FrameTransformGet_nwc_yarp() = default;
    FrameTransformGet_nwc_yarp(const FrameTransformGet_nwc_yarp&) = delete;
    FrameTransformGet_nwc_yarp(FrameTransformGet_nwc_yarp&&) = delete;
    FrameTransformGet_nwc_yarp& operator=(const FrameTransformGet_nwc_yarp&) = delete;
    FrameTransformGet_nwc_yarp& operator=(FrameTransformGet_nwc_yarp&&) = delete;
    ~FrameTransformGet_nwc_yarp() override = default;

    // yarp::dev::DeviceDriver
    bool  open(yarp::os::Searchable &params) override;
    bool  close() override;

    // yarp::dev::IFrameTransformStorageGet
    bool getTransforms(std::vector<yarp::math::FrameTransform>& transforms) const override;

private:
    int    m_verbose{4};

    // for the RPC with the NWS
    yarp::os::Port      m_thrift_rpcPort;
    std::string         m_thrift_rpcPort_Name{"/frameTransformGet/clientRPC"};
    std::string         m_thrift_server_rpcPort_Name{"/frameTransformGet/serverRPC"};
    mutable FrameTransformStorageGetRPC m_frameTransformStorageGetRPC;

};

#endif // YARP_DEV_FRAMETRANSFORMGET_NWC_YARP_H
