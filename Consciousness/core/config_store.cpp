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
    assign(records.begin(), records.end());
}
void config_store::to_file(const std::filesystem::path& path) const
{
    std::vector<record_t> records(begin(), end());
    record_converter::to_file(records, path);
}
