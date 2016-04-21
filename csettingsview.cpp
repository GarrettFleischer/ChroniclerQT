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


CSettingsView::CSettingsView(QSettings *settings, QWidget *parent)
    : QWidget(parent), m_settings(settings)
{
    QVBoxLayout *vl_main = new QVBoxLayout(this);

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


    QGroupBox *gb_history = new QGroupBox("History");
    QFormLayout *fl_history = new QFormLayout(gb_history);
    fl_history->addRow("max autosaves", hl_autosaves);
    fl_history->addRow("max undos", hl_undos);


    //vl_main->addLayout(hl_undos);
    vl_main->addWidget(gb_history);
    vl_main->addStretch(1);


    // Load settings
    sb_autosaves->setValue(settings->value("Editor/MaxAutosaves", 5).toInt());
    sb_undos->setValue(settings->value("Editor/MaxUndos", 100).toInt());
    cb_history->setCheckState(static_cast<Qt::CheckState>(settings->value("Editor/StoreHistory", Qt::Unchecked).toInt()));
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
