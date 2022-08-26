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

#include <memory>
#include <random>

#include <QDialog>
#include <QEvent>
#include <QString>
#include <QStringView>

class question_base_t
{
public:
    virtual QString get_question() const = 0;
    virtual bool check(QStringView answer) const = 0;
};
class question_arithmetic_t : public question_base_t
{
private:
    int a, b;

public:
    question_arithmetic_t()
    {
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_int_distribution<int> dist(114, 514);
        a = dist(e);
        b = dist(e);
    }
    QString get_question() const override
    {
        return QString("%1 + %2 = ?").arg(a).arg(b);
    }
    bool check(QStringView answer) const override
    {
        return answer.toInt() == a + b;
    }
};

/**
 * @brief Popup dialog to remind the user to be conscious.
 * The dialog should be created on the main thread using popup_manager.
 * It uses slots and signals (closed) to notify the core.
 * Input: id and alias.
 * Output: user's dicision.
 */
class popup : public QDialog
{
    Q_OBJECT

private:
    Ui::popup ui;

private:
    std::shared_ptr<question_base_t> question;
    uint32_t id;

public:
    popup(uint32_t id, const QString& alias, QWidget* parent = nullptr);

private slots:
    void on_edit_answer_textChanged(const QString& arg1);
    void on_dial_minute_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_popup_accepted();
    void on_popup_rejected();

signals:
    void closed(uint32_t id, bool permit, int minutes);

private:
    /**
     * @brief Monitor the language change.
     */
    void changeEvent(QEvent* event) override;

private:
    /**
     * @brief Generate the question and set the question label.
     */
    void initialize_question();
};
