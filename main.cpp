#include <QApplication>
#include <QSettings>

#include "cmainwindow.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(chroniclernext);

    QApplication app(argv, args);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Chronicler", "Chronicler");

    CMainWindow mainWindow(&settings);
    mainWindow.setGeometry(100, 100, 1280, 720);
    mainWindow.show();
    
    return app.exec();
}
