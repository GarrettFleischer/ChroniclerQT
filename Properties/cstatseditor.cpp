#include "cstatseditor.h"

#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QVBoxLayout>
#include <QScrollArea>

#include "Misc/Stats/cstaticon.h"


CStatsEditor::CStatsEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new CStatIcon(Chronicler::TextStat, QPixmap(":/images/icn_trash.png"), tr("Text"), this));
    layout->addWidget(new CStatIcon(Chronicler::PercentStat, QPixmap(":/images/icn_trash.png"), tr("Percent"), this));
    layout->addWidget(new CStatIcon(Chronicler::OpposedPairStat, QPixmap(":/images/icn_trash.png"), tr("Opposed Pair"), this));
    layout->addWidget(new CStatIcon(Chronicler::CustomStat, QPixmap(":/images/icn_trash.png"), tr("Custom"), this));

    QScrollArea *area = new QScrollArea(this);
    area->setLayout(layout);
}


void CStatsEditor::mousePressEvent(QMouseEvent *event)
{
    CStatIcon *child = static_cast<CStatIcon *>(childAt(event->pos())->childAt(event->pos()));
    if (!child)
        return;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << int(child->type());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/dnd-stat_type", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(child->pixmap());
    drag->setHotSpot(event->pos() - child->pos());

    drag->exec(Qt::MoveAction);
}
