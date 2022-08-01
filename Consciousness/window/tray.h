/**
 * @file tray.h
 * @author UnnamedOrange
 * @brief Tray system of this app.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <functional>
#include <memory>

#include <QAction>
#include <QEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QWidget>

class tray : public QWidget
{
    Q_OBJECT

private:
    QSystemTrayIcon* system_tray{};
    QMenu* menu{};
    std::function<void()> on_show_main_window;

public:
    tray();

private:
    void create_system_tray_icon();
    void create_menu();

public:
    void show();

private slots:
    void show_main_window_proxy();

public:
    void set_on_show_main_window(std::function<void()> on_show_main_window);

private:
    void changeEvent(QEvent* event) override;
};
