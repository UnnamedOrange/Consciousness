/**
 * @file core.cpp
 * @author UnnamedOrange
 * @brief Backend of core for Windows.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "../core.h"

using namespace consciousness;

#if defined(_WIN32)

#include <Windows.h>

void core::poll()
{
    auto cs = config_store.lock();
    // TODO.
}

#endif // _WIN32
