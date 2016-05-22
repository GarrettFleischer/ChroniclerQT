#include <QApplication>
#include <QSettings>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaContent>
#include <QTimer>

#include <QDebug>

#include "cmainwindow.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(chroniclernext);

    QApplication app(argv, args);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Chronicler-Next", "Chronicler");

    bool showIntro = settings.value("MainWindow/ShowIntro", false).toBool();

    CMainWindow mainWindow(&settings);
    mainWindow.setGeometry(100, 100, 1280, 720);


    //     Show the intro video or not, false by default.
    if(!showIntro)
        mainWindow.show();
    else
    {
        QMediaPlayer mp;
        QVideoWidget intro;

        mp.setVideoOutput(&intro);
        mp.setMedia(QUrl::fromLocalFile(app.applicationDirPath() + "/GameSmithIntro.mp4"));

        qDebug() << app.applicationDirPath() + "/GameSmithIntro.mp4";

        intro.setGeometry(100, 100, 1280, 720);
        intro.show();

        mp.play();

        QTimer::singleShot(7000, Qt::VeryCoarseTimer, &intro, SLOT(close()));
        QTimer::singleShot(7000, Qt::VeryCoarseTimer, &mainWindow, SLOT(show()));
    }
    
    return app.exec();
}
