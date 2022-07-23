/**
 * @file main_window.h
 * @author UnnamedOrange
 * @brief Header for main window.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "./ui_main_window.h"

#include <QMainWindow>

#include <core/config_store.h> // Shoule be included after QMainWindow. Bug of Qt.
#include <utils/lock_view.hpp>

class main_window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::main_window ui;

public:
    main_window(QWidget* parent = nullptr);
    ~main_window();

private slots:
    void on_action_exit_triggered();

private:
    consciousness::config_store _config_store_value;
    utils::lock_viewer<consciousness::config_store> config_store{
        _config_store_value};

private:
    void init_list();
    void add_dummy_item();
};
