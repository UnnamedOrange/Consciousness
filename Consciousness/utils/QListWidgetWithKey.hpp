/**
 * @file QListWidgetWithKey.hpp
 * @author UnnamedOrange
 * @brief QListWidget with key event capture.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <QKeyEvent>
#include <QListWidget>

class QListWidgetWithKey : public QListWidget
{
    Q_OBJECT

public:
    QListWidgetWithKey(QWidget* parent = nullptr) : QListWidget(parent) {}

public:
    void keyPressEvent(QKeyEvent* event) override
    {
        QListWidget::keyPressEvent(event);
        if (event->key() == Qt::Key_Delete)
        {
            emit delete_key_pressed();
        }
    }

signals:
    void delete_key_pressed();
};
