#include <QSettings>
#include <QFileInfo>
#include <QProcess>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Chronicler-Next", "Chronicler");
    QString update_dir = QFileInfo(settings.fileName()).absolutePath();
    QString program_dir = QFileInfo(argv[0]).absolutePath();
    QString program;

#ifdef Q_OS_WIN
    program = "/Chronicler-Next.exe";
#endif
#ifdef Q_OS_OSX
    program = "";
#endif
#ifdef Q_OS_LINUX
    program = "/Chronicler-Next";
#endif

    if(!program.isEmpty())
    {
        program_dir += program;
        update_dir += program;

        QFile::remove(program_dir);
        QFile::copy(update_dir, program_dir);

        QProcess process;
        process.startDetached(program_dir);
    }

    return 0;
}
