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
#include <unordered_map>

#include "config_store.h"
#include "record_status.h"
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

        /**
         * config_store can be accessed by multiple threads.
         * runtime_status is internal.
         * Access runtime_status via id from config_store.
         *
         * e.g.
         *     runtime_status[config_store.lock()->back().id]
         */
    private:
        utils::lock_viewer<consciousness::config_store>& config_store;
        bool is_pause{};
        std::unordered_map<std::uint32_t, record_status> runtime_status;

    private:
        /**
         * @brief Maintain runtime_status according to the config_store.
         */
        void maintain_runtime_status()
        {
            auto cs = config_store.lock();

            // Create a new record_status if it doesn't exist.
            for (const auto& r : *cs)
                runtime_status.operator[](r.id);

            // Remove records that are no longer in the config_store.
            // Considering modification of config_store is rare,
            // this is not a performance issue.
            for (auto it = runtime_status.begin(); it != runtime_status.end();)
            {
                if (std::find_if(cs->begin(), cs->end(), [&it](const auto& r) {
                        return r.id == it->first;
                    }) == cs->end())
                    it = runtime_status.erase(it);
                else
                    ++it;
            }

            // Update permit.
            for (auto& [_, s] : runtime_status)
            {
                if (s.permit &&
                    s.previous_permit_time_point + s.previous_permit_duration <
                        clock::now())
                    s.permit = false;
            }
        }

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

                if (is_pause)
                    continue;
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

    public:
        /**
         * @brief Pause the core.
         */
        void set_pause(bool pause) { is_pause = pause; }
    };
} // namespace consciousness
