/**
 * @file lock_view.hpp
 * @author UnnamedOrange
 * @brief Lock view.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <mutex>
#include <type_traits>

namespace utils
{
    template <typename value_type>
    class _lock_view
    {
    private:
        value_type& _ref;
        std::mutex& _mutex;

    public:
        _lock_view(value_type& ref, std::mutex& mutex)
            : _ref(ref), _mutex(mutex)
        {
            _mutex.lock();
        }
        ~_lock_view() { _mutex.unlock(); }
        value_type* operator->() { return &_ref; }
        value_type& operator*() { return _ref; }
    };

    template <typename T>
    class lock_viewer
    {
    public:
        using value_type = std::decay_t<T>;

    private:
        value_type& _ref;
        std::mutex _mutex;

    public:
        lock_viewer(value_type& ref) : _ref(ref) {}

    public:
        _lock_view<value_type> lock()
        {
            return _lock_view<value_type>(_ref, _mutex);
        }
    };
} // namespace utils
