/**
 * @file main_window.cpp
 * @author UnnamedOrange
 * @brief Main window.
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

main_window::main_window(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    // Initialize the language menu.
    for (const auto& language : available_languages)
    {
        ui.menu_language->addAction(
            language.name, this,
            &main_window::on_action_change_language_triggered);
    }

    // Initialize the list.
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
    init_list();
    enable_edit_widgets(false);
}
main_window::~main_window()
{
    auto cs = config_store.lock();
    cs->to_file();
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
        on_listWidget_windows_itemSelectionChanged();
    }
}
void main_window::on_listWidget_windows_itemSelectionChanged()
{
    int index = ui.listWidget_windows->currentRow();
    if (~index && !ui.listWidget_windows->currentItem()->isSelected())
        index = -1;
    if (index == -1 || index == ui.listWidget_windows->count() - 1)
    {
        enable_edit_widgets(false);
        ui.lineEdit_window_name->clear();
        ui.lineEdit_window_class_name->clear();
        return;
    }
    auto cs = config_store.lock();
    auto& record = (*cs)[index];
    ui.lineEdit_window_name->setText(
        QString::fromStdU16String(record.window_name));
    ui.lineEdit_window_class_name->setText(
        QString::fromStdU16String(record.window_class_name));
    enable_edit_widgets(true);
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
    on_listWidget_windows_itemSelectionChanged();
}

void main_window::on_action_change_language_triggered()
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
    init_list();
}

void main_window::init_list()
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
void main_window::enable_edit_widgets(bool enable)
{
    ui.lineEdit_window_name->setEnabled(enable);
    ui.lineEdit_window_class_name->setEnabled(enable);
    ui.button_delete->setEnabled(enable);
}
QString main_window::handle_window_name(const QString& name)
{
    if (name.isEmpty())
        return tr("(Window name not set)");
    return name;
}
