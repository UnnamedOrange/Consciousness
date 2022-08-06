/**
 * @file platform.h
 * @author UnnamedOrange
 * @brief Platform selector for the core.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#if defined(_WIN32)
#include "windows/core.h"
#endif
