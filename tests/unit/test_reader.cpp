/// GPLv3 LICENSE, Copyright (©) 2025, Maksim Shchavelev <maksimshchavelev@gmail.com>
/// See LICENSE for details

#include <gtest/gtest.h>
#include "parser/parser.hpp"
#include "reader/reader.hpp"

using namespace optoctreeparser;


TEST(Reader, read_real_optoctree) {
    OptocTreeView batch = Reader::optoctreeview_from_file("resources/read_real_optoctree.optoctree");

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


TEST(Reader, read_real_subnautica_optoctree) {
    OptocTreeView batch = Reader::optoctreeview_from_file("resources/read_real_subnautica_optoctree.optoctrees");
    OptocRoot parsed = Parser::parse_optoctree_batch(batch);

    ASSERT_EQ(parsed.version, 4);
    ASSERT_EQ(parsed.trees.size(), 125);

    for (const auto& tree : parsed.trees) {
        ASSERT_EQ(tree.node_count, 1);
    }
}

