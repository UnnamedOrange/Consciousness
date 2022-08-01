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
    create_system_tray_icon();
    create_menu();
}

void tray::create_system_tray_icon()
{
    system_tray = new QSystemTrayIcon(this);
    system_tray->setToolTip("Consciousness");
    system_tray->setIcon(QFileIconProvider().icon(QFileIconProvider::File));
}
void tray::create_menu()
{
    // Create hierarchy.
    if (menu)
        delete menu;
    menu = new QMenu(this);
    auto* show_main_window_action = new QAction(tr("&Main Window"), menu);
    menu->addAction(show_main_window_action);
    menu->addSeparator();
    auto* quit_action = new QAction(tr("&Quit"), menu);
    menu->addAction(quit_action);
    system_tray->setContextMenu(menu);

    // Connect signals.
    connect(system_tray, &QSystemTrayIcon::activated, this,
            &tray::show_main_window_proxy);
    connect(show_main_window_action, &QAction::triggered, this,
            &tray::show_main_window_proxy);
    connect(quit_action, &QAction::triggered, this,
            [this]() { QApplication::quit(); });
}

void tray::show() { system_tray->show(); }

void tray::show_main_window_proxy()
{
    if (on_show_main_window)
        on_show_main_window();
}

void tray::set_on_show_main_window(std::function<void()> on_show_main_window)
{
    this->on_show_main_window = on_show_main_window;
}

void tray::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange)
        create_menu();
}
