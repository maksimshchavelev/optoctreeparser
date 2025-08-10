/**
 * @brief Parser to parse optoctree
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#pragma once

#include "base_struct/base_struct.hpp"
#include <span>

namespace optoctreeparser {

/**
 * @brief Parser to parse optoctree
 *
 * @section example_usage Example usage
 *
 * Let's assume that the optoctree bytes are as follows:
 *
 * ```
 * 0x04 0x00 0x00 0x00
 * 0x02 0x00 0x25 0x80
 * 0x02 0x00 0x00 0x7E
 * 0x00 0x00 ... (0x0 2*124 times to fill trees)
 * ```
 *
 * Explanation:
 *
 * ```
 * // ---- Version (int32 Little endian = 4) ----
 * 0x04 0x00 0x00 0x00
 *
 * // ---- Count of nodes (uint16 Little endian = 2) ----
 * 0x02, 0x00,
 *
 * // ---- Node 0 ----
 * 0x25,       // material type = 37
 * 0x80,       // signed distance = 128
 * 0x02, 0x00, // first child_node = 2
 *
 * // ---- Node 1 ----
 * 0x00,       // material type = 0
 * 0x7E,       // signed distance = 126
 * 0x00, 0x00  // first child_node = 0
 * ```
 *
 * Using this code:
 * @code{.cpp}
 * OptocRoot batch = Parser::parse(Your OptocTreeView here);
 * @endcode
 *
 * You will get the following batch structure:
 * ```
 * OptocRoot
 * └── version = 4
 *     trees[0]
 *     └── node_count = 2
 *         nodes[0] = { material_type=37, signed_distance=128, first_child_node=2 }
 *         nodes[1] = { material_type=0, signed_distance=126, first_child_node=0 }
 * ```
 *
 * @note `OptocTreeView` is just `std::vector<byte>`. This is necessary for representing OptocTree
 * bytes. Use `Reader::optoctreeview_from_file` to read bytes from file
 */
class Parser {
  public:
    /**
     * @brief Parses optoctree from its binary representation
     * @param optoctree Byte representation of optoctree
     * @return Parsed `OptocRoot`
     * @see `OptocTreeView`
     */
    static OptocRoot parse_optoctree_batch(const OptocTreeView& optoctree);

    /**
     * @brief Packs `OptocRoot` into binary representation
     * @param batch `OptocBatch`
     * @return `OptocTreeView` with binary representation
     */
    static OptocTreeView pack_optoctree_batch(const OptocRoot& batch);

    /**
     * @brief Parses optoctreepatch from its binary representation
     * @param optoctree Byte representation of optoctreepatch
     * @return Parsed `OptocPatchRoot`
     * @see `OptocTreeView`
     */
    static OptocPatchRoot parse_optoctreepatch(const OptocTreeView& optoctree);

    /**
     * @brief Packs `OptocPatchRoot` into binary representation
     * @param patch `OptocPatchRoot`
     * @return `OptocTreeView` with binary representation
     */
    static OptocTreeView pack_optoctreepatch(const OptocPatchRoot& patch);

  private:
    /**
     * @brief Reads uint16_t in **little endian** (used in optoctree) from the buffer at the
     * specified offset
     * @param buffer Buffer with data
     * @param offset Offset
     * @return `uint16_t`
     */
    static uint16_t read_u16_le(std::span<const byte> buffer, size_t offset);

    /**
     * @brief Reads int16_t in **little endian** (used in optoctreepatch) from the buffer at the
     * specified offset
     * @param buffer Buffer with data
     * @param offset Offset
     * @return `int16_t`
     */
    static int16_t read_i16_le(std::span<const byte> buffer, size_t offset);

    /**
     * @brief Reads int32_t in **little endian** (used in optoctree) from the buffer at the
     * specified offset
     * @param buffer Buffer with data
     * @param offset Offset
     * @return `uint16_t`
     */
    static int32_t read_i32_le(std::span<const byte> buffer, size_t offset);

    /**
     * @brief Writes uint16_t in **little endian** (used in optoctree) from the buffer at the
     * specified offset
     * @param buffer Buffer with data
     * @param offset Offset
     * @param value Value for writing
     */
    static void write_u16_le(std::span<byte> buffer, size_t offset, uint16_t value);

    /**
     * @brief Writes int32_t in **little endian** (used in optoctree) from the buffer at the
     * specified offset
     * @param buffer Buffer with data
     * @param offset Offset
     * @param value Value for writing
     */
    static void write_i32_le(std::span<byte> buffer, size_t offset, int32_t value);

    /**
     * @brief Writes int16_t in **little endian** (used in optoctreepatch) to the buffer at the
     * specified offset
     * @param buffer Buffer with data
     * @param offset Offset
     * @param value Value to write
     */
    static void write_i16_le(std::span<byte> buffer, size_t offset, int16_t value);

    /**
     * @brief Reads node from the buffer at the specified offset
     * @param buffer Buffer with data
     * @param offset offset
     * @return `OptocNode`
     */
    static OptocNode read_node(std::span<const byte> buffer, size_t offset);

    /**
     * @brief Writes node from the buffer at the specified offset
     * @param buffer Buffer with data
     * @param offset offset
     * @param node `OptocNode` node
     */
    static void write_node(std::span<byte> buffer, size_t offset, const OptocNode& node);
};

} // namespace optoctreeparser
