#include "cchoicebubble.h"

#include <QtMath>
#include <QFontMetrics>

#include "Misc/cpaletteaction.h"

#include "Misc/cchoicemodel.h"
#include "cchoice.h"

CChoiceBubble::CChoiceBubble(t_uid uid, const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(uid, pos, palette, font, parent)
{
    m_type = Chronicler::Choice;

    m_choices = new CChoiceModel(choiceList(), this);
    connect(m_choices, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(ModelUpdated()));
    connect(m_choices, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(ModelUpdated()));
    connect(m_choices, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(ModelUpdated()));
    connect(m_choices, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this, SLOT(ModelUpdated()));
    connect(m_choices, SIGNAL(modelReset()),
            this, SLOT(ModelUpdated()));

    AdjustMinSize();
    m_bounds = QRectF(-m_minSize.width() / 2, -m_minSize.height() / 2, m_minSize.width(), m_minSize.height());
    UpdatePolygon();

    setPalette(m_palette);
}

void CChoiceBubble::setPalette(CPaletteAction *palette)
{
    CBubble::setPalette(palette);
    for(CChoice *choice : m_choices->choices())
        choice->setPalette(palette);
}

void CChoiceBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    AdjustMinSize();
    UpdatePolygon();
}


// Links are not attached directly, but rather to the CChoice sub-bubbles.
void CChoiceBubble::AddLink(CConnection *)
{}
void CChoiceBubble::RemoveLink(CConnection *)
{}
void CChoiceBubble::RemoveLink(Chronicler::Anchor)
{}

QList<CConnection *> CChoiceBubble::links()
{
    QList<CConnection *> links;

    for(CChoice *choice : m_choices->choices())
        links.append(choice->links());

    return links;
}

CChoiceModel *CChoiceBubble::choices()
{
    return m_choices;
}

CChoiceBubble::choiceList CChoiceBubble::choiceBubbles()
{
    return m_choices->choices();
}

void CChoiceBubble::UpdatePolygon()
{
    m_bounds.setHeight(qMax(m_bounds.height(), m_minSize.height()));
    CBubble::UpdatePolygon();

    choiceList choices = m_choices->choices();

    qreal height = 0;
    if(choices.length() > 0)
        height = boundingRect().center().y() - (choices.length() * (choices[0]->boundingRect().height() + 2)) / 2;

    for(CChoice *choice : choices)
    {
        choice->setWidth(boundingRect().width() - 5);
        choice->setPos(m_bounds.x() + 2, height);
        height += choice->boundingRect().height() + 2;
    }
}

void CChoiceBubble::AdjustMinSize()
{
    choiceList choices = m_choices->choices();

    qreal height = 0;
    if(choices.length() > 0)
        height = (choices.length() * (choices[0]->boundingRect().height() + 2));

    height += QFontMetrics(m_font).height() * 2;

    m_minSize.setHeight(height);
}

void CChoiceBubble::ModelUpdated()
{
    AdjustMinSize();
    UpdatePolygon();
    emit PositionOrShapeChanged();
}


Chronicler::Anchor CChoiceBubble::OutputAnchorAtPosition(const QPointF &)
{
    return Chronicler::None;
}

Chronicler::Anchor CChoiceBubble::InputAnchorAtPosition(const QPointF &pos)
{
    if(pos.y() > sceneBoundingRect().center().y())
        return Chronicler::Down;

    return Chronicler::Up;
}


QDataStream &CChoiceBubble::Read(QDataStream &ds, const QString &version)
{
    CBubble::Read(ds, version);

    qint64 num_choices;
    ds >> num_choices;

    for(int i = 0; i < num_choices; ++i)
    {
        CChoice *choice = new CChoice(0, m_palette, m_font, this);
        ds >> *choice;
        m_choices->AddItem(choice);
    }

    setPalette(m_palette);

    return ds;
}

QDataStream & CChoiceBubble::Write(QDataStream &ds)
{
    CBubble::Write(ds);

    QList<CChoice *> tmp = m_choices->choices();

    ds << static_cast<qint64>(tmp.length());
    for(CChoice *choice : tmp)
        ds << *choice;

    return ds;
}
