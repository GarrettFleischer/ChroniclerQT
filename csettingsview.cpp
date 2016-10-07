#include "csettingsview.h"

#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QFontComboBox>
#include <QSpinBox>
#include <QPalette>
#include <QBrush>
#include <QColorDialog>
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>

CSettingsView::CSettingsView(QSettings *settings, QWidget *parent)
    : QWidget(parent), m_settings(settings)
{
    QVBoxLayout *vl_main = new QVBoxLayout(this);

    m_apply = new QPushButton("Apply");
    connect(m_apply, SIGNAL(clicked(bool)),
            this, SLOT(SettingsApplied()));
    m_cancel = new QPushButton("Cancel");
    connect(m_cancel, SIGNAL(clicked(bool)),
            this, SLOT(SettingsCanceled()));

    SetupChoiceScript(vl_main);
    SetupEditor(vl_main);
    SetupHistory(vl_main);

    LoadSettings();
    MarkChanged(false);

    QHBoxLayout *hl_apply = new QHBoxLayout();
    hl_apply->addStretch(1);
    hl_apply->addWidget(m_apply);
    hl_apply->addWidget(m_cancel);

    vl_main->addLayout(hl_apply);
    vl_main->addStretch(1);
}

QSettings *CSettingsView::settings()
{
    return m_settings;
}

QString CSettingsView::choiceScriptDirectory()
{
    return m_csdir->text();
}

QFont CSettingsView::font()
{
    return m_font;
}

QColor CSettingsView::fontColor()
{
    return m_fontColor;
}

int CSettingsView::maxAutosaves()
{
    return m_autosaves->value();
}

int CSettingsView::autosaveInterval()
{
    return m_autosave_interval->value() * 60000;
}

int CSettingsView::maxUndos()
{
    return m_undos->value();
}

bool CSettingsView::storeHistoryInProject()
{
    return m_history->isChecked();
}

int CSettingsView::maxRecentFiles()
{
    return m_recent_files->value();
}


bool CSettingsView::pendingChanges()
{
    return m_apply->isEnabled();
}

void CSettingsView::ApplyPendingChanges()
{
    SettingsApplied();
}

void CSettingsView::SetupChoiceScript(QLayout *main_layout)
{
    // ChoiceScript Group box
    QGroupBox *gb_cs = new QGroupBox("ChoiceScript");
    main_layout->addWidget(gb_cs);
    QFormLayout *fl_cs = new QFormLayout(gb_cs);


    // ChoiceScript directory
    m_csdir = new QLineEdit();
    m_csdir->setReadOnly(true);
    connect(m_csdir, SIGNAL(textChanged(QString)),
            this, SLOT(SettingChanged()));

    // File picker button
    QPushButton *pb_dir = new QPushButton(QIcon(":/images/icn_load.png"), "");
    connect(pb_dir, SIGNAL(clicked(bool)),
            this, SLOT(CSDirButtonPressed()));

    QHBoxLayout *hl_dir = new QHBoxLayout();
    hl_dir->addWidget(m_csdir, 1);
    hl_dir->addWidget(pb_dir);
    hl_dir->addStretch(2);


    // Add Rows
    fl_cs->addRow("ChoiceScript directory", hl_dir);
}

void CSettingsView::SetupEditor(QLayout *main_layout)
{
    // Editor Group box
    QGroupBox *gb_editor = new QGroupBox("Editor");
    main_layout->addWidget(gb_editor);
    QFormLayout *fl_editor = new QFormLayout(gb_editor);


    // Font settings
    m_fontCombo = new QFontComboBox();
    connect(m_fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(FontChanged()));

    m_fontSize = new QSpinBox();
    m_fontSize->setRange(8, 42);
    connect(m_fontSize, SIGNAL(valueChanged(int)),
            this, SLOT(FontChanged()));

    m_fontColorButton = new QPushButton();
    m_fontColorButton->setMaximumSize(24, 24);
    connect(m_fontColorButton, SIGNAL(clicked(bool)),
            this, SLOT(FontColorButtonPressed()));

    // Font layout
    QHBoxLayout *hl_font = new QHBoxLayout();
    hl_font->addWidget(m_fontCombo, 1);
    hl_font->addWidget(m_fontSize);
    hl_font->addWidget(m_fontColorButton);
    hl_font->addStretch(4);

    // Add rows
    fl_editor->addRow("Font", hl_font);
    //    fl_editor->addRow("Theme", new QCheckBox("dark"));
}

void CSettingsView::SetupHistory(QLayout *main_layout)
{
    // History Group box
    QGroupBox *gb_history = new QGroupBox("History");
    main_layout->addWidget(gb_history);
    QFormLayout *fl_history = new QFormLayout(gb_history);


    // Max Autosaves
    QHBoxLayout *hl_autosaves = new QHBoxLayout();

    m_autosaves = new QSpinBox();
    m_autosaves->setRange(0, 100);
    connect(m_autosaves, SIGNAL(valueChanged(int)),
            this, SLOT(SettingChanged()));
    hl_autosaves->addWidget(m_autosaves, 0, Qt::AlignLeft);
    hl_autosaves->addStretch(1);

    // Autosave interval
    QHBoxLayout *hl_autosaves_interval = new QHBoxLayout();

    m_autosave_interval = new QSpinBox();
    m_autosave_interval->setRange(1, 60);
    connect(m_autosave_interval, SIGNAL(valueChanged(int)),
            this, SLOT(SettingChanged()));
    hl_autosaves_interval->addWidget(m_autosave_interval, 0, Qt::AlignLeft);
    hl_autosaves_interval->addWidget(new QLabel(" minutes", this));
    hl_autosaves_interval->addStretch(1);

    // Max Undos
    QHBoxLayout *hl_undos = new QHBoxLayout();

    m_undos = new QSpinBox();
    m_undos->setRange(0, 500);
    connect(m_undos, SIGNAL(valueChanged(int)),
            this, SLOT(SettingChanged()));
    hl_undos->addWidget(m_undos, 0, Qt::AlignLeft);

    m_history = new QCheckBox("store history in project");
    m_history->setEnabled(false);
    connect(m_history, SIGNAL(stateChanged(int)),
            this, SLOT(SettingChanged()));
    hl_undos->addWidget(m_history, 0, Qt::AlignLeft);
    hl_undos->addStretch(1);

    // Max Recent Files
    QHBoxLayout *hl_recent = new QHBoxLayout();
    m_recent_files = new QSpinBox();
    m_recent_files->setRange(0, 100);
    connect(m_recent_files, SIGNAL(valueChanged(int)),
            this, SLOT(SettingChanged()));
    hl_recent->addWidget(m_recent_files);
    hl_recent->addStretch(1);

    // Add rows
    fl_history->addRow("max autosaves", hl_autosaves);
    fl_history->addRow("autosave interval", hl_autosaves_interval);
    fl_history->addRow("max undos", hl_undos);
    fl_history->addRow("max recent files", hl_recent);
}

void CSettingsView::LoadSettings()
{
    // Load Choicescript
    m_csdir->setText(m_settings->value("Editor/CSDir").toString());

    // Load Editor
    QFont font = m_settings->value("Editor/Font", QVariant::fromValue(QFont("Arial", 10))).value<QFont>();
    m_fontSize->setValue(font.pointSize());
    m_fontCombo->setCurrentFont(font);
    FontColorSelected(m_settings->value("Editor/FontColor", QVariant::fromValue(QColor(Qt::black))).value<QColor>());

    // Load History
    m_autosaves->setValue(m_settings->value("Editor/MaxAutosaves", 5).toInt());
    m_autosave_interval->setValue(m_settings->value("Editor/AutosaveInterval", 5).toInt());
    m_undos->setValue(m_settings->value("Editor/MaxUndos", 100).toInt());
    m_history->setCheckState(static_cast<Qt::CheckState>(m_settings->value("Editor/StoreHistory", Qt::Unchecked).toInt()));
    m_recent_files->setValue(m_settings->value("Editor/MaxRecentFiles", 10).toInt());
}

void CSettingsView::SaveSettings()
{
    // Save Choicescript
    m_settings->setValue("Editor/CSDir", choiceScriptDirectory());

    // Save Editor
    m_settings->setValue("Editor/Font", QVariant::fromValue(font())); // includes point size
    m_settings->setValue("Editor/FontColor", QVariant::fromValue(fontColor()));

    // Save History
    m_settings->setValue("Editor/MaxAutosaves", maxAutosaves());
    m_settings->setValue("Editor/AutosaveInterval", m_autosave_interval->value());
    m_settings->setValue("Editor/MaxUndos", maxUndos());
    m_settings->setValue("Editor/StoreHistory", static_cast<int>(m_history->checkState()));
    m_settings->setValue("Editor/MaxRecentFiles", maxRecentFiles());
}

void CSettingsView::MarkChanged(bool changed)
{
    if(changed)
    {
        m_apply->setEnabled(true);
        m_cancel->setEnabled(true);
    }
    else
    {
        m_apply->setEnabled(false);
        m_cancel->setEnabled(false);
    }
}

QIcon CSettingsView::ColorIcon(const QSize &size, const QColor &color)
{
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(QPoint(0, 0), size), color);

    return QIcon(pixmap);
}



static bool cpDir(const QString &srcPath, const QString &dstPath)
{
    //    rmDir(dstPath);
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName()))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
        }
    }
    return true;
}

void CSettingsView::CSDirButtonPressed()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::DirectoryOnly);

    QDir dir(m_csdir->text());
    dir.cdUp();

    QString new_dir = fd.getExistingDirectory(0, "ChoiceScript directory", dir.path());
    if(new_dir.length())
    {
        if(QDir(new_dir + "/web").exists())
        {
            // update the directory and backup the web folder
            m_csdir->setText(new_dir);
            if(!QDir(new_dir + "/web_backup").exists())
                cpDir(new_dir + "/web", new_dir + "/web_backup");
        }
        else
        {
            m_csdir->setText("");
            QMessageBox msg;
            msg.setText("Invalid directory.");
            msg.setInformativeText("The ChoiceScript directory must contain a \"web\" folder");
            msg.exec();
        }
    }
}

void CSettingsView::FontColorButtonPressed()
{
    QColorDialog *cd = new QColorDialog(this);
    cd->setCurrentColor(m_fontColor);

    connect(cd, SIGNAL(colorSelected(QColor)),
            this, SLOT(FontColorSelected(QColor)));

    cd->show();
}

void CSettingsView::SettingsApplied()
{
    SaveSettings();
    MarkChanged(false);

    emit SettingsChanged();
}

void CSettingsView::SettingsCanceled()
{
    LoadSettings();
    MarkChanged(false);

    emit SettingsChanged();
}

void CSettingsView::SettingChanged()
{
    MarkChanged(true);
}

void CSettingsView::FontChanged()
{
    m_font = m_fontCombo->currentFont();
    m_font.setPointSize(m_fontSize->value());

    SettingChanged();
}

void CSettingsView::FontColorSelected(const QColor &color)
{
    m_fontColor = color;
    m_fontColorButton->setIcon(ColorIcon(QSize(20, 20), color));

    SettingChanged();
}
