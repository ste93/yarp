/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_FRAMEGRABBER_NWS_ROS_H
#define YARP_FRAMEGRABBER_NWS_ROS_H

#include <yarp/os/Node.h>
#include <yarp/os/Publisher.h>
#include <yarp/os/PeriodicThread.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Stamp.h>

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IFrameGrabberControls.h>
#include <yarp/dev/IFrameGrabberControlsDC1394.h>
#include <yarp/dev/IFrameGrabberImage.h>
#include <yarp/dev/IPreciselyTimed.h>
#include <yarp/dev/IRgbVisualParams.h>
#include <yarp/dev/WrapperSingle.h>

#include <yarp/rosmsg/sensor_msgs/CameraInfo.h>
#include <yarp/rosmsg/sensor_msgs/Image.h>

/**
 *  @ingroup dev_impl_wrapper
 *
 *  Parameters required by this device are:
 * | Parameter name  | Type   | Units   | Default Value | Required  | Description                                                                                  | Notes |
 * |:---------------:|:------:|:-------:|:-------------:|:--------: |:--------------------------------------------------------------------------------------------:|:-----:|
 * | period          | float  | seconds |  0.03 s       | No        | the period of publication                                                                    |       |
 * | node_name       | String | -       | -             | Yes       | the name of the ros node                                                                     | must begin with /      |
 * | base_topic_name | String | -       | -             | Yes       | the base name of the topic, the subtopics will be name/image_rect_color and name/camera_info | must begin with /      |
 * | frame_id        | String | -       | -             | Yes       | the frame where the grabber is placed                                                        |       |
 * | subdevice       | string | -       |  -            | Yes       | the name of the grabber used                                                                 | -     |
 *
 */

class FrameGrabber_nws_ros :
        public yarp::dev::DeviceDriver,
        public yarp::dev::WrapperSingle,
        public yarp::os::PeriodicThread
{
private:
    // Publishers
    typedef yarp::os::Publisher<yarp::rosmsg::sensor_msgs::Image> ImageTopicType;
    typedef yarp::os::Publisher<yarp::rosmsg::sensor_msgs::CameraInfo> CameraInfoTopicType;

    yarp::os::Node* node {nullptr};
    ImageTopicType publisherPort_image;
    CameraInfoTopicType publisherPort_cameraInfo;

    // Subdevice
    yarp::dev::PolyDriver* subdevice {nullptr};
    bool isSubdeviceOwned {false};

    // Interfaces handled
    yarp::dev::IRgbVisualParams* iRgbVisualParams {nullptr};
    yarp::dev::IFrameGrabberImage* iFrameGrabberImage {nullptr};
    yarp::dev::IPreciselyTimed* iPreciselyTimed {nullptr};

    // Images
    yarp::sig::ImageOf<yarp::sig::PixelRgb>* img {nullptr};

    // Internal state
    bool m_active {false};
    yarp::os::Stamp m_stamp;
    std::string m_frameId;

    // Options
    static constexpr double s_default_period = 0.03; // seconds
    double m_period {s_default_period};

    bool setCamInfo(yarp::rosmsg::sensor_msgs::CameraInfo& cameraInfo);

public:
    FrameGrabber_nws_ros();
    FrameGrabber_nws_ros(const FrameGrabber_nws_ros&) = delete;
    FrameGrabber_nws_ros(FrameGrabber_nws_ros&&) = delete;
    FrameGrabber_nws_ros& operator=(const FrameGrabber_nws_ros&) = delete;
    FrameGrabber_nws_ros& operator=(FrameGrabber_nws_ros&&) = delete;
    ~FrameGrabber_nws_ros() override;

    // DeviceDriver
    bool close() override;
    bool open(yarp::os::Searchable& config) override;

    // IWrapper interface
    bool attach(yarp::dev::PolyDriver* poly) override;
    bool detach() override;

    //RateThread
    bool threadInit() override;
    void threadRelease() override;
    void run() override;
};

#endif // YARP_FRAMEGRABBER_NWS_ROS_H
