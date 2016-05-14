#include "cactionbubble.h"

#include <QFontMetrics>
#include <QRectF>
#include "Connections/cconnection.h"
#include "Misc/ctextitem.h"
#include "Misc/cstringlistmodel.h"

CActionBubble::CActionBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Action;

    m_palette.fill = QColor(161,88,136);

    m_actionsView = new CTextItem("", QRectF(), this);
    m_actionsView->SetStyle(Qt::AlignAbsolute | Qt::AlignVCenter);

    QStringList test;
    test.append("Action 1");
    test.append("Action 2");
    test.append("Action 3");
    test.append("Action 4");

    m_actions = new CStringListModel(this);
    connect(m_actions, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(ModelUpdated()));
    connect(m_actions, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(ModelUpdated()));
    connect(m_actions, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(ModelUpdated()));
    connect(m_actions, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this, SLOT(ModelUpdated()));
    connect(m_actions, SIGNAL(modelReset()),
            this, SLOT(ModelUpdated()));

    m_actions->setStringList(test);

    AdjustMinSize();
    m_bounds = QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width(), m_minSize.height());
    UpdatePolygon();
}

void CActionBubble::UpdatePolygon()
{
    CBubble::UpdatePolygon();
    QRectF b = boundingRect();
    const int off = 10;
    m_actionsView->Resize(QRectF(b.x() + off, b.y() + off, b.width() - off * 2, b.height() - off * 2));
}

void CActionBubble::ModelUpdated()
{
    QString txt;
    for(QString action : m_actions->stringList())
        txt += " " + action + '\n';

    m_actionsView->setText(txt);
    AdjustMinSize();
    UpdatePolygon();
}

void CActionBubble::setPalette(const Chronicler::CPalette &palette)
{
    CBubble::setPalette(palette);
    m_actionsView->setColor(palette.font);
}

void CActionBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    AdjustMinSize();
    UpdatePolygon();
}

CStringListModel *CActionBubble::actions()
{
    return m_actions;
}

void CActionBubble::AdjustMinSize()
{
    m_minSize.setHeight(m_actionsView->textBounds(QSizeF(100,100)).height());
}
