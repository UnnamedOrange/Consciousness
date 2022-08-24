/**
 * @file popup.cpp
 * @author UnnamedOrange
 * @brief Popup dialog to remind the user to be conscious.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "popup.h"

#include <QLayout>

popup::popup(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WidgetAttribute::WA_QuitOnClose, false);
    ui.verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint |
                   Qt::WindowStaysOnTopHint);
}

void popup::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);
    if (event->type() == QEvent::LanguageChange)
        ui.retranslateUi(this);
}
