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

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Connections/clink.h"

//! [0]
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
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetLineColor(myLineColor);
    }
//    if (isItemChange(Arrow::Type)) {
//        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
//        item->setColor(myLineColor);
//        update();
//    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetFontColor(myTextColor);
    }

//    if (isItemChange(DiagramTextItem::Type)) {
//        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
//        item->setDefaultTextColor(myTextColor);
//    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetColor(myItemColor);
    }
//    if (isItemChange(DiagramItem::Type)) {
//        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
//        item->setBrush(myItemColor);
//    }
}
//! [3]

//! [4]
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
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::linkClicked(CLink *link)
{
    setMode(InsertLine);
    
}
//! [5]

//! [6]
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
                line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
                line->setPen(QPen(myLineColor, 2));
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
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == Cursor) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            //startItems.first()->type() == DiagramItem::Type &&
            //endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            CBubble *startItem = qgraphicsitem_cast<CBubble *>(startItems.first());
            CBubble *endItem = qgraphicsitem_cast<CBubble *>(endItems.first());
            if(startItem && endItem)
            {
                Arrow *arrow = new Arrow(startItem, endItem);
                arrow->setColor(myLineColor);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }
        }
    }
//! [12] //! [13]
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    m_rubberBand = false;

    emit leftReleased();
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
//! [14]
