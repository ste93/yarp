/*
 * SPDX-FileCopyrightText: 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Autogenerated by Thrift Compiler (0.14.1-yarped)
//
// This is an automatically generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_SENSORMEASUREMENT_H
#define YARP_THRIFT_GENERATOR_STRUCT_SENSORMEASUREMENT_H

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>
#include <yarp/sig/Vector.h>

class SensorMeasurement :
        public yarp::os::idl::WirePortable
{
public:
    // Fields
    yarp::sig::Vector measurement{};
    double timestamp{0.0};

    // Default constructor
    SensorMeasurement() = default;

    // Constructor with field values
    SensorMeasurement(const yarp::sig::Vector& measurement,
                      const double timestamp);

    // Read structure on a Wire
    bool read(yarp::os::idl::WireReader& reader) override;

    // Read structure on a Connection
    bool read(yarp::os::ConnectionReader& connection) override;

    // Write structure on a Wire
    bool write(const yarp::os::idl::WireWriter& writer) const override;

    // Write structure on a Connection
    bool write(yarp::os::ConnectionWriter& connection) const override;

    // Convert to a printable string
    std::string toString() const;

    // If you want to serialize this class without nesting, use this helper
    typedef yarp::os::idl::Unwrapped<SensorMeasurement> unwrapped;

    class Editor :
            public yarp::os::Wire,
            public yarp::os::PortWriter
    {
    public:
        // Editor: default constructor
        Editor();

        // Editor: constructor with base class
        Editor(SensorMeasurement& obj);

        // Editor: destructor
        ~Editor() override;

        // Editor: Deleted constructors and operator=
        Editor(const Editor& rhs) = delete;
        Editor(Editor&& rhs) = delete;
        Editor& operator=(const Editor& rhs) = delete;
        Editor& operator=(Editor&& rhs) = delete;

        // Editor: edit
        bool edit(SensorMeasurement& obj, bool dirty = true);

        // Editor: validity check
        bool isValid() const;

        // Editor: state
        SensorMeasurement& state();

        // Editor: start editing
        void start_editing();

#ifndef YARP_NO_DEPRECATED // Since YARP 3.2
        YARP_DEPRECATED_MSG("Use start_editing() instead")
        void begin()
        {
            start_editing();
        }
#endif // YARP_NO_DEPRECATED

        // Editor: stop editing
        void stop_editing();

#ifndef YARP_NO_DEPRECATED // Since YARP 3.2
        YARP_DEPRECATED_MSG("Use stop_editing() instead")
        void end()
        {
            stop_editing();
        }
#endif // YARP_NO_DEPRECATED

        // Editor: measurement field
        void set_measurement(const yarp::sig::Vector& measurement);
        const yarp::sig::Vector& get_measurement() const;
        virtual bool will_set_measurement();
        virtual bool did_set_measurement();

        // Editor: timestamp field
        void set_timestamp(const double timestamp);
        double get_timestamp() const;
        virtual bool will_set_timestamp();
        virtual bool did_set_timestamp();

        // Editor: clean
        void clean();

        // Editor: read
        bool read(yarp::os::ConnectionReader& connection) override;

        // Editor: write
        bool write(yarp::os::ConnectionWriter& connection) const override;

    private:
        // Editor: state
        SensorMeasurement* obj;
        bool obj_owned;
        int group;

        // Editor: dirty variables
        bool is_dirty;
        bool is_dirty_measurement;
        bool is_dirty_timestamp;
        int dirty_count;

        // Editor: send if possible
        void communicate();

        // Editor: mark dirty overall
        void mark_dirty();

        // Editor: mark dirty single fields
        void mark_dirty_measurement();
        void mark_dirty_timestamp();

        // Editor: dirty_flags
        void dirty_flags(bool flag);
    };

private:
    // read/write measurement field
    bool read_measurement(yarp::os::idl::WireReader& reader);
    bool write_measurement(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_measurement(yarp::os::idl::WireReader& reader);
    bool nested_write_measurement(const yarp::os::idl::WireWriter& writer) const;

    // read/write timestamp field
    bool read_timestamp(yarp::os::idl::WireReader& reader);
    bool write_timestamp(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_timestamp(yarp::os::idl::WireReader& reader);
    bool nested_write_timestamp(const yarp::os::idl::WireWriter& writer) const;
};

#endif // YARP_THRIFT_GENERATOR_STRUCT_SENSORMEASUREMENT_H
