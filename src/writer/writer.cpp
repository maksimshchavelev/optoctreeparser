/**
 * @brief Writer to write optoctree to file
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#include "writer/writer.hpp"
#include <fstream>

namespace optoctreeparser {

// Public static method
void Writer::optoctreeview_to_file(const std::string_view path,
                                   const OptocTreeView&   optoctreeview) {
    std::ofstream out(path.data(), std::ios::binary | std::ios::trunc);

    if (!out.is_open()) {
        throw std::system_error(
            errno, std::generic_category(), std::format("Can't open '{}'", path));
    }

    out.write(reinterpret_cast<const char*>(optoctreeview.data()),
              static_cast<std::streamsize>(optoctreeview.size()));
    out.close();
}

} // namespace optoctreeparser
