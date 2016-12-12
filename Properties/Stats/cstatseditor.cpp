#include "cstatseditor.h"

#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QVBoxLayout>
#include <QScrollArea>

#include "Misc/Stats/cstaticon.h"
#include "Misc/chronicler.h"
using Chronicler::t_uid;

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
    QWidget *child = childAt(event->pos());
    if (!child)
        return;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << reinterpret_cast<t_uid>(child);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/dnd-stat_pointer", itemData);

    QPixmap pixmap;
    child->render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());

    drag->exec(Qt::MoveAction);
}
