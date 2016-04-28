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
#include <QToolButton>

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
    // ChoiceScript Group box
    QGroupBox *gb_editor = new QGroupBox("Editor");
    main_layout->addWidget(gb_editor);
    QFormLayout *fl_editor = new QFormLayout(gb_editor);


    QFontComboBox *fcb_font = new QFontComboBox();
    //    connect(fcb_editor, SIGNAL(currentFontChanged(QFont)),
    //            this, SLOT(CurrentFontChanged(QFont)));
    fcb_font->setCurrentText("Times New Roman");

    QSpinBox *sb_font_size = new QSpinBox();
    sb_font_size->setRange(8, 42);
    sb_font_size->setValue(11);
    //    connect(m_fontSizeCombo, SIGNAL(valueChanged(QString)),
    //            this, SLOT(FontSizeChanged(QString)));



    QToolButton *tb_font_color = new QToolButton;
    tb_font_color->setPopupMode(QToolButton::MenuButtonPopup);
    //tb_font_color->setMenu(CreateColorMenu(SLOT(TextColorChanged()), Qt::black));
    //m_textAction = m_fontColorToolButton->menu()->defaultAction();
    //tb_font_color->setIcon(CreateColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    tb_font_color->setAutoFillBackground(true);
    //    connect(m_fontColorToolButton, SIGNAL(clicked()),
    //            this, SLOT(TextButtonTriggered()));

    QHBoxLayout *hl_font = new QHBoxLayout();
    hl_font->addWidget(fcb_font, 1);
    hl_font->addWidget(sb_font_size);
    hl_font->addWidget(tb_font_color);
    hl_font->addStretch(4);

    fl_editor->addRow("Font", hl_font);

    //    m_boldAction = new QAction(tr("Bold"), this);
    //    m_boldAction->setCheckable(true);
    //    QPixmap pixmap(":/images/bold.png");
    //    m_boldAction->setIcon(QIcon(pixmap));
    //    m_boldAction->setShortcut(tr("Ctrl+B"));
    //    connect(m_boldAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    //    m_italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    //    m_italicAction->setCheckable(true);
    //    m_italicAction->setShortcut(tr("Ctrl+I"));
    //    connect(m_italicAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    //    m_underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    //    m_underlineAction->setCheckable(true);
    //    m_underlineAction->setShortcut(tr("Ctrl+U"));
    //    connect(m_underlineAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));
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

void CSettingsView::CSDirChanged(QString dir)
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
