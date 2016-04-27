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
    QLineEdit *le_dir = new QLineEdit();
    le_dir->setMaximumWidth(500);
    connect(le_dir, SIGNAL(textChanged()), this, SLOT(CSDirChanged(QString)));

    // File picker button
    QPushButton *pb_dir = new QPushButton(QIcon(":/images/floodfill.png"), "");

    QHBoxLayout *hl_dir = new QHBoxLayout();
    hl_dir->addWidget(le_dir);
    hl_dir->addWidget(pb_dir);
    hl_dir->addStretch(1);


    // Add Rows
    fl_cs->addRow("ChoiceScript directory", hl_dir);

    // Load Settings
    le_dir->setText(m_settings->value("Editor/CSDir", "").toString());
}

void CSettingsView::SetupEditor(QLayout *main_layout)
{

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
