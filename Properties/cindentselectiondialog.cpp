#include "cindentselectiondialog.h"

#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

CIndentSelectionDialog::CIndentSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Indent style picker");

    QMenu *types = new QMenu();
    types->addAction("Tabs", this, SLOT(TabSelected()));
    types->addAction("Spaces", this, SLOT(SpaceSelected()));

    m_type = new QPushButton("Spaces", this);
    m_type->setMenu(types);

    m_count = new QSpinBox();
    m_count->setRange(1, 100);
    m_count->setValue(4);
    connect(m_count, SIGNAL(valueChanged(int)), this, SLOT(IndentChanged(int)));

    QHBoxLayout *hl_indent = new QHBoxLayout();
    hl_indent->addWidget(new QLabel("Indent style: "));
    hl_indent->addWidget(m_count);
    hl_indent->addWidget(m_type);
    hl_indent->addStretch(1);

    QPushButton *ok = new QPushButton("Ok");
    connect(ok, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *hl_ok = new QHBoxLayout();
    hl_ok->addStretch(1);
    hl_ok->addWidget(ok);

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addLayout(hl_indent);
    vl_main->addLayout(hl_ok);

//    QFormLayout *fl_main = new QFormLayout(this);
//    fl_main->addRow("Type:", m_type);
//    fl_main->addRow("count:", m_count);


    m_indent.type = ' ';
    m_indent.count = 4;
}

Chronicler::CSIndent CIndentSelectionDialog::getIndent()
{
    return m_indent;
}

void CIndentSelectionDialog::TabSelected()
{
    m_type->setText("Tabs");
    m_indent.type = '\t';
}

void CIndentSelectionDialog::SpaceSelected()
{
    m_type->setText("Spaces");
    m_indent.type = ' ';
}

void CIndentSelectionDialog::IndentChanged(int count)
{
    m_indent.count = count;
}
