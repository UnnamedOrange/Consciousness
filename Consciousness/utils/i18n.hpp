/**
 * @file i18n.hpp
 * @author UnnamedOrange
 * @brief i18n using QTranslator.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <QApplication>
#include <QLocale>
#include <QTranslator>

namespace utils
{
    class i18n
    {
    private:
        inline static QTranslator translator{};

    public:
        static void change_language_to_system_default()
        {
            auto& a = *QApplication::instance();
            a.removeTranslator(&translator);

            const QStringList uiLanguages = QLocale::system().uiLanguages();
            for (const QString& locale : uiLanguages)
            {
                QString baseName = QLocale(locale).name();
                if (translator.load(":/i18n/" + baseName))
                {
                    a.installTranslator(&translator);
                    break;
                }
            }
        }
        static void change_language(const QString& language_base_name)
        {
            auto& a = *QApplication::instance();
            a.removeTranslator(&translator);

            if (translator.load(":/i18n/" + language_base_name))
                a.installTranslator(&translator);
        }
    };
} // namespace utils
