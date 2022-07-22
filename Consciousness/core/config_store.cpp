/**
 * @file config_store.cpp
 * @author UnnamedOrange
 * @brief Class for storing configs.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "config_store.h"

using namespace consciousness;

void config_store::from_file(const std::filesystem::path& path)
{
    auto records = record_converter::from_file(path);
    static_cast<std::vector<record_t>&>(*this) = records;
}
void config_store::to_file(const std::filesystem::path& path) const
{
    record_converter::to_file(static_cast<const std::vector<record_t>&>(*this),
                              path);
}
