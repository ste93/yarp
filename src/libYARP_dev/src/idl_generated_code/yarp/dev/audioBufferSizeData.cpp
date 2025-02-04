/*
 * SPDX-FileCopyrightText: 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Autogenerated by Thrift Compiler (0.14.1-yarped)
//
// This is an automatically generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <yarp/dev/audioBufferSizeData.h>

namespace yarp::dev {

// Constructor with field values
audioBufferSizeData::audioBufferSizeData(const std::int32_t m_samples,
                                         const std::int32_t m_channels,
                                         const std::int32_t m_depth,
                                         const std::int32_t size) :
        WirePortable(),
        m_samples(m_samples),
        m_channels(m_channels),
        m_depth(m_depth),
        size(size)
{
}

// Read structure on a Wire
bool audioBufferSizeData::read(yarp::os::idl::WireReader& reader)
{
    if (!read_m_samples(reader)) {
        return false;
    }
    if (!read_m_channels(reader)) {
        return false;
    }
    if (!read_m_depth(reader)) {
        return false;
    }
    if (!read_size(reader)) {
        return false;
    }
    return !reader.isError();
}

// Read structure on a Connection
bool audioBufferSizeData::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListHeader(4)) {
        return false;
    }
    return read(reader);
}

// Write structure on a Wire
bool audioBufferSizeData::write(const yarp::os::idl::WireWriter& writer) const
{
    if (!write_m_samples(writer)) {
        return false;
    }
    if (!write_m_channels(writer)) {
        return false;
    }
    if (!write_m_depth(writer)) {
        return false;
    }
    if (!write_size(writer)) {
        return false;
    }
    return !writer.isError();
}

// Write structure on a Connection
bool audioBufferSizeData::write(yarp::os::ConnectionWriter& connection) const
{
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(4)) {
        return false;
    }
    return write(writer);
}

// Convert to a printable string
std::string audioBufferSizeData::toString() const
{
    yarp::os::Bottle b;
    b.read(*this);
    return b.toString();
}

// Editor: default constructor
audioBufferSizeData::Editor::Editor()
{
    group = 0;
    obj_owned = true;
    obj = new audioBufferSizeData;
    dirty_flags(false);
    yarp().setOwner(*this);
}

// Editor: constructor with base class
audioBufferSizeData::Editor::Editor(audioBufferSizeData& obj)
{
    group = 0;
    obj_owned = false;
    edit(obj, false);
    yarp().setOwner(*this);
}

// Editor: destructor
audioBufferSizeData::Editor::~Editor()
{
    if (obj_owned) {
        delete obj;
    }
}

// Editor: edit
bool audioBufferSizeData::Editor::edit(audioBufferSizeData& obj, bool dirty)
{
    if (obj_owned) {
        delete this->obj;
    }
    this->obj = &obj;
    obj_owned = false;
    dirty_flags(dirty);
    return true;
}

// Editor: validity check
bool audioBufferSizeData::Editor::isValid() const
{
    return obj != nullptr;
}

// Editor: state
audioBufferSizeData& audioBufferSizeData::Editor::state()
{
    return *obj;
}

// Editor: grouping begin
void audioBufferSizeData::Editor::start_editing()
{
    group++;
}

// Editor: grouping end
void audioBufferSizeData::Editor::stop_editing()
{
    group--;
    if (group == 0 && is_dirty) {
        communicate();
    }
}
// Editor: m_samples setter
void audioBufferSizeData::Editor::set_m_samples(const std::int32_t m_samples)
{
    will_set_m_samples();
    obj->m_samples = m_samples;
    mark_dirty_m_samples();
    communicate();
    did_set_m_samples();
}

// Editor: m_samples getter
std::int32_t audioBufferSizeData::Editor::get_m_samples() const
{
    return obj->m_samples;
}

// Editor: m_samples will_set
bool audioBufferSizeData::Editor::will_set_m_samples()
{
    return true;
}

// Editor: m_samples did_set
bool audioBufferSizeData::Editor::did_set_m_samples()
{
    return true;
}

// Editor: m_channels setter
void audioBufferSizeData::Editor::set_m_channels(const std::int32_t m_channels)
{
    will_set_m_channels();
    obj->m_channels = m_channels;
    mark_dirty_m_channels();
    communicate();
    did_set_m_channels();
}

// Editor: m_channels getter
std::int32_t audioBufferSizeData::Editor::get_m_channels() const
{
    return obj->m_channels;
}

// Editor: m_channels will_set
bool audioBufferSizeData::Editor::will_set_m_channels()
{
    return true;
}

// Editor: m_channels did_set
bool audioBufferSizeData::Editor::did_set_m_channels()
{
    return true;
}

// Editor: m_depth setter
void audioBufferSizeData::Editor::set_m_depth(const std::int32_t m_depth)
{
    will_set_m_depth();
    obj->m_depth = m_depth;
    mark_dirty_m_depth();
    communicate();
    did_set_m_depth();
}

// Editor: m_depth getter
std::int32_t audioBufferSizeData::Editor::get_m_depth() const
{
    return obj->m_depth;
}

// Editor: m_depth will_set
bool audioBufferSizeData::Editor::will_set_m_depth()
{
    return true;
}

// Editor: m_depth did_set
bool audioBufferSizeData::Editor::did_set_m_depth()
{
    return true;
}

// Editor: size setter
void audioBufferSizeData::Editor::set_size(const std::int32_t size)
{
    will_set_size();
    obj->size = size;
    mark_dirty_size();
    communicate();
    did_set_size();
}

// Editor: size getter
std::int32_t audioBufferSizeData::Editor::get_size() const
{
    return obj->size;
}

// Editor: size will_set
bool audioBufferSizeData::Editor::will_set_size()
{
    return true;
}

// Editor: size did_set
bool audioBufferSizeData::Editor::did_set_size()
{
    return true;
}

// Editor: clean
void audioBufferSizeData::Editor::clean()
{
    dirty_flags(false);
}

// Editor: read
bool audioBufferSizeData::Editor::read(yarp::os::ConnectionReader& connection)
{
    if (!isValid()) {
        return false;
    }
    yarp::os::idl::WireReader reader(connection);
    reader.expectAccept();
    if (!reader.readListHeader()) {
        return false;
    }
    int len = reader.getLength();
    if (len == 0) {
        yarp::os::idl::WireWriter writer(reader);
        if (writer.isNull()) {
            return true;
        }
        if (!writer.writeListHeader(1)) {
            return false;
        }
        writer.writeString("send: 'help' or 'patch (param1 val1) (param2 val2)'");
        return true;
    }
    std::string tag;
    if (!reader.readString(tag)) {
        return false;
    }
    if (tag == "help") {
        yarp::os::idl::WireWriter writer(reader);
        if (writer.isNull()) {
            return true;
        }
        if (!writer.writeListHeader(2)) {
            return false;
        }
        if (!writer.writeTag("many", 1, 0)) {
            return false;
        }
        if (reader.getLength() > 0) {
            std::string field;
            if (!reader.readString(field)) {
                return false;
            }
            if (field == "m_samples") {
                if (!writer.writeListHeader(1)) {
                    return false;
                }
                if (!writer.writeString("std::int32_t m_samples")) {
                    return false;
                }
            }
            if (field == "m_channels") {
                if (!writer.writeListHeader(1)) {
                    return false;
                }
                if (!writer.writeString("std::int32_t m_channels")) {
                    return false;
                }
            }
            if (field == "m_depth") {
                if (!writer.writeListHeader(1)) {
                    return false;
                }
                if (!writer.writeString("std::int32_t m_depth")) {
                    return false;
                }
            }
            if (field == "size") {
                if (!writer.writeListHeader(1)) {
                    return false;
                }
                if (!writer.writeString("std::int32_t size")) {
                    return false;
                }
            }
        }
        if (!writer.writeListHeader(5)) {
            return false;
        }
        writer.writeString("*** Available fields:");
        writer.writeString("m_samples");
        writer.writeString("m_channels");
        writer.writeString("m_depth");
        writer.writeString("size");
        return true;
    }
    bool nested = true;
    bool have_act = false;
    if (tag != "patch") {
        if (((len - 1) % 2) != 0) {
            return false;
        }
        len = 1 + ((len - 1) / 2);
        nested = false;
        have_act = true;
    }
    for (int i = 1; i < len; ++i) {
        if (nested && !reader.readListHeader(3)) {
            return false;
        }
        std::string act;
        std::string key;
        if (have_act) {
            act = tag;
        } else if (!reader.readString(act)) {
            return false;
        }
        if (!reader.readString(key)) {
            return false;
        }
        if (key == "m_samples") {
            will_set_m_samples();
            if (!obj->nested_read_m_samples(reader)) {
                return false;
            }
            did_set_m_samples();
        } else if (key == "m_channels") {
            will_set_m_channels();
            if (!obj->nested_read_m_channels(reader)) {
                return false;
            }
            did_set_m_channels();
        } else if (key == "m_depth") {
            will_set_m_depth();
            if (!obj->nested_read_m_depth(reader)) {
                return false;
            }
            did_set_m_depth();
        } else if (key == "size") {
            will_set_size();
            if (!obj->nested_read_size(reader)) {
                return false;
            }
            did_set_size();
        } else {
            // would be useful to have a fallback here
        }
    }
    reader.accept();
    yarp::os::idl::WireWriter writer(reader);
    if (writer.isNull()) {
        return true;
    }
    writer.writeListHeader(1);
    writer.writeVocab32('o', 'k');
    return true;
}

// Editor: write
bool audioBufferSizeData::Editor::write(yarp::os::ConnectionWriter& connection) const
{
    if (!isValid()) {
        return false;
    }
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(dirty_count + 1)) {
        return false;
    }
    if (!writer.writeString("patch")) {
        return false;
    }
    if (is_dirty_m_samples) {
        if (!writer.writeListHeader(3)) {
            return false;
        }
        if (!writer.writeString("set")) {
            return false;
        }
        if (!writer.writeString("m_samples")) {
            return false;
        }
        if (!obj->nested_write_m_samples(writer)) {
            return false;
        }
    }
    if (is_dirty_m_channels) {
        if (!writer.writeListHeader(3)) {
            return false;
        }
        if (!writer.writeString("set")) {
            return false;
        }
        if (!writer.writeString("m_channels")) {
            return false;
        }
        if (!obj->nested_write_m_channels(writer)) {
            return false;
        }
    }
    if (is_dirty_m_depth) {
        if (!writer.writeListHeader(3)) {
            return false;
        }
        if (!writer.writeString("set")) {
            return false;
        }
        if (!writer.writeString("m_depth")) {
            return false;
        }
        if (!obj->nested_write_m_depth(writer)) {
            return false;
        }
    }
    if (is_dirty_size) {
        if (!writer.writeListHeader(3)) {
            return false;
        }
        if (!writer.writeString("set")) {
            return false;
        }
        if (!writer.writeString("size")) {
            return false;
        }
        if (!obj->nested_write_size(writer)) {
            return false;
        }
    }
    return !writer.isError();
}

// Editor: send if possible
void audioBufferSizeData::Editor::communicate()
{
    if (group != 0) {
        return;
    }
    if (yarp().canWrite()) {
        yarp().write(*this);
        clean();
    }
}

// Editor: mark dirty overall
void audioBufferSizeData::Editor::mark_dirty()
{
    is_dirty = true;
}

// Editor: m_samples mark_dirty
void audioBufferSizeData::Editor::mark_dirty_m_samples()
{
    if (is_dirty_m_samples) {
        return;
    }
    dirty_count++;
    is_dirty_m_samples = true;
    mark_dirty();
}

// Editor: m_channels mark_dirty
void audioBufferSizeData::Editor::mark_dirty_m_channels()
{
    if (is_dirty_m_channels) {
        return;
    }
    dirty_count++;
    is_dirty_m_channels = true;
    mark_dirty();
}

// Editor: m_depth mark_dirty
void audioBufferSizeData::Editor::mark_dirty_m_depth()
{
    if (is_dirty_m_depth) {
        return;
    }
    dirty_count++;
    is_dirty_m_depth = true;
    mark_dirty();
}

// Editor: size mark_dirty
void audioBufferSizeData::Editor::mark_dirty_size()
{
    if (is_dirty_size) {
        return;
    }
    dirty_count++;
    is_dirty_size = true;
    mark_dirty();
}

// Editor: dirty_flags
void audioBufferSizeData::Editor::dirty_flags(bool flag)
{
    is_dirty = flag;
    is_dirty_m_samples = flag;
    is_dirty_m_channels = flag;
    is_dirty_m_depth = flag;
    is_dirty_size = flag;
    dirty_count = flag ? 4 : 0;
}

// read m_samples field
bool audioBufferSizeData::read_m_samples(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(m_samples)) {
        reader.fail();
        return false;
    }
    return true;
}

// write m_samples field
bool audioBufferSizeData::write_m_samples(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(m_samples)) {
        return false;
    }
    return true;
}

// read (nested) m_samples field
bool audioBufferSizeData::nested_read_m_samples(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(m_samples)) {
        reader.fail();
        return false;
    }
    return true;
}

// write (nested) m_samples field
bool audioBufferSizeData::nested_write_m_samples(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(m_samples)) {
        return false;
    }
    return true;
}

// read m_channels field
bool audioBufferSizeData::read_m_channels(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(m_channels)) {
        reader.fail();
        return false;
    }
    return true;
}

// write m_channels field
bool audioBufferSizeData::write_m_channels(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(m_channels)) {
        return false;
    }
    return true;
}

// read (nested) m_channels field
bool audioBufferSizeData::nested_read_m_channels(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(m_channels)) {
        reader.fail();
        return false;
    }
    return true;
}

// write (nested) m_channels field
bool audioBufferSizeData::nested_write_m_channels(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(m_channels)) {
        return false;
    }
    return true;
}

// read m_depth field
bool audioBufferSizeData::read_m_depth(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(m_depth)) {
        reader.fail();
        return false;
    }
    return true;
}

// write m_depth field
bool audioBufferSizeData::write_m_depth(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(m_depth)) {
        return false;
    }
    return true;
}

// read (nested) m_depth field
bool audioBufferSizeData::nested_read_m_depth(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(m_depth)) {
        reader.fail();
        return false;
    }
    return true;
}

// write (nested) m_depth field
bool audioBufferSizeData::nested_write_m_depth(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(m_depth)) {
        return false;
    }
    return true;
}

// read size field
bool audioBufferSizeData::read_size(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(size)) {
        reader.fail();
        return false;
    }
    return true;
}

// write size field
bool audioBufferSizeData::write_size(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(size)) {
        return false;
    }
    return true;
}

// read (nested) size field
bool audioBufferSizeData::nested_read_size(yarp::os::idl::WireReader& reader)
{
    if (reader.noMore()) {
        reader.fail();
        return false;
    }
    if (!reader.readI32(size)) {
        reader.fail();
        return false;
    }
    return true;
}

// write (nested) size field
bool audioBufferSizeData::nested_write_size(const yarp::os::idl::WireWriter& writer) const
{
    if (!writer.writeI32(size)) {
        return false;
    }
    return true;
}

} // namespace yarp::dev
