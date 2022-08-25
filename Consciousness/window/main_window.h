/**
 * @file main_window.h
 * @author UnnamedOrange
 * @brief Header for main window.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "ui_main_window.h"

#include <QMainWindow>
#include <QString>
#include <QTranslator>

#include <core/config_store.h> // Shoule be included after QMainWindow. Bug of Qt.
#include <core/core.h>
#include <utils/i18n.hpp>
#include <utils/lock_view.hpp>

class main_window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::main_window ui;

public:
    main_window(utils::lock_viewer<consciousness::config_store>& config_store,
                consciousness::core& core, QWidget* parent = nullptr);

private:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_action_exit_triggered();
    void on_action_system_default_triggered();
    void on_listWidget_windows_itemDoubleClicked(QListWidgetItem* item);
    void _on_listWidget_windows_selection_changed();
    void on_lineEdit_window_name_textEdited(const QString& arg1);
    void on_lineEdit_window_class_name_textEdited(const QString& arg1);
    void on_lineEdit_process_name_textEdited(const QString& arg1);
    void on_button_delete_clicked();
    void on_listWidget_windows_delete_key_pressed();

private:
    std::vector<utils::language_info_t> available_languages{
        utils::i18n::available_languages()};

private slots:
    void on_action_change_language_triggered();

private:
    void change_language(const QString& language_code = "");

private:
    utils::lock_viewer<consciousness::config_store>& config_store;
    consciousness::core& core;

private:
    void init_list();
    void add_dummy_item();
    void enable_edit_widgets(bool enable);
    static QString handle_window_name(const QString& name);
};
