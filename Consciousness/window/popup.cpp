/**
 * @file popup.cpp
 * @author UnnamedOrange
 * @brief Popup dialog to remind the user to be conscious.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "popup.h"

#include <QPushButton>

popup::popup(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);

    // Initialize the dialog.
    setAttribute(Qt::WidgetAttribute::WA_QuitOnClose, false);
    ui.verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint |
                   Qt::WindowStaysOnTopHint);

    // Initialize the labels.
    on_dial_minute_valueChanged(ui.dial_minute->value());

    // Initialize the buttons.
    ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok)
        ->setEnabled(false);
}

void popup::on_dial_minute_valueChanged(int value)
{
    ui.label_minute->setText(QString("%1").arg(value, 2, 10, QLatin1Char('0')));
}

void popup::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);
    if (event->type() == QEvent::LanguageChange)
        ui.retranslateUi(this);
}
