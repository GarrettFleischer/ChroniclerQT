#ifndef CSETTINGSVIEW_H
#define CSETTINGSVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSettings;
class QLayout;
QT_END_NAMESPACE


class CSettingsView : public QWidget
{
    Q_OBJECT

public:
    CSettingsView(QSettings *settings, QWidget *parent = 0);

private:
    //max autosaves
    //max undos
    //save undo history to project
    //choicescript directory
    //dark/light theme?
    //editor font
    //autocomplete snippets
    //hide/show various warnings

    void SetupChoiceScript(QLayout *main_layout);
    void SetupEditor(QLayout *main_layout);
    void SetupHistory(QLayout *main_layout);

    QSettings *m_settings;


private slots:
    void CSDirChanged(QString);

    void AutosavesChanged(int);
    void UndosChanged(int);
    void StoreHistoryChanged(int);
};

#endif // CSETTINGSVIEW_H
