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

#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "Misc/cserializable.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QWheelEvent;
QT_END_NAMESPACE

class CConnection;
class CStartBubble;
class CLink;
class CLine;

#include "Bubbles/cbubble.h"
#include "Misc/chronicler.h"
using Chronicler::CPalette;
using Chronicler::BubbleType;
using Chronicler::Mode;

//! [0]
class CGraphicsScene : public QGraphicsScene, public CSerializable
{
    Q_OBJECT

public:
    explicit CGraphicsScene(bool create_start, const QString &name, QObject *parent = 0);

    void setFont(const QFont &font);
    QFont getFont() const { return m_font; }

    QString name();
    void setName(const QString &name);

    CLine * getLine();

    bool isRubberBandSelecting() const { return m_rubberBand; }

    CBubble *AddBubble(BubbleType type, const QPointF &pos, bool shift);
    void AddBubble(CBubble *bubble);
    CConnection *AddConnection(CBubble *start, CBubble *end, Chronicler::Anchor start_anchor, Chronicler::Anchor end_anchor);
    CConnection *AddConnection();

    void RemoveBubble(CBubble *bubble);
    void RemoveConnection(CConnection *connection);

    CStartBubble * startBubble();
    QList<CBubble *> bubbles();
    QList<CConnection *> connections();

    CBubble *BubbleAt(const QPointF &point, bool choiceAllowed = false);

    virtual QDataStream & Serialize(QDataStream &ds) override;
    virtual QDataStream & Deserialize(QDataStream &ds, const CVersion &version) override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    QString m_name;
    QPointF m_startPoint;
    CLine *m_line;
    QFont m_font;
    CPalette m_palette;
    bool m_rubberBand;

    CStartBubble *m_startBubble;

    QList<CBubble *> m_bubbles;
    QList<CConnection *> m_connections;

signals:
    void itemInserted(CBubble *item);
    void itemSelected(QGraphicsItem *item);
    void leftPressed();
    void leftReleased();
    void nameChanged();

public slots:
    void SelectAll();

private slots:
    void ItemSelected(QGraphicsItem *selectedItem);

    void UpdateSceneRect();
};

#endif // CGRAPHICSSCENE_H
