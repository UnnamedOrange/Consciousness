/**
 * @file core.h
 * @author UnnamedOrange
 * @brief Define backend core.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <chrono>
#include <semaphore>
#include <thread>

#include "config_store.h"
#include <utils/lock_view.hpp>

namespace consciousness
{
    /**
     * @brief Backend core.
     * A thread is created on constructing this class.
     */
    class core
    {
    public:
        static constexpr auto polling_interval = std::chrono::milliseconds(500);

    private:
        utils::lock_viewer<consciousness::config_store>& config_store;

    private:
        std::thread _thread;
        std::binary_semaphore _exit_sem{0};

    private:
        void background_routine()
        {
            while (true)
            {
                if (_exit_sem.try_acquire_for(polling_interval))
                    break;

                poll();
            }
        }
        /**
         * @brief Platform dependent polling routine.
         */
        void poll();

    public:
        core(utils::lock_viewer<consciousness::config_store>& config_store)
            : _thread(&core::background_routine, this),
              config_store(config_store)
        {
        }
        ~core()
        {
            _exit_sem.release();
            _thread.join();
        }
    };
} // namespace consciousness
