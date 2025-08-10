/// GPLv3 LICENSE, Copyright (©) 2025, Maksim Shchavelev <maksimshchavelev@gmail.com>
/// See LICENSE for details

#include <gtest/gtest.h>
#include "writer/writer.hpp"
#include "reader/reader.hpp"

using namespace optoctreeparser;

TEST(Reader, write_real_optoctree) {
    OptocTreeView batch = {
        // ---- Version (int32 LE = 4) ----
        0x04, 0x00, 0x00, 0x00,

        // ---- node_count (uint16 LE = 2) ----
        0x02, 0x00,

        // ---- Node 0 ----
        0x25,       // material_type = 37
        0x80,       // signed_distance = 128
        0x02, 0x00, // first_child_node = 2

        // ---- Node 1 ----
        0x00,       // material_type = 0
        0x7E,       // signed_distance = 126
        0x00, 0x00  // first_child_node = 0
    };

    batch.resize(batch.size() + (124 * 2), 0x00); // Fill empty trees

    Writer::optoctreeview_to_file("resources/write_real_optoctree.optoctree", batch);
    OptocTreeView readed = Reader::optoctreeview_from_file("resources/write_real_optoctree.optoctree");
    ASSERT_EQ(readed, batch);
}

