/**
 * @brief Parser to parse_optoctree_batch optoctree
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#include "parser/parser.hpp"
#include <cstddef>

namespace optoctreeparser {

// Static public method
OptocRoot Parser::parse_optoctree_batch(const OptocTreeView& optoctree) {
    std::span<const byte> span(optoctree);

    OptocRoot batch{};

    // Read version
    batch.version = read_i32_le(span, 0);

    // Iterates over optoctrees
    size_t offset = 4;
    batch.trees.reserve(125);
    for (std::size_t i = 0; i < 125; ++i) {
        OptocTree tree{};

        tree.node_count = read_u16_le(span, offset);
        tree.nodes.reserve(tree.node_count);
        offset += 2; // count is 2 bytes

        for (std::size_t node_index = 0; node_index < tree.node_count; ++node_index) {
            tree.nodes.push_back(read_node(span, offset));
            offset += 4; // node size is 4
        }

        batch.trees.push_back(std::move(tree));
    }

    return batch;
}




// Static public method
OptocTreeView Parser::pack_optoctree_batch(const OptocRoot& batch) {
    OptocTreeView optoctreeview;

    // Counting size of batch
    std::size_t size_of_batch{0};
    size_of_batch += 4; // for version

    for (const auto& tree : batch.trees) {
        size_of_batch += sizeof(tree.node_count) + tree.nodes.size() * 4;
    }

    // Reserving
    optoctreeview.resize(size_of_batch, 0x0);

    // Packing
    size_t          offset = 0;
    std::span<byte> buffer(optoctreeview);
    write_i32_le(buffer, offset, batch.version);
    offset += 4; // version is 4 bytes

    // Iterating over trees
    for (const auto& tree : batch.trees) {
        write_u16_le(buffer, offset, tree.node_count); // write node count
        offset += 2;

        // Write nodes
        for (const auto& node : tree.nodes) {
            write_node(buffer, offset, node);
            offset += 4; // node is 4 bytes
        }
    }

    return optoctreeview;
}




// Static public method
OptocPatchRoot Parser::parse_optoctreepatch(const OptocTreeView& optoctree) {
    std::span<const byte> span(optoctree);

    OptocPatchRoot root{};

    // Read version
    root.version = read_i32_le(span, 0);

    // Iterates over batches
    for (std::size_t offset = 4; offset < span.size();) {
        OptocPatchBatch batch{};

        batch.x_position = read_i16_le(span, offset);
        offset += 2; // Position is 2 bytes

        batch.y_position = read_i16_le(span, offset);
        offset += 2; // Position is 2 bytes

        batch.z_position = read_i16_le(span, offset);
        offset += 2; // Position is 2 bytes

        batch.octree_count = span[offset];
        ++offset; // Octree count is 1 byte

        // Iterate over octrees
        for (std::size_t i = 0; i < batch.octree_count; ++i) {
            OptocPatchTree tree{};

            tree.octree_number = span[offset];
            ++offset; // Octree number is 1 byte

            tree.node_count = read_u16_le(span, offset);
            offset += 2; // Node count is 2 bytes

            tree.nodes.push_back(read_node(span, offset));
            offset += 4; // Node is 4 bytes

            batch.octrees.push_back(std::move(tree));
        }

        root.batches.push_back(std::move(batch));
    }

    return root;
}




// Static public method
OptocTreeView Parser::pack_optoctreepatch(const OptocPatchRoot& patch) {
    OptocTreeView optoctreeview;

    // Counting size of batch
    std::size_t size_of_patch{0};
    size_of_patch += 4; // for version

    // Iterate over batches
    for (const auto& batch : patch.batches) {
        size_of_patch += sizeof(batch.x_position);
        size_of_patch += sizeof(batch.y_position);
        size_of_patch += sizeof(batch.z_position);
        size_of_patch += sizeof(batch.octree_count);

        // Iterate over octrees
        for (const auto& octree : batch.octrees) {
            size_of_patch += sizeof(octree.octree_number);
            size_of_patch += sizeof(octree.node_count);
            size_of_patch += octree.nodes.size() * 4; // Node is 4 bytes
        }
    }

    // Resize and fill 0x00
    optoctreeview.resize(size_of_patch, 0x00);

    // Create span
    std::span<byte> buffer(optoctreeview);

    // Offset is 0 for beginning
    std::size_t offset{0};

    // Write version
    write_i32_le(buffer, offset, patch.version);
    offset += 4; // Version is 4 bytes

    // Iterate over batches
    for (const auto& batch : patch.batches) {
        write_i16_le(buffer, offset, batch.x_position);
        offset += 2; // Position is 2 bytes

        write_i16_le(buffer, offset, batch.y_position);
        offset += 2; // Position is 2 bytes

        write_i16_le(buffer, offset, batch.z_position);
        offset += 2; // Position is 2 bytes

        buffer[offset] = batch.octree_count;
        offset += 1; // Octree count is 1 byte

        // Iterate over octrees
        for (const auto& octree : batch.octrees) {
            buffer[offset] = octree.octree_number;
            offset += 1; // Octree number is 1 byte

            write_u16_le(buffer, offset, octree.node_count);
            offset += 2; // Node is 2 bytes

            // Iterate over nodes
            for (const auto& node : octree.nodes) {
                write_node(buffer, offset, node);
                offset += 4; // Node is 4 bytes
            }
        }
    }

    return optoctreeview;
}




// Static private method
uint16_t Parser::read_u16_le(std::span<const byte> buffer, size_t offset) {
    uint16_t result = static_cast<uint16_t>(buffer[offset] | (buffer[offset + 1] << 8));
    return result;
}




// Static private method
int16_t Parser::read_i16_le(std::span<const byte> buffer, size_t offset) {
    int16_t result = static_cast<int16_t>(buffer[offset] | (buffer[offset + 1] << 8));
    return result;
}




// Static private method
int32_t Parser::read_i32_le(std::span<const byte> buffer, size_t offset) {
    int32_t result = static_cast<int32_t>(buffer[offset]) | (buffer[offset + 1] << 8) |
                     (buffer[offset + 2] << 16) | (buffer[offset + 3] << 24);
    return result;
}




// Static private method
void Parser::write_i32_le(std::span<byte> buffer, size_t offset, int32_t value) {
    buffer[offset] = static_cast<byte>(value & 0xFF);
    buffer[offset + 1] = static_cast<byte>((value >> 8) & 0xFF);
    buffer[offset + 2] = static_cast<byte>((value >> 16) & 0xFF);
    buffer[offset + 3] = static_cast<byte>((value >> 24) & 0xFF);
}




// Static private method
void Parser::write_i16_le(std::span<byte> buffer, size_t offset, int16_t value) {
    buffer[offset] = static_cast<byte>(value & 0xFF);
    buffer[offset + 1] = static_cast<byte>((value >> 8) & 0xFF);
}




// Static private method
void Parser::write_u16_le(std::span<byte> buffer, size_t offset, uint16_t value) {
    buffer[offset] = static_cast<byte>(value & 0xFF);
    buffer[offset + 1] = static_cast<byte>((value >> 8) & 0xFF);
}




// Static private method
OptocNode Parser::read_node(std::span<const byte> buffer, size_t offset) {
    OptocNode node{};
    node.material_type = buffer[offset];
    node.signed_distance = buffer[offset + 1];
    node.first_child_node = read_u16_le(buffer, offset + 2);
    return node;
}




// Static private method
void Parser::write_node(std::span<byte> buffer, size_t offset, const OptocNode& node) {
    buffer[offset] = node.material_type;
    buffer[offset + 1] = node.signed_distance;
    write_u16_le(buffer, offset + 2, node.first_child_node);
}



} // namespace optoctreeparser
