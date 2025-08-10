/**
 * @brief Differ to find difference between two OptocRoot
 *
 * @copyright maksimshchavelev © 2025 maksimshchavelev@gmail.com
 * @license MIT
 */

#pragma once

#include "base_struct/base_struct.hpp"

namespace optoctreeparser {

/**
 * @brief  Differ to find difference between two OptocRoot
 */
class Differ {
  public:
    /**
     * @brief Finds the difference between two `OptocRoot`     *
     * @param old_root The old "base" batch. It is compared with `new_root`.
     * @param new_root New batch
     * @return `std::vector` with octrees (`OptocPatchTree`) from `new_root` that are either absent
     * from `old_root` or have changed nodes
     *
     * @note This method is useful for creating `OptocPatchRoot`. For example, you have a base game
     * patch - `old_root`, you made some changes and ended up with `new_root`, then you can call
     * this method, which will return the octrees that have been changed. Then you can insert these
     * octrees into the desired patch in `OptocPatchRoot`.
     */
    static std::vector<OptocPatchTree> find_difference(const OptocRoot& old_root,
                                                       const OptocRoot& new_root);

  private:
    /**
     * @brief Compares two trees
     * @param a First `OptocTree`
     * @param b Second `OptocTree`
     * @return `true` if equal
     */
    static bool trees_equal(const OptocTree& a, const OptocTree& b);
};

} // namespace optoctreeparser
