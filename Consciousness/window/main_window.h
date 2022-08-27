/**
 * @file main_window.h
 * @author UnnamedOrange
 * @brief Main window for configuring the application.
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

/**
 * @brief Main window for configuring the application.
 * The core is paused when main window is shown.
 * On editing the config, the changes to the config_store are valid immediately.
 * On close, the config_store is saved to the config file.
 */
class main_window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::main_window ui;

public:
    main_window(utils::lock_viewer<consciousness::config_store>& config_store,
                consciousness::core& core, QWidget* parent = nullptr);

private:
    /**
     * @brief Save config on close.
     */
    void closeEvent(QCloseEvent* event);

private slots:
    void on_action_exit_triggered();
    void on_action_system_default_triggered();
    void on_listWidget_windows_itemDoubleClicked(QListWidgetItem* item);
    void _on_listWidget_windows_selection_changed();
    void on_lineEdit_window_name_textEdited(const QString& arg1);
    void on_lineEdit_window_class_name_textEdited(const QString& arg1);
    void on_lineEdit_process_name_textEdited(const QString& arg1);
    void on_lineEdit_alias_textEdited(const QString& arg1);
    void on_button_delete_clicked();
    void on_listWidget_windows_delete_key_pressed();

private:
    std::vector<utils::language_info_t> available_languages{
        utils::i18n::available_languages()};

private slots:
    void _on_action_change_language_triggered();

private:
    /**
     * @brief Change the language of the application.
     *
     * @param language_code utils::language_info_t::code.
     */
    void change_language(const QString& language_code = "");

private:
    utils::lock_viewer<consciousness::config_store>& config_store;
    consciousness::core& core;

private:
    /**
     * @brief Refresh the list.
     * This function is called when main window is created or
     * language has been changed.
     */
    void refresh_list();
    /**
     * @brief Add "New item..." to the list.
     * The string literal is here for Qt Linguist.
     */
    void add_dummy_item();
    /**
     * @brief Enable or disable the editing widgets.
     */
    void enable_editing_widgets(bool enable);
    /**
     * @brief Get the alias of the window to show in the list.
     *
     * @todo Rename and rewrite this function.
     *
     * @param name The window name.
     * @return QString The string to show in the list.
     */
    static QString handle_window_name(const QString& name);
};
