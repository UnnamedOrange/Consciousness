/**
 * @file tray.cpp
 * @author UnnamedOrange
 * @brief Tray system of this app.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "tray.h"

#include <QApplication>
#include <QEvent>
#include <QFileIconProvider>
#include <QIcon>

tray::tray()
{
    ui.setupUi(this);
    create_system_tray();
}

void tray::on_action_main_window_triggered()
{
    if (on_show_main_window)
        on_show_main_window();
}
void tray::on_action_quit_triggered() { QApplication::quit(); }

void tray::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange)
        ui.retranslateUi(this);
}

void tray::create_system_tray()
{
    system_tray = new QSystemTrayIcon(this);
    system_tray->setToolTip("Consciousness");
    system_tray->setIcon(QFileIconProvider().icon(QFileIconProvider::File));
    system_tray->setContextMenu(this);

    // Connect signals.
    connect(system_tray, &QSystemTrayIcon::activated, this,
            &tray::on_action_main_window_triggered);
}

void tray::show_system_tray() { system_tray->show(); }

void tray::set_on_show_main_window(std::function<void()> on_show_main_window)
{
    this->on_show_main_window = on_show_main_window;
}
