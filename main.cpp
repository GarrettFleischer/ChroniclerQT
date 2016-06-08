#include <QApplication>
#include <QSettings>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaContent>
#include <QTimer>

#include <QTranslator>

#include "cmainwindow.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(chroniclernext);

    QApplication app(argv, args);

//    QTranslator translator;
//    translator.load("C:\\chroniclernext_es.qm");
//    app.installTranslator(&translator);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Chronicler-Next", "Chronicler");

    QString filename = ((argv > 1) ? args[1] : "");

    CMainWindow mainWindow(&settings, filename);

    QRect window = settings.value("MainWindow/Geometry", QRect(100, 100, 1280, 720)).value<QRect>();

    mainWindow.setGeometry(window);

    mainWindow.show();


    return app.exec();
}
