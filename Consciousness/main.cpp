/**
 * @file main.cpp
 * @author UnnamedOrange
 * @brief Entry.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <window/main_window.h>

#include <QApplication>

#include <utils/i18n.hpp>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    utils::i18n::change_language_to_system_default();
    main_window w;
    w.show();
    return a.exec();
}
