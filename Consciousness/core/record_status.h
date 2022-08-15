/**
 * @file record_status.h
 * @author UnnamedOrange
 * @brief Runtime statuses for a record.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <chrono>

#include "record.h"

namespace consciousness
{
    using clock = std::chrono::steady_clock;

    /**
     * @brief Runtime statuses for a record.
     */
    struct record_status
    {
        /**
         * @brief Whether the record is temporarily permitted.
         * The time point and duration should be checked to determine if the
         * record is authentically permitted.
         */
        bool permit;
        /**
         * @brief Time point of the previous permission.
         * Valid if permit is true.
         */
        clock::time_point previous_permit_time_point;
        /**
         * @brief Duration of the previous permission.
         * Valid if permit is true.
         */
        clock::duration previous_permit_duration;

        /**
         * @brief Whether the record is skipped beacuse of a popup window.
         */
        bool skip_for_popup;
    };
} // namespace consciousness
