/**
 * @file popup.h
 * @author UnnamedOrange
 * @brief Popup dialog to remind the user to be conscious.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "ui_popup.h"

#include <QDialog>

class popup : public QDialog
{
    Q_OBJECT

private:
    Ui::popup ui;

public:
    explicit popup(QWidget* parent = nullptr);
};
