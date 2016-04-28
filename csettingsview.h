#ifndef CSETTINGSVIEW_H
#define CSETTINGSVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSettings;
class QLayout;
class QLineEdit;
class QColor;
class QFontComboBox;
class QSpinBox;
class QPushButton;
class QSize;
class QCheckBox;
QT_END_NAMESPACE


class CSettingsView : public QWidget
{
    Q_OBJECT

public:
    CSettingsView(QSettings *settings, QWidget *parent = 0);

private:
    //autocomplete snippets
    //hide/show various warnings

    void SetupChoiceScript(QLayout *main_layout);
    void SetupEditor(QLayout *main_layout);
    void SetupHistory(QLayout *main_layout);

    void LoadSettings();
    void SaveSettings();

    QIcon ColorIcon(const QSize &size, const QColor &color);

    QSettings *m_settings;

    QLineEdit *m_csdir;

    QFontComboBox   *m_fontPicker;
    QSpinBox        *m_fontSize;
    QPushButton     *m_fontColorButton;
    QColor           m_fontColor;

    QSpinBox *m_autosaves;
    QSpinBox *m_undos;
    QCheckBox *m_history;

signals:
    FontChanged(const QFont &);
    FontColorChanged(const QColor &);


private slots:
    void CSDirChanged(const QString &);
    void CSDirButtonPressed();

    void AutosavesChanged(int);
    void UndosChanged(int);
    void StoreHistoryChanged(int);

    void FontColorButtonPressed();

    void FontChanged();
    void FontColorSelected(const QColor &);
};

#endif // CSETTINGSVIEW_H
