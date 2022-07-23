/**
 * @file main_window.cpp
 * @author UnnamedOrange
 * @brief Main window.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "main_window.h"

#include <QDebug>

main_window::main_window(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    init_list();
}
main_window::~main_window()
{
    auto cs = config_store.lock();
    cs->to_file();
}

void main_window::on_action_exit_triggered() { QApplication::quit(); }

void main_window::init_list()
{
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

    // Add existing items to list.
    for (const auto& item : *cs)
    {
        ui.listWidget_windows->addItem(
            QString::fromStdU16String(item.window_name));
    }

    // Add a dummy item to list.
    add_dummy_item();
}
void main_window::add_dummy_item()
{
    ui.listWidget_windows->addItem(tr("New Item..."));
}