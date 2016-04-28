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
//#include <QToolButton>

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

int CSettingsView::maxUndos()
{
    return m_undos->value();
}

bool CSettingsView::storeHistoryInProject()
{
    return m_history->isChecked();
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
    connect(m_csdir, SIGNAL(textChanged(QString)),
            this, SLOT(CSDirChanged()));

    // File picker button
    QPushButton *pb_dir = new QPushButton(QIcon(":/images/floodfill.png"), "");
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
    fl_editor->addRow("Theme", new QCheckBox("dark"));
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
            this, SLOT(AutosavesChanged()));
    hl_autosaves->addWidget(m_autosaves,0, Qt::AlignLeft);
    hl_autosaves->addStretch(1);

    // Max Undos
    QHBoxLayout *hl_undos = new QHBoxLayout();

    m_undos = new QSpinBox();
    m_undos->setRange(0, 500);
    connect(m_undos, SIGNAL(valueChanged(int)),
            this, SLOT(UndosChanged()));
    hl_undos->addWidget(m_undos, 0, Qt::AlignLeft);

    m_history = new QCheckBox("store history in project");
    connect(m_history, SIGNAL(stateChanged(int)),
            this, SLOT(StoreHistoryChanged()));
    hl_undos->addWidget(m_history, 0, Qt::AlignLeft);
    hl_undos->addStretch(1);


    // Add rows
    fl_history->addRow("max autosaves", hl_autosaves);
    fl_history->addRow("max undos", hl_undos);
}

void CSettingsView::LoadSettings()
{
    // Load Choicescript
    m_csdir->setText(m_settings->value("Editor/CSDir").toString());

    // Load Editor
    QFont font = static_cast<QFont>(m_settings->value("Editor/Font", QVariant::fromValue(QFont("Arial", 10))).value<QFont>());
    m_fontSize->setValue(font.pointSize());
    m_fontCombo->setCurrentFont(font);
    FontColorSelected(static_cast<QColor>(m_settings->value("Editor/FontColor", QVariant::fromValue(QColor(Qt::black))).value<QColor>()));

    // Load History
    m_autosaves->setValue(m_settings->value("Editor/MaxAutosaves", 5).toInt());
    m_undos->setValue(m_settings->value("Editor/MaxUndos", 100).toInt());
    m_history->setCheckState(static_cast<Qt::CheckState>(m_settings->value("Editor/StoreHistory", Qt::Unchecked).toInt()));
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
    m_settings->setValue("Editor/MaxUndos", maxUndos());
    m_settings->setValue("Editor/StoreHistory", static_cast<int>(m_history->checkState()));
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

void CSettingsView::CSDirChanged()
{
    MarkChanged(true);
}

void CSettingsView::CSDirButtonPressed()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::DirectoryOnly);

    QDir dir(m_csdir->text());
    dir.cdUp();

    m_csdir->setText(fd.getExistingDirectory(0, "ChoiceScript directory", dir.path()));
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

void CSettingsView::AutosavesChanged()
{
    MarkChanged(true);
}

void CSettingsView::UndosChanged()
{
    MarkChanged(true);
}

void CSettingsView::StoreHistoryChanged()
{
    MarkChanged(true);
}

void CSettingsView::FontChanged()
{
    m_font = m_fontCombo->currentFont();
    m_font.setPointSize(m_fontSize->value());

    MarkChanged(true);
}

void CSettingsView::FontColorSelected(const QColor &color)
{
    m_fontColor = color;
    m_fontColorButton->setIcon(ColorIcon(QSize(20, 20), color));

    MarkChanged(true);
}