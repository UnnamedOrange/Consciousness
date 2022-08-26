/**
 * @file popup_manager.h
 * @author UnnamedOrange
 * @brief Manage popup dialogs using slots and signals.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <string>

#include <QObject>

namespace consciousness
{
    class core;

    /**
     * @brief Struct of user's decision.
     */
    struct popup_close_t
    {
        uint32_t id;
        bool permit;
        int minutes;
    };

    /**
     * @brief Manage popup dialogs using slots and signals.
     */
    class popup_manager : public QObject
    {
        Q_OBJECT

    private:
        core& _core;

    public:
        popup_manager(core& _core);

    public:
        /**
         * @brief Call from any thread to create a popup.
         *
         * @param id ID of the record.
         * @param alias The alias to show on the popup.
         */
        void create_popup(uint32_t id, const std::u16string& alias);

    private slots:
        /**
         * @brief Create a popup on main thread.
         */
        void _on_create_popup(uint32_t id, const std::u16string& alias);

    signals:
        void create_popup_signal(uint32_t id, const std::u16string& alias);

    public slots:
        /**
         * @brief Call from main thread to notify that the popup has closed.
         */
        void _on_popup_closed(uint32_t id, bool permit, int minutes);
    };
} // namespace consciousness
