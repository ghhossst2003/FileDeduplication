#include "samefilelistwidget.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "repeatfileitem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "remove_same_file_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    SameFileListWidget w;
//    RepeatFileItem w;
    w.show();
    return a.exec();
}
