/*
 * SPDX-FileCopyrightText: 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Autogenerated by Thrift Compiler (0.14.1-yarped)
//
// This is an automatically generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_MAP2DLOCATIONDATA_H
#define YARP_THRIFT_GENERATOR_STRUCT_MAP2DLOCATIONDATA_H

#include <yarp/dev/api.h>

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

namespace yarp::dev {

class YARP_dev_API Map2DLocationData :
        public yarp::os::idl::WirePortable
{
public:
    // Fields
    /**
     * name of the map
     */
    std::string map_id{};
    /**
     * x position of the location [m], expressed in the map reference frame
     */
    double x{0.0};
    /**
     * y position of the location [m], expressed in the map reference frame
     */
    double y{0.0};
    /**
     * orientation [deg] in the map reference frame
     */
    double theta{0.0};
    /**
     * user defined string
     */
    std::string description{};

    // Default constructor
    Map2DLocationData() = default;

    // Constructor with field values
    Map2DLocationData(const std::string& map_id,
                      const double x,
                      const double y,
                      const double theta,
                      const std::string& description);

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
    typedef yarp::os::idl::Unwrapped<Map2DLocationData> unwrapped;

    class Editor :
            public yarp::os::Wire,
            public yarp::os::PortWriter
    {
    public:
        // Editor: default constructor
        Editor();

        // Editor: constructor with base class
        Editor(Map2DLocationData& obj);

        // Editor: destructor
        ~Editor() override;

        // Editor: Deleted constructors and operator=
        Editor(const Editor& rhs) = delete;
        Editor(Editor&& rhs) = delete;
        Editor& operator=(const Editor& rhs) = delete;
        Editor& operator=(Editor&& rhs) = delete;

        // Editor: edit
        bool edit(Map2DLocationData& obj, bool dirty = true);

        // Editor: validity check
        bool isValid() const;

        // Editor: state
        Map2DLocationData& state();

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

        // Editor: map_id field
        void set_map_id(const std::string& map_id);
        const std::string& get_map_id() const;
        virtual bool will_set_map_id();
        virtual bool did_set_map_id();

        // Editor: x field
        void set_x(const double x);
        double get_x() const;
        virtual bool will_set_x();
        virtual bool did_set_x();

        // Editor: y field
        void set_y(const double y);
        double get_y() const;
        virtual bool will_set_y();
        virtual bool did_set_y();

        // Editor: theta field
        void set_theta(const double theta);
        double get_theta() const;
        virtual bool will_set_theta();
        virtual bool did_set_theta();

        // Editor: description field
        void set_description(const std::string& description);
        const std::string& get_description() const;
        virtual bool will_set_description();
        virtual bool did_set_description();

        // Editor: clean
        void clean();

        // Editor: read
        bool read(yarp::os::ConnectionReader& connection) override;

        // Editor: write
        bool write(yarp::os::ConnectionWriter& connection) const override;

    private:
        // Editor: state
        Map2DLocationData* obj;
        bool obj_owned;
        int group;

        // Editor: dirty variables
        bool is_dirty;
        bool is_dirty_map_id;
        bool is_dirty_x;
        bool is_dirty_y;
        bool is_dirty_theta;
        bool is_dirty_description;
        int dirty_count;

        // Editor: send if possible
        void communicate();

        // Editor: mark dirty overall
        void mark_dirty();

        // Editor: mark dirty single fields
        void mark_dirty_map_id();
        void mark_dirty_x();
        void mark_dirty_y();
        void mark_dirty_theta();
        void mark_dirty_description();

        // Editor: dirty_flags
        void dirty_flags(bool flag);
    };

private:
    // read/write map_id field
    bool read_map_id(yarp::os::idl::WireReader& reader);
    bool write_map_id(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_map_id(yarp::os::idl::WireReader& reader);
    bool nested_write_map_id(const yarp::os::idl::WireWriter& writer) const;

    // read/write x field
    bool read_x(yarp::os::idl::WireReader& reader);
    bool write_x(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_x(yarp::os::idl::WireReader& reader);
    bool nested_write_x(const yarp::os::idl::WireWriter& writer) const;

    // read/write y field
    bool read_y(yarp::os::idl::WireReader& reader);
    bool write_y(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_y(yarp::os::idl::WireReader& reader);
    bool nested_write_y(const yarp::os::idl::WireWriter& writer) const;

    // read/write theta field
    bool read_theta(yarp::os::idl::WireReader& reader);
    bool write_theta(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_theta(yarp::os::idl::WireReader& reader);
    bool nested_write_theta(const yarp::os::idl::WireWriter& writer) const;

    // read/write description field
    bool read_description(yarp::os::idl::WireReader& reader);
    bool write_description(const yarp::os::idl::WireWriter& writer) const;
    bool nested_read_description(yarp::os::idl::WireReader& reader);
    bool nested_write_description(const yarp::os::idl::WireWriter& writer) const;
};

} // namespace yarp::dev

#endif // YARP_THRIFT_GENERATOR_STRUCT_MAP2DLOCATIONDATA_H
