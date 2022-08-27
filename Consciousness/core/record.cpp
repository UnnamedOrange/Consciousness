/**
 * @file record.cpp
 * @author UnnamedOrange
 * @brief Record type and JSON converter.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "record.h"

#include <QFile>
#include <QJsonDocument>

using namespace consciousness;

QJsonObject record_t::to_json() const
{
    QJsonObject json_record;
    json_record[keyname_window_name] = QString::fromStdU16String(window_name);
    json_record[keyname_window_class_name] =
        QString::fromStdU16String(window_class_name);
    json_record[keyname_process_name] = QString::fromStdU16String(process_name);
    json_record[keyname_alias] = QString::fromStdU16String(alias);
    return json_record;
}
record_t record_t::from_json(const QJsonObject& json_record_obj)
{
    record_t record;
    record.window_name = json_record_obj[record_t::keyname_window_name]
                             .toString()
                             .toStdU16String();
    record.window_class_name =
        json_record_obj[record_t::keyname_window_class_name]
            .toString()
            .toStdU16String();
    record.process_name = json_record_obj[record_t::keyname_process_name]
                              .toString()
                              .toStdU16String();
    record.alias =
        json_record_obj[record_t::keyname_alias].toString().toStdU16String();
    return record;
}

QJsonObject record_converter::to_json(std::span<const record_t> records)
{
    QJsonArray json_array;
    for (auto& record : records)
        json_array.append(record.to_json());
    QJsonObject json;
    json[record_t::keyname_entry] = json_array;
    return json;
}
void record_converter::to_file(std::span<const record_t> records,
                               const std::filesystem::path& path)
{
    QJsonObject json = to_json(records);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        throw std::runtime_error("Could not open file for writing.");
    file.write(QJsonDocument{json}.toJson());
    file.close();
}

std::vector<record_t> record_converter::from_json(const QJsonObject& json)
{
    std::vector<record_t> records;
    const auto& json_array = json[record_t::keyname_entry].toArray();
    for (const auto& json_record : json_array)
    {
        const auto& json_record_obj = json_record.toObject();
        records.push_back(record_t::from_json(json_record_obj));
    }
    return records;
}
std::vector<record_t> record_converter::from_file(
    const std::filesystem::path& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Could not open file for reading.");
    QJsonDocument json_document = QJsonDocument::fromJson(file.readAll());
    file.close();
    return from_json(json_document.object());
}
