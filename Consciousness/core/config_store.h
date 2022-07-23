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
    class config_store : protected std::vector<record_t>
    {
    public:
        using vector::begin;
        using vector::cbegin;
        using vector::cend;
        using vector::clear;
        using vector::emplace_back;
        using vector::end;
        using vector::push_back;
        using vector::size;
        using vector::operator[];

    public:
        void from_file(const std::filesystem::path& path = "records.json");
        void to_file(const std::filesystem::path& path = "records.json") const;
    };
} // namespace consciousness
