/**
 * @file core.cpp
 * @author UnnamedOrange
 * @brief Backend of core for Windows.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "../core.h"

#include <filesystem>
#include <vector>

#include "../record_status.h"

using namespace consciousness;

#if defined(_WIN32)

#include <Windows.h>

using native_handle_t = HWND;

std::u16string get_window_name(native_handle_t hwnd)
{
    int length = GetWindowTextLengthW(hwnd);
    if (!length)
        return std::u16string{};
    std::wstring result(length + 1, 0);
    GetWindowTextW(hwnd, result.data(), length + 1);
    result.resize(length);
    std::filesystem::path code_convert{std::move(result)};
    return code_convert.u16string();
}
std::u16string get_window_class_name(native_handle_t hwnd)
{
    constexpr int max_length = 256;
    std::wstring result(max_length + 1, 0);
    int length = GetClassNameW(hwnd, result.data(), max_length + 1);
    result.resize(length);
    std::filesystem::path code_convert{std::move(result)};
    return code_convert.u16string();
}

bool is_window_matched(native_handle_t hwnd, const record_t& r)
{
    if (r.window_name.empty() && r.window_class_name.empty())
        return false;

    if (!r.window_name.empty() && r.window_name != get_window_name(hwnd))
        return false;
    if (!r.window_class_name.empty() &&
        r.window_class_name != get_window_class_name(hwnd))
        return false;
    return true;
}

std::vector<native_handle_t> find_matched_windows_all(const record_t& r)
{
    std::vector<native_handle_t> ret;
    struct param_t
    {
        std::reference_wrapper<const record_t> r;
        std::reference_wrapper<std::vector<native_handle_t>> ret;
    } param{r, ret};
    EnumWindows(
        [](HWND hwnd, LPARAM lParam) {
            auto& [r, ret] = *reinterpret_cast<param_t*>(lParam);
            if (is_window_matched(hwnd, r))
                ret.get().push_back(hwnd);
            return TRUE;
        },
        reinterpret_cast<LPARAM>(&param));

    return ret;
}

std::u16string get_alias(const record_t& r)
{
    if (!r.window_name.empty())
        return r.window_name;
    return r.window_class_name;
}

void core::poll()
{
    auto cs = config_store.lock();
    // Steps:
    // 1. Iterate `(*cs)`, denoting an element by *record*.
    // 2. Find the rule of *record* in `this->runtime_status[record.id]`.
    // 3.1. If the record is permitted right now, just skip.
    // 3.2. If the record is false permitted, update `rule.permit`.
    // 4.1. If related popup exists, just skip.
    // 4.2. Else, check if there is any matched window.
    // 5.2. (see 5.1.) Else, do nothing.
    // 5.1. If there is any, close all, and show the popup
    //   to let the user decide if he was conscious.

    // 1.
    for (const auto& record : *cs)
    {
        // 2.
        // Because runtime_status has been maintained,
        // no check for existence is needed.
        auto& rule = runtime_status[record.id];

        if (rule.permit && clock::now() <= rule.previous_permit_time_point +
                                               rule.previous_permit_duration)
            continue; // 3.1.
        else if (rule.permit)
            rule.permit = false; // 3.2.

        if (rule.skip_for_popup)
            continue; // 4.1.

        // 4.2.
        auto matched_windows = find_matched_windows_all(record);

        // 5.2.
        if (matched_windows.empty())
            continue;

        // 5.1.
        // Close all windows.
        for (native_handle_t hwnd : matched_windows)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
        // Popup to let the user decide if he was conscious.
        rule.skip_for_popup = true; // Update rule.
        // Show the popup.
        pm.create_popup(record.id, get_alias(record));
    }
}

#endif // _WIN32
