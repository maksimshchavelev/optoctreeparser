/// GPLv3 LICENSE, Copyright (©) 2025, Maksim Shchavelev <maksimshchavelev@gmail.com>
/// See LICENSE for details

#include "differ/differ.hpp"
#include "parser/parser.hpp"
#include <gtest/gtest.h>

using namespace optoctreeparser;

TEST(Differ, no_difference) {
    OptocTreeView batch = {
        // ---- Version (int32 LE = 4) ----
        0x04,
        0x00,
        0x00,
        0x00,

        // ---- node_count (uint16 LE = 2) ----
        0x02,
        0x00,

        // ---- Node 0 ----
        0x25, // material_type = 37
        0x80, // signed_distance = 128
        0x02,
        0x00, // first_child_node = 2

        // ---- Node 1 ----
        0x00, // material_type = 0
        0x7E, // signed_distance = 126
        0x00,
        0x00 // first_child_node = 0
    };

    batch.resize(batch.size() + (124 * 2), 0x00); // Fill empty trees

    OptocRoot parsed = Parser::parse_optoctree_batch(batch);

    auto difference = Differ::find_difference(parsed, parsed);
    ASSERT_TRUE(difference.empty());
}


TEST(Differ, modified_tree) {
    OptocTreeView batch = {
        // ---- Version (int32 LE = 4) ----
        0x04,
        0x00,
        0x00,
        0x00,

        // ---- node_count (uint16 LE = 2) ----
        0x02,
        0x00,

        // ---- Node 0 ----
        0x25, // material_type = 37
        0x80, // signed_distance = 128
        0x02,
        0x00, // first_child_node = 2

        // ---- Node 1 ----
        0x00, // material_type = 0
        0x7E, // signed_distance = 126
        0x00,
        0x00 // first_child_node = 0
    };
    batch.resize(batch.size() + (124 * 2), 0x00); // Fill empty trees

    OptocRoot parsed = Parser::parse_optoctree_batch(batch);

    OptocRoot patched = parsed;
    patched.trees[2] = OptocTree{
        .node_count = 1,
        .nodes = {OptocNode{.material_type = 23, .signed_distance = 0, .first_child_node = 0}}};

    auto difference = Differ::find_difference(parsed, patched);
    ASSERT_FALSE(difference.empty());
    ASSERT_EQ(difference[0].node_count, 1);
    ASSERT_EQ(difference[0].octree_number, 2);
    ASSERT_EQ(difference[0].nodes[0].first_child_node, 0);
    ASSERT_EQ(difference[0].nodes[0].material_type, 23);
    ASSERT_EQ(difference[0].nodes[0].signed_distance, 0);
}
