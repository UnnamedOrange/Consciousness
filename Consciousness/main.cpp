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
#include <singleapplication.h>

#include <utils/i18n.hpp>
#include <window/main_window.h>
#include <window/tray.h>

/**
 * @brief Entry of the application.
 * The background program is on the tray.
 */
int main(int argc, char* argv[])
{
    SingleApplication a(argc, argv);
    utils::i18n::change_language_to_system_default();

    tray tray;
    tray.show_system_tray();

    std::shared_ptr<main_window> w;
    bool is_creating{}; // The callback below is called in interrupt.
    tray.set_show_main_window_callback([&]() {
        if (is_creating)
            return;
        is_creating = true;
        if (!w || !w->isVisible())
        {
            w = std::make_shared<main_window>(tray.get_config_store(),
                                              tray.get_core());
            w->setAttribute(Qt::WA_QuitOnClose, false);
            w->show();
        }
        else if (w->isMinimized())
            w->showNormal();
        w->raise();
        w->activateWindow();
        is_creating = false;
    });

    tray.show_main_window();

    return a.exec();
}
