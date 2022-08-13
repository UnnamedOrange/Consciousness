/**
 * @file config_store.h
 * @author UnnamedOrange
 * @brief Class for storing configs.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "record.h"

#include <filesystem>
#include <vector>

namespace consciousness
{
    class config_store : public std::vector<record_ex_t>
    {
    public:
        void from_file(const std::filesystem::path& path = "records.json");
        void to_file(const std::filesystem::path& path = "records.json") const;
    };
} // namespace consciousness
