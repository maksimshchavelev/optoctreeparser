/**
 * @brief Reader to read optoctree from file
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
class Reader {
public:
    /**
     * @brief Reads optoctree file
     * @param path Path to .optoctree
     * @return `OptocTreeView`
     *
     * @throws `std::system_error` when:
     * - .optoctree file opening error
     */
    static OptocTreeView optoctreeview_from_file(const std::string_view path);
};

}
