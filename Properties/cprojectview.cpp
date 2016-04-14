#include "cprojectview.h"

#include <QLabel>

CProjectView::CProjectView(QWidget *parent)
    : QWidget(parent)
{
    QLabel *lbl = new QLabel("Project View", this);
}
