/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef YARP_DEV_LOCALIZATION2D_NWS_ROS_H
#define YARP_DEV_LOCALIZATION2D_NWS_ROS_H


#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Time.h>
#include <yarp/os/Port.h>
#include <yarp/os/Stamp.h>
#include <yarp/os/Node.h>
#include <yarp/os/Publisher.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/PeriodicThread.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/IMultipleWrapper.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/ILocalization2D.h>
#include <yarp/dev/OdometryData.h>
#include <yarp/rosmsg/nav_msgs/Odometry.h>
#include <yarp/rosmsg/tf2_msgs/TFMessage.h>
#include <math.h>

 /**
 * @ingroup dev_impl_network_wrapper dev_impl_navigation
 *
 * \section Localization2D_nws_ros
 *
 * \brief `Localization2D_nws_ros`: A localization server which can be wrap multiple algorithms and devices to provide robot localization in a 2D World.
 *
 *
 *  Parameters required by this device are:
 * | Parameter name   | SubParameter      | Type    | Units          | Default Value            | Required     | Description                                                        | Notes |
 * |:----------------:|:-----------------:|:-------:|:--------------:|:------------------------:|:-----------: |:-----------------------------------------------------------------: |:-----:|
 * | GENERAL          |  period           | double  | s              | 0.01                     | No           | The period of the working thread                                   |       |
 * | GENERAL          |  yarp_base_name   | string  |  -             |                          | Yes          | The name of the server, used as a prefix for the opened yarp ports | By default ports opened are: /xxx/rpc |
 * | GENERAL          |  publish_odometry | bool    |  -             | true                     | No           | Periodically publish odometry data over the network                | -     |
 * | GENERAL          |  publish_tf       | bool    |  -             | true                     | No           | Periodically publish tf data over the network                      | -     |
 * | ROS              | parent_frame_id   | string  |  -             | odom                     | No           | The name of the of the parent frame published in the /tf topic     | -     |
 * | ROS              | child_frame_id    | string  |  -             | base_link                | No           | The name of the of the child frame published in the /tf topic      | -     |
 * | ROS              | base_topic_name   | string  |  -             |                          | Yes          | The name of the of the odometry topic                              | -     |
 * | ROS              | node_name         | string  |  -             |                          | Yes          | The name of the of the ROS node                                    | -     |
 * | subdevice        |  -                | string  |  -             |  -                       | Yes          | The name of the of Localization device to be used                  | -     |
 */
class Localization2D_nws_ros :
        public yarp::dev::DeviceDriver,
        public yarp::os::PeriodicThread,
        public yarp::dev::IMultipleWrapper,
        public yarp::os::PortReader
{
protected:

    //yarp
    std::string                               m_local_name = "/localization2D_nws_ros";
    yarp::os::Port                            m_rpcPort;
    std::string                               m_rpcPortName;
    std::string                               m_robot_frame;
    std::string                               m_fixed_frame;
    bool                                      m_enable_publish_odometry_topic = true;
    bool                                      m_enable_publish_odometry_tf = true;

    //ROS
    std::string                                           m_child_frame_id = "base_link";
    std::string                                           m_parent_frame_id = "odom";
    std::string                                           m_ros_node_name;
    yarp::os::Node*                                       m_ros_node = nullptr;
    std::string                                           m_odom_topic_name;
    yarp::os::Publisher<yarp::rosmsg::nav_msgs::Odometry> m_odometry_publisher;
    yarp::os::Publisher<yarp::rosmsg::tf2_msgs::TFMessage>  m_tf_publisher;

    //drivers and interfaces
    yarp::dev::PolyDriver                   pLoc;
    yarp::dev::Nav2D::ILocalization2D*      iLoc = nullptr;

    double                                  m_stats_time_last;
    double                                  m_period;
    yarp::os::Stamp                         m_loc_stamp;
    yarp::os::Stamp                         m_odom_stamp;

    yarp::dev::OdometryData                     m_current_odometry;
    yarp::dev::Nav2D::Map2DLocation             m_current_position;
    yarp::dev::Nav2D::LocalizationStatusEnum    m_current_status = yarp::dev::Nav2D::LocalizationStatusEnum::localization_status_not_yet_localized;

private:
    void publish_odometry_on_ROS_topic();
    void publish_odometry_on_TF_topic();

public:
    Localization2D_nws_ros();

public:
    virtual bool open(yarp::os::Searchable& prop) override;
    virtual bool close() override;
    virtual bool detachAll() override;
    virtual bool attachAll(const yarp::dev::PolyDriverList &l) override;
    virtual void run() override;

    bool initialize_YARP(yarp::os::Searchable &config);
    bool initialize_ROS(yarp::os::Searchable& config);
    virtual bool read(yarp::os::ConnectionReader& connection) override;
};

#endif // YARP_DEV_LOCALIZATION2D_NWS_ROS
