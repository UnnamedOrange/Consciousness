/**
 * @file record.h
 * @author UnnamedOrange
 * @brief Record type and JSON converter.
 * Record type consists of two parts:
 * - User specified data (record_t), which should be stored into file.
 * - Runtime data (record_ex_t - record_t), which are generated on runtime.
 * Record type does not include the statuses.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <cstdint>
#include <filesystem>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <QJsonArray>
#include <QJsonObject>

namespace consciousness
{
    /**
     * @brief Record information that should be stored into file.
     */
    struct record_t
    {
        static constexpr std::u16string_view keyname_entry = u"records";

        static constexpr std::u16string_view keyname_window_name =
            u"window_name";
        std::u16string window_name;
        static constexpr std::u16string_view keyname_window_class_name =
            u"window_class_name";
        std::u16string window_class_name;
        static constexpr std::u16string_view keyname_process_name =
            u"process_name";
        std::u16string process_name;

        QJsonObject to_json() const;
        static record_t from_json(const QJsonObject& json);
    };

    /**
     * @brief Record information with runtime data.
     */
    struct record_ex_t : public record_t
    {
    private:
        using hash_value_t = std::uint32_t;
        inline static std::default_random_engine random_engine;
        inline static std::uniform_int_distribution<hash_value_t>
            random_distribution;

    public:
        /**
         * @brief Runtime id to identify a record.
         */
        hash_value_t id{random_distribution(random_engine)};

    public:
        record_ex_t() = default;
        record_ex_t(const record_t& record) : record_t(record) {}
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
