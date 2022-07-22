/**
 * @file record.h
 * @author UnnamedOrange
 * @brief Record type and JSON converter.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <QJsonArray>
#include <QJsonObject>

namespace consciousness
{
    struct record_t
    {
        static constexpr std::u16string_view keyname_entry = u"records";

        static constexpr std::u16string_view keyname_window_name =
            u"window_name";
        std::u16string window_name;
        static constexpr std::u16string_view keyname_window_class_name =
            u"window_class_name";
        std::u16string window_class_name;
    };

    class record_converter
    {
    public:
        static QJsonObject to_json(std::span<const record_t> records);
        static void to_file(std::span<const record_t> records,
                            const std::filesystem::path& path = "records.json");

    public:
        static std::vector<record_t> from_json(const QJsonObject& json);
        static std::vector<record_t> from_file(
            const std::filesystem::path& path = "records.json");
    };
} // namespace consciousness
