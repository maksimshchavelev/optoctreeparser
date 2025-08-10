/**
 * @brief Basic structures for storing optoctree data
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#pragma once

#include <cstdint>
#include <vector>

namespace optoctreeparser {

using byte = uint8_t;
using OptocTreeView = std::vector<byte>;

struct OptocTree;       // Forward declaration
struct OptocNode;       // Forward declaration
struct OptocRoot;       // Forward declaration
struct OptocPatchRoot;  // Forward declaration
struct OptocPatchBatch; // Forward declaration
struct OptocPatchTree;  // Forward declaration


/**
 * @brief Root of `optoctreepatch` file
 * @see `optoctreepatch` format documentation:
 * https://github.com/Esper89/Subnautica-TerrainPatcher/blob/master/doc/Subnautica%20Terrain%20Patch%20Format.pdf
 */
struct OptocPatchRoot {
    int32_t                      version; ///< version of optoctreepatch file
    std::vector<OptocPatchBatch> batches; ///< Patched batches

    bool operator==(const OptocPatchRoot& other) const = default;
};


/**
 * @brief Patched batch of `optoctreepatch` file
 * @see `optoctreepatch` format documentation:
 * https://github.com/Esper89/Subnautica-TerrainPatcher/blob/master/doc/Subnautica%20Terrain%20Patch%20Format.pdf
 */
struct OptocPatchBatch {
    int16_t                     x_position;   ///< X position of batch
    int16_t                     y_position;   ///< Y position of batch
    int16_t                     z_position;   ///< Z position of batch
    byte                        octree_count; ///< Count of octrees
    std::vector<OptocPatchTree> octrees;      ///< Octrees

    bool operator==(const OptocPatchBatch& other) const = default;
};


/**
 * @brief Patched tree of `optoctreepatch` file
 * @see `optoctreepatch` format documentation:
 * https://github.com/Esper89/Subnautica-TerrainPatcher/blob/master/doc/Subnautica%20Terrain%20Patch%20Format.pdf
 */
struct OptocPatchTree {
    byte                   octree_number; ///< Number of octree
    uint16_t               node_count;    ///< Count of nodes
    std::vector<OptocNode> nodes;         ///< Nodes

    bool operator==(const OptocPatchTree& other) const = default;
};


/**
 * @brief Root of `optoctree` file
 */
struct OptocRoot {
    int32_t                version; ///< version of optoctree file
    std::vector<OptocTree> trees;   ///< Optoctrees

    bool operator==(const OptocRoot& other) const = default;
};


/**
 * @brief One Optocnode. The smallest unit of information in an Optoctree
 */
struct OptocNode {
    byte material_type;   ///< Type of material. For example, 0 - empty, 37 - sand
    byte signed_distance; ///< Signed distance to the isosurface. Values 1–125 = above surface,
                          ///< 127–252 = below, 0 = completely solid, if material ≠ 0
    uint16_t first_child_node; ///< Index of first child node. If it = 0 → it is a sheet; otherwise
                               ///< — starting from this index, there are 8 children

    bool operator==(const OptocNode& other) const = default;
};


/**
 * @brief One OctoTree
 */
struct OptocTree {
    uint16_t               node_count; ///< Count of nodes
    std::vector<OptocNode> nodes;      ///< Nodes

    bool operator==(const OptocTree& other) const = default;
};

} // namespace optoctreeparser
