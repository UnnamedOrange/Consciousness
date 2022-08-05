/**
 * @file tray.cpp
 * @author UnnamedOrange
 * @brief Tray system and data center of this app.
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

    auto cs = config_store.lock();
    try
    {
        cs->from_file();
    }
    catch (const std::runtime_error& e)
    {
        qDebug() << e.what();
        cs->clear(); // If failed, start with empty list.
    }
}

void tray::on_system_tray_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        on_action_main_window_triggered();
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
            &tray::on_system_tray_activated);
}

void tray::show_system_tray() { system_tray->show(); }

void tray::set_on_show_main_window(std::function<void()> on_show_main_window)
{
    this->on_show_main_window = on_show_main_window;
}
