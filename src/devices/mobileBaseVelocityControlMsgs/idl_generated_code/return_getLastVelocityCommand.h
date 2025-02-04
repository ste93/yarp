/*
 * SPDX-FileCopyrightText: 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Autogenerated by Thrift Compiler (0.14.1-yarped)
//
// This is an automatically generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_RETURN_GETLASTVELOCITYCOMMAND_H
#define YARP_THRIFT_GENERATOR_STRUCT_RETURN_GETLASTVELOCITYCOMMAND_H

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class return_getLastVelocityCommand :
        public yarp::os::idl::WirePortable
{
public:
    // Fields
    bool retvalue{false};
    double x_vel{0.0};
    double y_vel{0.0};
    double theta_vel{0.0};

    // Default constructor
    return_getLastVelocityCommand() = default;

    // Constructor with field values
    return_getLastVelocityCommand(const bool retvalue,
                                  const double x_vel,
                                  const double y_vel,
                                  const double theta_vel);

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
    typedef yarp::os::idl::Unwrapped<return_getLastVelocityCommand> unwrapped;

    class Editor :
            public yarp::os::Wire,
            public yarp::os::PortWriter
    {
    public:
        // Editor: default constructor
        Editor();

        // Editor: constructor with base class
        Editor(return_getLastVelocityCommand& obj);

        // Editor: destructor
        ~Editor() override;

        // Editor: Deleted constructors and operator=
        Editor(const Editor& rhs) = delete;
        Editor(Editor&& rhs) = delete;
        Editor& operator=(const Editor& rhs) = delete;
        Editor& operator=(Editor&& rhs) = delete;

        // Editor: edit
        bool edit(return_getLastVelocityCommand& obj, bool dirty = true);

        // Editor: validity check
        bool isValid() const;

        // Editor: state
        return_getLastVelocityCommand& state();

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

        // Editor: retvalue field
        void set_retvalue(const bool retvalue);
        bool get_retvalue() const;
        virtual bool will_set_retvalue();
        virtual bool did_set_retvalue();

        // Editor: x_vel field
        void set_x_vel(const double x_vel);
        double get_x_vel() const;
        virtual bool will_set_x_vel();
        virtual bool did_set_x_vel();

        // Editor: y_vel field
        void set_y_vel(const double y_vel);
        double get_y_vel() const;
        virtual bool will_set_y_vel();
        virtual bool did_set_y_vel();

        // Editor: theta_vel field
        void set_theta_vel(const double theta_vel);
        double get_theta_vel() const;
        virtual bool will_set_theta_vel();
        virtual bool did_set_theta_vel();

        // Editor: clean
        void clean();

        // Editor: read
        bool read(yarp::os::ConnectionReader& connection) override;

        // Editor: write
        bool write(yarp::os::ConnectionWriter& connection) const override;

    private:
        // Editor: state
        return_getLastVelocityCommand* obj;
        bool obj_owned;
        int group;

        // Editor: dirty variables
        bool is_dirty;
        bool is_dirty_retvalue;
        bool is_dirty_x_vel;
        bool is_dirty_y_vel;
        bool is_dirty_theta_vel;
        int dirty_count;

        // Editor: send if possible
        void communicate();

        // Editor: mark dirty overall
        void mark_dirty();

        // Editor: mark dirty single fields
        void mark_dirty_retvalue();
        void mark_dirty_x_vel();
        void mark_dirty_y_vel();
        void mark_dirty_theta_vel();

        // Editor: dirty_flags
        void dirty_flags(bool flag);
    };

private:
    // read/write retvalue field
    bool read_retvalue(yarp::os::idl::WireReader& reader);
    bool write_retvalue(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_retvalue(yarp::os::idl::WireReader& reader);
    bool nested_write_retvalue(const yarp::os::idl::WireWriter& writer) const;

    // read/write x_vel field
    bool read_x_vel(yarp::os::idl::WireReader& reader);
    bool write_x_vel(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_x_vel(yarp::os::idl::WireReader& reader);
    bool nested_write_x_vel(const yarp::os::idl::WireWriter& writer) const;

    // read/write y_vel field
    bool read_y_vel(yarp::os::idl::WireReader& reader);
    bool write_y_vel(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_y_vel(yarp::os::idl::WireReader& reader);
    bool nested_write_y_vel(const yarp::os::idl::WireWriter& writer) const;

    // read/write theta_vel field
    bool read_theta_vel(yarp::os::idl::WireReader& reader);
    bool write_theta_vel(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_theta_vel(yarp::os::idl::WireReader& reader);
    bool nested_write_theta_vel(const yarp::os::idl::WireWriter& writer) const;
};

#endif // YARP_THRIFT_GENERATOR_STRUCT_RETURN_GETLASTVELOCITYCOMMAND_H
