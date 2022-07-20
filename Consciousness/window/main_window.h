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

class main_window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::main_window ui;

public:
    main_window(QWidget* parent = nullptr);
};
