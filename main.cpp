#include <QApplication>
#include <QSettings>
#include <QTranslator>

#include "cmainwindow.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(chroniclernext);

    QApplication app(argv, args);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Chronicler-Next", "Chronicler");
    QRect windowRect = settings.value("MainWindow/Geometry", QRect(100, 100, 1280, 720)).value<QRect>();

//    QTranslator translator;
//    translator.load("C:\\chroniclernext_es.qm");
//    app.installTranslator(&translator);

    QString filename = ((argv > 1) ? args[1] : "");
    CMainWindow mainWindow(&settings, filename);
    mainWindow.setGeometry(windowRect);

    mainWindow.show();

    return app.exec();
}
