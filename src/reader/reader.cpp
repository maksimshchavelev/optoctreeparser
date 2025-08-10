/**
 * @brief Reader to read optoctree from file
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#include "reader/reader.hpp"
#include <fstream>

namespace optoctreeparser {

// Public static method
OptocTreeView Reader::optoctreeview_from_file(const std::string_view path) {
    std::ifstream in(path.data(), std::ios::binary);

    if (!in.is_open()) {
        throw std::system_error(
            errno, std::generic_category(), std::format("Can't open '{}'", path));
    }

    OptocTreeView optoctreeview((std::istreambuf_iterator<char>(in)), {});

    in.close();

    return optoctreeview;
}

} // namespace optoctreeparser
