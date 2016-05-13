#include "cchoicebubble.h"

#include <QtMath>
#include <QFontMetrics>

#include "Misc/cchoicemodel.h"
#include "cchoice.h"

CChoiceBubble::CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Choice;

    m_palette.fill = QColor(151,118,166);

//    m_modelView = new CTextItem("", QRectF(), this);
//    m_modelView->SetStyle(Qt::AlignCenter);

//    QStringList test;
//    test.append("Code");
//    test.append("Eat");
//    test.append("Sleep");
//    test.append("In that order...");
    choiceList test;
    test.append(new CChoice(contextMenu, pos, palette, font, this, "Code"));
    test.append(new CChoice(contextMenu, pos, palette, font, this, "Eat"));
    test.append(new CChoice(contextMenu, pos, palette, font, this, "Sleep"));
    test.append(new CChoice(contextMenu, pos, palette, font, this, "In that order..."));

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

    m_choices->setChoices(test);

    AdjustMinSize();
    m_bounds = QRectF(0, 0, m_minSize.width(), m_minSize.height());
    UpdatePolygon();
}

void CChoiceBubble::setPalette(const Chronicler::CPalette &palette)
{
    CBubble::setPalette(palette);
//    m_modelView->setColor(palette.font);
}

void CChoiceBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    AdjustMinSize();
    UpdatePolygon();
}


void CChoiceBubble::AddLink(CConnection *)
{

}

void CChoiceBubble::RemoveLink(CConnection *)
{

}

QList<CConnection *> CChoiceBubble::links()
{
    return {};
}

CChoiceModel *CChoiceBubble::choices()
{
    return m_choices;
}

void CChoiceBubble::UpdatePolygon()
{
    CBubble::UpdatePolygon();

    choiceList choices = m_choices->choices();

    qreal height = 0;
    if(choices.length() > 0)
        height = boundingRect().center().y() - (choices.length() * choices[0]->boundingRect().height()) / 2;

    for(CChoice *choice : choices)
    {
        choice->setWidth(boundingRect().width() - 12);
        choice->setPos(5, height);
        height += choice->boundingRect().height();
    }
}

void CChoiceBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);

    choiceList choices = m_choices->choices();

    qreal height = fm.height();
    if(choices.length() > 0)
        height = choices[0]->boundingRect().height();

    m_minSize.setHeight(height * (1 + choices.length()));
}

void CChoiceBubble::ModelUpdated()
{
    choiceList choices = m_choices->choices();
    qreal height = 0;

    for(CChoice *choice : choices)
    {
        choice->setPos(0, height);
        height += choice->boundingRect().height();
    }

//    for(int i = 0; i < choices.length(); ++i)
//    {
//        if(i < choices.length())
//        {
//            m_choiceBubbles[i]->setChoice(choices[i]);
//            m_choiceBubbles[i]->setPos(0, height);
//            height += m_choiceBubbles[i]->boundingRect().height();
//        }
//        else
//        {
//            QPointF pos(10, height);
//            CChoice *c = new CChoice(m_contextMenu, pos, m_palette, m_font, this);
//            c->setChoice(choices[i]);
//            connect(this, SIGNAL(PositionChanged()), c, SIGNAL(PositionChanged()));
//            m_choiceBubbles.append(c);
//            height += c->boundingRect().height();
//        }
//    }

    AdjustMinSize();
    UpdatePolygon();
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
