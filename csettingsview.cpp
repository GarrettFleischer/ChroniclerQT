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

    SetupChoiceScript(vl_main);
    SetupEditor(vl_main);
    SetupHistory(vl_main);

    vl_main->addStretch(1);
}

void CSettingsView::SetupChoiceScript(QLayout *main_layout)
{
    // ChoiceScript Group box
    QGroupBox *gb_cs = new QGroupBox("ChoiceScript");
    main_layout->addWidget(gb_cs);
    QFormLayout *fl_cs = new QFormLayout(gb_cs);


    // ChoiceScript directory
    m_csdir = new QLineEdit();
    //m_csdir->setMaximumWidth(500);
    connect(m_csdir, SIGNAL(textChanged(QString)), this, SLOT(CSDirChanged(QString)));

    // File picker button
    QPushButton *pb_dir = new QPushButton(QIcon(":/images/floodfill.png"), "");
    connect(pb_dir, SIGNAL(clicked(bool)), this, SLOT(CSDirButtonPressed()));

    QHBoxLayout *hl_dir = new QHBoxLayout();
    hl_dir->addWidget(m_csdir, 1);
    hl_dir->addWidget(pb_dir);
    hl_dir->addStretch(2);


    // Add Rows
    fl_cs->addRow("ChoiceScript directory", hl_dir);

    // Load Settings
    m_csdir->setText(m_settings->value("Editor/CSDir").toString());
}

void CSettingsView::SetupEditor(QLayout *main_layout)
{
    // Editor Group box
    QGroupBox *gb_editor = new QGroupBox("Editor");
    main_layout->addWidget(gb_editor);
    QFormLayout *fl_editor = new QFormLayout(gb_editor);


    // Font settings
    m_fontPicker = new QFontComboBox();
    connect(m_fontPicker, SIGNAL(currentFontChanged(QFont)),
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
    hl_font->addWidget(m_fontPicker, 1);
    hl_font->addWidget(m_fontSize);
    hl_font->addWidget(m_fontColorButton);
    hl_font->addStretch(4);

    // Add rows
    fl_editor->addRow("Font", hl_font);
    fl_editor->addRow("Theme", new QCheckBox("dark"));

    // Load settings
    QFont font = static_cast<QFont>(m_settings->value("Editor/Font", QVariant::fromValue(QFont("Times New Roman", 11))).value<QFont>());
    m_fontSize->setValue(font.pointSize());
    m_fontPicker->setCurrentFont(font);

    FontColorSelected(static_cast<QColor>(m_settings->value("Editor/FontColor", QVariant::fromValue(QColor(Qt::black))).value<QColor>()));
}

void CSettingsView::SetupHistory(QLayout *main_layout)
{
    // History Group box
    QGroupBox *gb_history = new QGroupBox("History");
    main_layout->addWidget(gb_history);
    QFormLayout *fl_history = new QFormLayout(gb_history);


    // Max Autosaves
    QHBoxLayout *hl_autosaves = new QHBoxLayout();

    QSpinBox *sb_autosaves = new QSpinBox();
    sb_autosaves->setRange(0, 100);
    connect(sb_autosaves, SIGNAL(valueChanged(int)),
            this, SLOT(AutosavesChanged(int)));
    hl_autosaves->addWidget(sb_autosaves,0, Qt::AlignLeft);
    hl_autosaves->addStretch(1);

    // Max Undos
    QHBoxLayout *hl_undos = new QHBoxLayout();

    QSpinBox *sb_undos = new QSpinBox();
    sb_undos->setRange(0, 500);
    connect(sb_undos, SIGNAL(valueChanged(int)),
            this, SLOT(UndosChanged(int)));
    hl_undos->addWidget(sb_undos, 0, Qt::AlignLeft);

    QCheckBox *cb_history = new QCheckBox("store history in project");
    connect(cb_history, SIGNAL(stateChanged(int)),
            this, SLOT(StoreHistoryChanged(int)));
    hl_undos->addWidget(cb_history, 0, Qt::AlignLeft);
    hl_undos->addStretch(1);


    // Add rows
    fl_history->addRow("max autosaves", hl_autosaves);
    fl_history->addRow("max undos", hl_undos);


    // Load settings
    sb_autosaves->setValue(m_settings->value("Editor/MaxAutosaves", 5).toInt());
    sb_undos->setValue(m_settings->value("Editor/MaxUndos", 100).toInt());
    cb_history->setCheckState(static_cast<Qt::CheckState>(m_settings->value("Editor/StoreHistory", Qt::Unchecked).toInt()));
}

void CSettingsView::LoadSettings()
{

}

void CSettingsView::SaveSettings()
{

}

QIcon CSettingsView::ColorIcon(const QSize &size, const QColor &color)
{
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(QPoint(0, 0), size), color);

    return QIcon(pixmap);
}

void CSettingsView::CSDirChanged(const QString &dir)
{
    m_settings->setValue("Editor/CSDir", dir);
}

void CSettingsView::CSDirButtonPressed()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::DirectoryOnly);

    QDir dir(m_csdir->text());
    dir.cdUp();

    m_csdir->setText(fd.getExistingDirectory(0, "ChoiceScript directory", dir.path()));
}

void CSettingsView::AutosavesChanged(int maxAutosaves)
{
    m_settings->setValue("Editor/MaxAutosaves", maxAutosaves);
}

void CSettingsView::UndosChanged(int maxUndos)
{
    m_settings->setValue("Editor/MaxUndos", maxUndos);
}

void CSettingsView::StoreHistoryChanged(int state)
{
    m_settings->setValue("Editor/StoreHistory", state);
}

void CSettingsView::FontColorButtonPressed()
{
    QColorDialog *cd = new QColorDialog(this);
    cd->setCurrentColor(m_fontColor);

    connect(cd, SIGNAL(colorSelected(QColor)),
            this, SLOT(FontColorSelected(QColor)));

    cd->show();
}

void CSettingsView::FontChanged()
{
    QFont font = m_fontPicker->currentFont();
    font.setPointSize(m_fontSize->value());

    m_settings->setValue("Editor/Font", QVariant::fromValue(font));

    emit FontChanged(font);
}

void CSettingsView::FontColorSelected(const QColor &color)
{
    m_fontColor = color;
    m_fontColorButton->setIcon(ColorIcon(QSize(20, 20), color));

    m_settings->setValue("Editor/FontColor", QVariant::fromValue(color));

    emit FontColorChanged(color);
}
