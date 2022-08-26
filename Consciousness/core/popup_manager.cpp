/**
 * @file popup_manager.cpp
 * @author UnnamedOrange
 * @brief Manage popup dialogs using slots and signals.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "core.h"

#include "popup_manager.h"

#include <QObject>

#include <window/popup.h>

using namespace consciousness;

popup_manager::popup_manager(core& _core) : _core(_core)
{
    connect(this, &popup_manager::create_popup_signal, this,
            &popup_manager::_on_create_popup);
}

void popup_manager::create_popup(uint32_t id, const std::u16string& alias)
{
    emit create_popup_signal(id, alias);
}

void popup_manager::_on_create_popup(uint32_t id, const std::u16string& alias)
{
    popup* p = new popup(id, QString::fromStdU16String(alias));
    connect(p, &popup::closed, this, &popup_manager::_on_popup_closed);
    p->setAttribute(Qt::WA_DeleteOnClose);
    p->open();
}

void popup_manager::_on_popup_closed(uint32_t id, bool permit, int minutes)
{
    _core.notify_popup_closed({id, permit, minutes});
}
