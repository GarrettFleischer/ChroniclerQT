#include "cchoicebubble.h"

#include "Misc/cstringlistmodel.h"
#include "Misc/ctextitem.h"

CChoiceBubble::CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Choice;

    m_palette.fill = QColor(151,118,166);


    m_modelView = new CTextItem("", QRectF(), this);
    m_modelView->SetStyle(Qt::AlignCenter);

    QStringList test;
    test.append("Code");
    test.append("Eat");
    test.append("Sleep");
    test.append("In that order...");

    m_choices = new CStringListModel(this);
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

    m_choices->setStringList(test);

    AdjustMinSize();
    m_bounds = QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width(), m_minSize.height());
    UpdatePolygon();
    //setPolygon(QPolygonF(m_bounds));
}

void CChoiceBubble::setPalette(const Chronicler::CPalette &palette)
{
    CBubble::setPalette(palette);
    m_modelView->setColor(palette.font);
}

void CChoiceBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    AdjustMinSize();
    UpdatePolygon();
}


void CChoiceBubble::AddLink(CConnection *link)
{

}

void CChoiceBubble::RemoveLink(CConnection *link)
{

}

QList<CConnection *> CChoiceBubble::links()
{
    return {};
}

CStringListModel *CChoiceBubble::choices()
{
    return m_choices;
}

void CChoiceBubble::UpdatePolygon()
{
    CBubble::UpdatePolygon();
    m_modelView->Resize(boundingRect());
}

void CChoiceBubble::AdjustMinSize()
{
    m_minSize.setHeight(m_modelView->textBounds(m_minSize).height());
}

void CChoiceBubble::ModelUpdated()
{
    QString txt;
    for(QString action : m_choices->stringList())
        txt += action + '\n';

    m_modelView->setText(txt);
    AdjustMinSize();
    UpdatePolygon();
}
