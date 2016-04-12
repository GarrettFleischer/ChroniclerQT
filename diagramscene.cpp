/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "diagramscene.h"
#include "Connections/arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Connections/clink.h"
#include "Connections/cline.h"


DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = Cursor;
    line = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
    m_rubberBand = false;

    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));
}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetLineColor(myLineColor);
    }
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetFontColor(myTextColor);
    }
}

void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetColor(myItemColor);
    }
}

void DiagramScene::setFont(const QFont &font)
{
    if(myFont != font)
    {
        myFont = font;

        foreach (QGraphicsItem *item, items())
        {
            CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
            if(bbl)
                bbl->SetFont(myFont);
        }
    }
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::linkClicked(CLink *)
{
    setMode(InsertLine);
    
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        CBubble *item;
        switch (myMode) {
        case InsertStory:
            item = new CStoryBubble(myItemMenu);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            item->SetFont(myFont);
            item->SetLineColor(myLineColor);
            item->SetFontColor(myTextColor);
            connect(item, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
            emit itemInserted(item);
            break;

        case InsertChoice:

            break;

        case InsertCondition:
            item = new CConditionBubble(myItemMenu);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            item->SetFont(myFont);
            item->SetLineColor(myLineColor);
            item->SetFontColor(myTextColor);
            connect(item, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
            emit itemInserted(item);
            break;

        case InsertLine:
            line = new CLine(this);
            line->setStart(mouseEvent->scenePos());
            line->setEnd(QPointF(mouseEvent->scenePos().x() + 200, mouseEvent->scenePos().y() + 200));
            addItem(line);
            break;

        default:
            break;
        }

        QGraphicsScene::mousePressEvent(mouseEvent);

        if(selectedItems().size() == 0)
            m_rubberBand = true;

        emit leftPressed();
    }
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0)
    {
        //QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        //line->setLine(newLine);
        line->setEnd(mouseEvent->scenePos());
    } else if (myMode == Cursor) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if (line != 0 && myMode == InsertLine)
//    {
//        QList<QGraphicsItem *> startItems = items(line->line().p1());
//        if (startItems.count() && startItems.first() == line)
//            startItems.removeFirst();
//        QList<QGraphicsItem *> endItems = items(line->line().p2());
//        if (endItems.count() && endItems.first() == line)
//            endItems.removeFirst();

//        removeItem(line);
//        delete line;

//        if (startItems.count() > 0 && endItems.count() > 0 &&
//                startItems.first() != endItems.first())
//        {
//            CBubble *startItem = qgraphicsitem_cast<CBubble *>(startItems.first());
//            CBubble *endItem = qgraphicsitem_cast<CBubble *>(endItems.first());
//            if(startItem && endItem)
//            {
//                Arrow *arrow = new Arrow(startItem, endItem);
//                arrow->setColor(myLineColor);
//                startItem->addArrow(arrow);
//                endItem->addArrow(arrow);
//                arrow->setZValue(-1000.0);
//                addItem(arrow);
//                arrow->updatePosition();
//            }
//        }
//    }

    //delete line;
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    m_rubberBand = false;

    emit leftReleased();
}

//void DiagramScene::wheelEvent(QGraphicsSceneWheelEvent *event)
//{
//    emit mouseScrolled(event->delta());
//}
