/*
 * Copyright (C) 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_DEV_IFRAMETRANSFORM_STORAGE_H
#define YARP_DEV_IFRAMETRANSFORM_STORAGE_H

#include <string>
#include <vector>

#include <yarp/dev/api.h>
#include <yarp/os/Vocab.h>
#include <yarp/math/FrameTransform.h>


namespace yarp {
    namespace dev {
        class IFrameTransformStorageSet;
        class IFrameTransformStorageGet;
        class IFrameTransformStorageUtils;
      }
}

/**
 * @ingroup dev_iface_transform
 *
 * Transform Interface.
 */
class YARP_dev_API yarp::dev::IFrameTransformStorageSet
{
public:
    /**
     * Destructor.
     */
    virtual ~IFrameTransformStorageSet();

    /**
    * Save some frame transforms in a storage.
    * @param transforms the list of transforms to be stored
    * @return true/false
    */
    virtual bool setTransforms(const std::vector<yarp::math::FrameTransform>& transforms) = 0;

    /**
    * Save a frame transform in a storage.
    * @param transforms the transform to be stored
    * @return true/false
    */
    virtual bool setTransform(const yarp::math::FrameTransform& transform) = 0;
};

/**
 * @ingroup dev_iface_transform
 *
 * Transform Interface.
 */
class YARP_dev_API yarp::dev::IFrameTransformStorageGet
{
public:
    /**
     * Destructor.
     */
    virtual ~IFrameTransformStorageGet();

    /**
    * Obtains all frame transforms saved in a storage.
    * @param transforms the returned list of frame transforms
    * @return true/false
    */
    virtual bool getTransforms(std::vector<yarp::math::FrameTransform>& transforms) const = 0;

};

/**
 * @ingroup dev_iface_transform
 *
 * Transform Interface.
 */
class YARP_dev_API yarp::dev::IFrameTransformStorageUtils
{
public:
    /**
     * Destructor.
     */
    virtual ~IFrameTransformStorageUtils();

    virtual bool clear() = 0;

    virtual bool deleteTransform(std::string t1, std::string t2) = 0;

    virtual bool size (size_t& size) const =0;

};

#endif // YARP_DEV_IFRAMETRANSFORM_STORAGE_H
