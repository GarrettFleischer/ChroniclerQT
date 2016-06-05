#include "cactionbubble.h"

#include <QFontMetrics>
#include <QRectF>
#include "Connections/cconnection.h"
#include "Misc/ctextitem.h"
#include "Misc/cstringlistmodel.h"

#include "Misc/cpaletteaction.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CActionBubble::CActionBubble(t_uid uid, const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(uid, pos, palette, font, parent)
{
    m_type = Chronicler::Action;

    m_actionsView = new CTextItem("", QRectF(), this);
    m_actionsView->SetStyle(Qt::AlignAbsolute | Qt::AlignVCenter);

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

    AdjustMinSize();
    m_bounds = QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width(), m_minSize.height());
    UpdatePolygon();

    setPalette(m_palette);
}

void CActionBubble::UpdatePolygon()
{
    CBubble::UpdatePolygon();
    QRectF b = boundingRect();
    const int off = 10;
    m_actionsView->Resize(QRectF(b.x() + off, b.y() + off, b.width() - off * 2, b.height() - off));
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

void CActionBubble::setPalette(CPaletteAction *palette)
{
    CBubble::setPalette(palette);
    m_actionsView->setColor(palette->getPalette().font);
}

void CActionBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    AdjustMinSize();
    UpdatePolygon();
}

QDataStream &CActionBubble::Deserialize(QDataStream &ds, const QString &version)
{
    CSingleLinkBubble::Deserialize(ds, version);

    QStringList actions;
    ds >> actions;

    m_actions->setStringList(actions);

    setPalette(m_palette);

    return ds;
}

QDataStream & CActionBubble::Serialize(QDataStream &ds)
{
    CSingleLinkBubble::Serialize(ds);

    ds << m_actions->stringList();

    return ds;
}

CStringListModel *CActionBubble::actions()
{
    return m_actions;
}

QString CActionBubble::actionString()
{
    QString a;
    QStringList lst = m_actions->stringList();

    for(int i = 0; i < lst.length() - 1; ++i)
        a += lst[i] + "\n";

    a += lst.last();

    return a;
}

void CActionBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);
    m_minSize.setHeight(m_actionsView->textBounds(QSizeF(m_minSize.width(), fm.height())).height() + 30);
}


