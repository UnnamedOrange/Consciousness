/**
 * @file tray.h
 * @author UnnamedOrange
 * @brief Tray system of this app.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "ui_tray.h"

#include <functional>
#include <memory>

#include <QEvent>
#include <QMenu>
#include <QSystemTrayIcon>

class tray : public QMenu
{
    Q_OBJECT

private:
    Ui::tray ui;
    QSystemTrayIcon* system_tray{};

public:
    tray();

private:
    std::function<void()> on_show_main_window;

private slots:
    void on_action_main_window_triggered();
    void on_action_quit_triggered();

private:
    void changeEvent(QEvent* event) override;

private:
    void create_system_tray();

public:
    void show_system_tray();

public:
    void set_on_show_main_window(std::function<void()> on_show_main_window);
};
