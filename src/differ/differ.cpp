/**
 * @brief Differ to find difference between two OptocRoot
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#include "differ/differ.hpp"
#include <algorithm>

namespace optoctreeparser {

// Static public method
std::vector<OptocPatchTree> Differ::find_difference(const OptocRoot& old_root,
                                                    const OptocRoot& new_root) {
    std::vector<OptocPatchTree> patches;

    std::size_t max_trees = std::max(old_root.trees.size(), new_root.trees.size());

    // Iterate over trees
    for (std::size_t tree = 0; tree < max_trees; ++tree) {
        bool exists_in_new = tree < new_root.trees.size();
        bool exists_in_old = tree < old_root.trees.size();

        // Added tree
        if (exists_in_new && !exists_in_old) {
            patches.push_back({static_cast<byte>(tree),
                               new_root.trees[tree].node_count,
                               new_root.trees[tree].nodes});
            continue;
        }

        // Removed tree
        if (!exists_in_new && exists_in_old) {
            continue;
        }

        // Chanded tree
        if (exists_in_new && exists_in_old &&
            !trees_equal(new_root.trees[tree], old_root.trees[tree])) {
            patches.push_back({static_cast<byte>(tree),
                               new_root.trees[tree].node_count,
                               new_root.trees[tree].nodes});
        }
    }

    return patches;
}



// Static private method
bool Differ::trees_equal(const OptocTree& a, const OptocTree& b) {
    if (a.node_count != b.node_count)
        return false;
    if (a.nodes.size() != b.nodes.size())
        return false;
    for (size_t i = 0; i < a.nodes.size(); ++i) {
        if (a.nodes[i].material_type != b.nodes[i].material_type ||
            a.nodes[i].signed_distance != b.nodes[i].signed_distance ||
            a.nodes[i].first_child_node != b.nodes[i].first_child_node) {
            return false;
        }
    }
    return true;
}

} // namespace optoctreeparser
