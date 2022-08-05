/**
 * @file main.cpp
 * @author UnnamedOrange
 * @brief Entry.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <memory>

#include <QApplication>

#include <utils/i18n.hpp>
#include <window/main_window.h>
#include <window/tray.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    utils::i18n::change_language_to_system_default();

    tray tray;
    tray.show_system_tray();

    std::shared_ptr<main_window> w;
    bool is_creating{}; // The callback below is called in interrupt.
    tray.set_on_show_main_window([&]() {
        if (is_creating)
            return;
        is_creating = true;
        if (!w || !w->isVisible())
        {
            w = std::make_shared<main_window>(tray.get_config_store());
            w->setAttribute(Qt::WA_QuitOnClose, false);
            w->show();
            w->raise();
            w->activateWindow();
        }
        else
        {
            w.reset();
        }
        is_creating = false;
    });

    tray.call_show_main_window();

    return a.exec();
}
