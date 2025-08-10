/**
 * @brief Writer to write optoctree to file
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#pragma once

#include "base_struct/base_struct.hpp"
#include <string_view>

namespace optoctreeparser {

/**
 * @brief Reads optoctree files
 */
class Writer {
  public:
    /**
     * @brief Writes optoctree file
     * @param path Path to file
     * @param optoctreeview `OptocTreeView` binary representation of optoctree
     * @return `OptocTreeView`
     * @note Creates a new file if the file at `path` does not exist. If it exists but is
     * overwritten
     *
     * @throws `std::system_error` when:
     * - .optoctree file opening error
     */
    static void optoctreeview_to_file(const std::string_view path,
                                      const OptocTreeView&   optoctreeview);
};

} // namespace optoctreeparser
