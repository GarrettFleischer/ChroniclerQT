#include "cvariablesview.h"

#include <QTableView>
#include <QVBoxLayout>

CVariablesView::CVariablesView(QWidget *parent)
    : QWidget(parent)
{
    m_view = new QTableView(this);
}
