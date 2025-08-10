/// GPLv3 LICENSE, Copyright (©) 2025, Maksim Shchavelev <maksimshchavelev@gmail.com>
/// See LICENSE for details

#include "parser/parser.hpp"
#include <gtest/gtest.h>

using namespace optoctreeparser;

TEST(Parser, parse_1_tree_2_nodes) {
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

    ASSERT_EQ(parsed.version, 4);
    ASSERT_EQ(parsed.trees.size(), 125);
    ASSERT_EQ(parsed.trees[0].node_count, 2);
    // Node 0
    ASSERT_EQ(parsed.trees[0].nodes[0].material_type, 37);
    ASSERT_EQ(parsed.trees[0].nodes[0].signed_distance, 128);
    ASSERT_EQ(parsed.trees[0].nodes[0].first_child_node, 2);
    // Node 0
    ASSERT_EQ(parsed.trees[0].nodes[1].material_type, 0);
    ASSERT_EQ(parsed.trees[0].nodes[1].signed_distance, 126);
    ASSERT_EQ(parsed.trees[0].nodes[1].first_child_node, 0);
}



TEST(Parser, parse_2_trees) {
    OptocTreeView batch = {
        // ---- Version (int32 LE = 4) ----
        0x04,
        0x00,
        0x00,
        0x00,

        // ---- Tree 0 ----
        // node_count (uint16 LE = 2)
        0x02,
        0x00,
        // Node 0
        0x25, // material_type = 37
        0x80, // signed_distance = 128
        0x02,
        0x00, // first_child_node = 2
        // Node 1
        0x00, // material_type = 0
        0x7E, // signed_distance = 126
        0x00,
        0x00, // first_child_node = 0

        // ---- Tree 1 ----
        // node_count (uint16 LE = 3)
        0x03,
        0x00,
        // Node 0
        0x10, // material_type = 16
        0x40, // signed_distance = 64
        0x01,
        0x00, // first_child_node = 1
        // Node 1
        0x11, // material_type = 17
        0x41, // signed_distance = 65
        0x02,
        0x00, // first_child_node = 2
        // Node 2
        0x12, // material_type = 18
        0x42, // signed_distance = 66
        0x00,
        0x00, // first_child_node = 0
    };

    // Fill the rest (125 - 2) empty trees
    batch.resize(batch.size() + ((125 - 2) * 2), 0x00);

    OptocRoot parsed = Parser::parse_optoctree_batch(batch);

    ASSERT_EQ(parsed.version, 4);
    ASSERT_EQ(parsed.trees.size(), 125);

    // Tree 0
    ASSERT_EQ(parsed.trees[0].node_count, 2);
    ASSERT_EQ(parsed.trees[0].nodes[0].material_type, 37);
    ASSERT_EQ(parsed.trees[0].nodes[0].signed_distance, 128);
    ASSERT_EQ(parsed.trees[0].nodes[0].first_child_node, 2);
    ASSERT_EQ(parsed.trees[0].nodes[1].material_type, 0);
    ASSERT_EQ(parsed.trees[0].nodes[1].signed_distance, 126);
    ASSERT_EQ(parsed.trees[0].nodes[1].first_child_node, 0);

    // Tree 1
    ASSERT_EQ(parsed.trees[1].node_count, 3);
    ASSERT_EQ(parsed.trees[1].nodes[0].material_type, 16);
    ASSERT_EQ(parsed.trees[1].nodes[0].signed_distance, 64);
    ASSERT_EQ(parsed.trees[1].nodes[0].first_child_node, 1);
    ASSERT_EQ(parsed.trees[1].nodes[1].material_type, 17);
    ASSERT_EQ(parsed.trees[1].nodes[1].signed_distance, 65);
    ASSERT_EQ(parsed.trees[1].nodes[1].first_child_node, 2);
    ASSERT_EQ(parsed.trees[1].nodes[2].material_type, 18);
    ASSERT_EQ(parsed.trees[1].nodes[2].signed_distance, 66);
    ASSERT_EQ(parsed.trees[1].nodes[2].first_child_node, 0);
}



TEST(Parser, pack_1_tree_2_nodes) {
    OptocTreeView source_batch = {
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

    source_batch.resize(source_batch.size() + (124 * 2), 0x00); // Fill empty trees

    OptocRoot parsed = Parser::parse_optoctree_batch(source_batch);

    OptocTreeView packed = Parser::pack_optoctree_batch(parsed);

    ASSERT_EQ(source_batch, packed);
    ASSERT_EQ(source_batch.size(), packed.size());
}



TEST(Parser, parse_optoctreepatch) {
    // See
    // https://github.com/Esper89/Subnautica-TerrainPatcher/blob/master/doc/Subnautica%20Terrain%20Patch%20Format.pdf
    OptocTreeView raw = {
        0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x12, 0x00, 0x0C, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7C, 0x01, 0x00, 0x23, 0x00, 0x00,
        0x00, 0xFE, 0xFF, 0x13, 0x00, 0xFC, 0xFF, 0x02, 0x23, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x4C, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x02, 0x64,
        0x01, 0x00, 0x23, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x23, 0x00, 0x00, 0x00};

    OptocPatchRoot root = Parser::parse_optoctreepatch(raw);

    ASSERT_EQ(root.version, 0);
    // Batch 0
    ASSERT_EQ(root.batches[0].x_position, 12);
    ASSERT_EQ(root.batches[0].y_position, 18);
    ASSERT_EQ(root.batches[0].z_position, 12);
    ASSERT_EQ(root.batches[0].octree_count, 3);
    ASSERT_EQ(root.batches[0].octrees[0].node_count, 1);
    ASSERT_EQ(root.batches[0].octrees[0].octree_number, 0);
    ASSERT_EQ(root.batches[0].octrees[1].node_count, 1);
    ASSERT_EQ(root.batches[0].octrees[1].octree_number, 1);
    ASSERT_EQ(root.batches[0].octrees[2].node_count, 1);
    ASSERT_EQ(root.batches[0].octrees[2].octree_number, 2);

    for (std::size_t node = 0; node < 1; ++node) {
        ASSERT_EQ(root.batches[0].octrees[0].nodes[node].first_child_node, 0);
        ASSERT_EQ(root.batches[0].octrees[0].nodes[node].material_type, 0);
        ASSERT_EQ(root.batches[0].octrees[0].nodes[node].signed_distance, 0);
    }

    // Batch 1
    ASSERT_EQ(root.batches[1].x_position, 0);
    ASSERT_EQ(root.batches[1].y_position, 0);
    ASSERT_EQ(root.batches[1].z_position, 0);
    ASSERT_EQ(root.batches[1].octree_count, 1);
    ASSERT_EQ(root.batches[1].octrees[0].node_count, 1);
    ASSERT_EQ(root.batches[1].octrees[0].octree_number, 124);

    for (std::size_t node = 0; node < 1; ++node) {
        ASSERT_EQ(root.batches[1].octrees[0].nodes[node].first_child_node, 0);
        ASSERT_EQ(root.batches[1].octrees[0].nodes[node].material_type, 0x23);
        ASSERT_EQ(root.batches[1].octrees[0].nodes[node].signed_distance, 0);
    }

    // Batch 2
    ASSERT_EQ(root.batches[2].x_position, -2);
    ASSERT_EQ(root.batches[2].y_position, 19);
    ASSERT_EQ(root.batches[2].z_position, -4);
    ASSERT_EQ(root.batches[2].octree_count, 2);
    ASSERT_EQ(root.batches[2].octrees[0].node_count, 1);
    ASSERT_EQ(root.batches[2].octrees[0].octree_number, 35);
    ASSERT_EQ(root.batches[2].octrees[1].node_count, 1);
    ASSERT_EQ(root.batches[2].octrees[1].octree_number, 76);

    for (std::size_t node = 0; node < 1; ++node) {
        ASSERT_EQ(root.batches[2].octrees[0].nodes[node].first_child_node, 0);
        ASSERT_EQ(root.batches[2].octrees[0].nodes[node].material_type, 0x01);
        ASSERT_EQ(root.batches[2].octrees[0].nodes[node].signed_distance, 0);
    }

    // Batch 3
    ASSERT_EQ(root.batches[3].x_position, 28);
    ASSERT_EQ(root.batches[3].y_position, 19);
    ASSERT_EQ(root.batches[3].z_position, 28);
    ASSERT_EQ(root.batches[3].octree_count, 2);
    ASSERT_EQ(root.batches[3].octrees[0].node_count, 1);
    ASSERT_EQ(root.batches[3].octrees[0].octree_number, 100);
    ASSERT_EQ(root.batches[3].octrees[1].node_count, 1);
    ASSERT_EQ(root.batches[3].octrees[1].octree_number, 0x03);

    for (std::size_t node = 0; node < 1; ++node) {
        ASSERT_EQ(root.batches[3].octrees[0].nodes[node].first_child_node, 0);
        ASSERT_EQ(root.batches[3].octrees[0].nodes[node].material_type, 0x23);
        ASSERT_EQ(root.batches[3].octrees[0].nodes[node].signed_distance, 0);
    }
}



TEST(Parser, pack_optoctreepatch) {
    // See
    // https://github.com/Esper89/Subnautica-TerrainPatcher/blob/master/doc/Subnautica%20Terrain%20Patch%20Format.pdf
    OptocTreeView raw = {
                         0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x12, 0x00, 0x0C, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7C, 0x01, 0x00, 0x23, 0x00, 0x00,
                         0x00, 0xFE, 0xFF, 0x13, 0x00, 0xFC, 0xFF, 0x02, 0x23, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
                         0x4C, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x02, 0x64,
                         0x01, 0x00, 0x23, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x23, 0x00, 0x00, 0x00};

    OptocPatchRoot root = Parser::parse_optoctreepatch(raw);
    OptocTreeView packed = Parser::pack_optoctreepatch(root);

    ASSERT_EQ(raw.size(), packed.size());
    ASSERT_EQ(raw, packed);
}
