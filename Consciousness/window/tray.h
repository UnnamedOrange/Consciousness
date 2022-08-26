/**
 * @file tray.h
 * @author UnnamedOrange
 * @brief Tray system and data center of this app.
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

#include <core/config_store.h> // Shoule be included after QMainWindow. Bug of Qt.
#include <core/core.h>
#include <utils/lock_view.hpp>

/**
 * @brief Tray system and data center of this app.
 * The properties of QMenu is not used.
 * Call show_system_tray() as show() in usual QWidget.
 * Use set_show_main_window_callback() to set the action of showing main window.
 */
class tray : public QMenu
{
    Q_OBJECT

private:
    Ui::tray ui;
    QSystemTrayIcon* system_tray{};

private:
    std::function<void()> show_main_window_callback;

    consciousness::config_store _config_store_value;
    utils::lock_viewer<consciousness::config_store> config_store{
        _config_store_value};

    consciousness::core _core{config_store}; // Define core after config_store.

public:
    tray();

private slots:
    void _on_system_tray_activated(QSystemTrayIcon::ActivationReason reason);
    void on_action_main_window_triggered();
    void on_action_quit_triggered();

private:
    /**
     * @brief Monitor the language change.
     */
    void changeEvent(QEvent* event) override;

private:
    /**
     * @brief Create system tray object and connect signals.
     *
     * @note This function is once-called.
     */
    void create_system_tray();

public:
    /**
     * @brief Show the system tray.
     * Call this function as show() in usual QWidget.
     */
    void show_system_tray();

public:
    /**
     * @brief Set callback function when the action "Main Window" is triggered.
     */
    void set_show_main_window_callback(
        std::function<void()> show_main_window_callback);
    /**
     * @brief Call show_main_window_callback() if it is valid.
     */
    void show_main_window() const;
    /**
     * @brief Get reference to config_store.
     */
    auto& get_config_store() { return config_store; }
    /**
     * @brief Get reference to core.
     */
    auto& get_core() { return _core; }
};
