/**
 * @file main_window.cpp
 * @author UnnamedOrange
 * @brief Main window for configuring the application.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "main_window.h"

#include <algorithm>

#include <QDebug>
#include <QDir>
#include <QStringList>

#include <core/config_store.h>
#include <core/record.h>

main_window::main_window(
    utils::lock_viewer<consciousness::config_store>& config_store,
    consciousness::core& core, QWidget* parent)
    : QMainWindow(parent), config_store(config_store), core(core)
{
    ui.setupUi(this);

    // Initialize the language menu.
    for (const auto& language : available_languages)
    {
        ui.menu_language->addAction(
            language.name, this,
            &main_window::_on_action_change_language_triggered);
    }

    // Pause the core.
    core.set_pause(true);

    // Initialize the list.
    refresh_list();
    enable_editing_widgets(false);

    // Link signals.
    connect(ui.listWidget_windows, &QListWidget::currentRowChanged, this,
            &main_window::_on_listWidget_windows_selection_changed);
    connect(ui.listWidget_windows, &QListWidget::itemSelectionChanged, this,
            &main_window::_on_listWidget_windows_selection_changed);
}
void main_window::closeEvent(QCloseEvent* event)
{
    auto cs = config_store.lock();
    cs->to_file();

    core.set_pause(false);
}

void main_window::on_action_exit_triggered() { QApplication::quit(); }
void main_window::on_action_system_default_triggered() { change_language(); }
void main_window::on_listWidget_windows_itemDoubleClicked(QListWidgetItem* item)
{
    // If the last item (New item...) is double clicked, add a new item.
    if (ui.listWidget_windows->currentRow() ==
        ui.listWidget_windows->count() - 1)
    {
        auto cs = config_store.lock();
        cs->emplace_back();
        item->setText(handle_window_name(
            QString::fromStdU16String(cs->back().window_name)));
        add_dummy_item();
        _on_listWidget_windows_selection_changed();
    }
}
void main_window::_on_listWidget_windows_selection_changed()
{
    int index = ui.listWidget_windows->currentRow();
    if (~index && !ui.listWidget_windows->currentItem()->isSelected())
        index = -1;
    if (index == -1 || index == ui.listWidget_windows->count() - 1)
    {
        enable_editing_widgets(false);
        ui.lineEdit_window_name->clear();
        ui.lineEdit_window_class_name->clear();
        ui.lineEdit_process_name->clear();
        ui.lineEdit_alias->clear();
        return;
    }
    auto cs = config_store.lock();
    auto& record = (*cs)[index];
    ui.lineEdit_window_name->setText(
        QString::fromStdU16String(record.window_name));
    ui.lineEdit_window_class_name->setText(
        QString::fromStdU16String(record.window_class_name));
    ui.lineEdit_process_name->setText(
        QString::fromStdU16String(record.process_name));
    ui.lineEdit_alias->setText(QString::fromStdU16String(record.alias));
    enable_editing_widgets(true);
}
void main_window::on_lineEdit_window_name_textEdited(const QString& arg1)
{
    int index = ui.listWidget_windows->currentRow();
    ui.listWidget_windows->currentItem()->setText(handle_window_name(arg1));
    auto cs = config_store.lock();
    (*cs)[index].window_name = arg1.toStdU16String();
}
void main_window::on_lineEdit_window_class_name_textEdited(const QString& arg1)
{
    int index = ui.listWidget_windows->currentRow();
    auto cs = config_store.lock();
    (*cs)[index].window_class_name = arg1.toStdU16String();
}
void main_window::on_lineEdit_process_name_textEdited(const QString& arg1)
{
    int index = ui.listWidget_windows->currentRow();
    auto cs = config_store.lock();
    (*cs)[index].process_name = arg1.toStdU16String();
}
void main_window::on_lineEdit_alias_textEdited(const QString& arg1)
{
    int index = ui.listWidget_windows->currentRow();
    auto cs = config_store.lock();
    (*cs)[index].alias = arg1.toStdU16String();
}
void main_window::on_button_delete_clicked()
{
    // Get the index of the selected item.
    int index = ui.listWidget_windows->currentRow();
    if (~index && !ui.listWidget_windows->currentItem()->isSelected())
        index = -1;
    if (index == -1 || index == ui.listWidget_windows->count() - 1)
        return;

    // Remove the item from the list.
    auto cs = config_store.lock();
    cs->erase(cs->begin() + index);
    ui.listWidget_windows->takeItem(index);

    // Update the current item.
    if (index >= ui.listWidget_windows->count())
        index = ui.listWidget_windows->count() - 1;
    ui.listWidget_windows->setCurrentRow(index);
    _on_listWidget_windows_selection_changed();
}
void main_window::on_listWidget_windows_delete_key_pressed()
{
    on_button_delete_clicked();
}

void main_window::_on_action_change_language_triggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QString lang_name = action->text();
    auto it = std::find_if(
        available_languages.begin(), available_languages.end(),
        [&lang_name](const auto& lang) { return lang.name == lang_name; });
    change_language(it->code);
}
void main_window::change_language(const QString& language_code)
{
    if (language_code.isEmpty())
        utils::i18n::change_language_to_system_default();
    else
        utils::i18n::change_language(language_code);
    ui.retranslateUi(this);
    refresh_list();
}

void main_window::refresh_list()
{
    auto cs = config_store.lock();

    // Store current index.
    int index = ui.listWidget_windows->currentRow();
    if (~index && !ui.listWidget_windows->currentItem()->isSelected())
        index = -1;

    // Clear the list.
    ui.listWidget_windows->clear();

    // Add existing items to list.
    for (const auto& item : *cs)
    {
        ui.listWidget_windows->addItem(
            handle_window_name(QString::fromStdU16String(item.window_name)));
    }

    // Add a dummy item to list.
    add_dummy_item();

    // Restore current index.
    if (index != -1)
        ui.listWidget_windows->setCurrentRow(index);
}
void main_window::add_dummy_item()
{
    ui.listWidget_windows->addItem(tr("New Item..."));
}
void main_window::enable_editing_widgets(bool enable)
{
    ui.lineEdit_window_name->setEnabled(enable);
    ui.lineEdit_window_class_name->setEnabled(enable);
    ui.lineEdit_process_name->setEnabled(enable);
    ui.lineEdit_alias->setEnabled(enable);
    ui.button_delete->setEnabled(enable);
}
QString main_window::handle_window_name(const QString& name)
{
    if (name.isEmpty())
        return tr("(Window name not set)");
    return name;
}
