#include "cpalettecreator.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QColorDialog>
#include <QLineEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QPushButton>

#include "Bubbles/cstorybubble.h"
#include "Misc/ccolorbutton.h"

#include "Misc/cpaletteaction.h"

#include "csettingsview.h"
using Chronicler::shared;

CPaletteCreator::CPaletteCreator(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Palette Creator");

    QFormLayout *fl_buttons = new QFormLayout();
    QHBoxLayout *hl_dialogButtons = new QHBoxLayout();
    QVBoxLayout *vl_buttonsDialog = new QVBoxLayout();
    QHBoxLayout *hl_viewButtons = new QHBoxLayout(this);

    QGraphicsScene *scene = new QGraphicsScene(0, 0, 450, 300);
    scene->setBackgroundBrush(QBrush(Qt::gray));
    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_bubble = new CStoryBubble(0, QPointF(0, 0), shared().defaultStory);
    m_bubble->setBounds(QRectF(0, 0, 200, 150));
    m_bubble->setLabel("Lorem");
    m_bubble->setStory("Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit.\nMauris id.");
    m_bubble->setFlag(QGraphicsItem::ItemIsMovable, false);
    m_bubble->setPos(scene->sceneRect().center() - QPointF(100, 75));

    scene->addItem(m_bubble);

    QSize s(32, 32);
    m_fillButton = new CColorButton(s, m_bubble->getPalette()->getPalette().fill);
    connect(m_fillButton, SIGNAL(colorChanged(QColor)), this, SLOT(PaletteChanged()));
    m_lineButton = new CColorButton(s, m_bubble->getPalette()->getPalette().line);
    connect(m_lineButton, SIGNAL(colorChanged(QColor)), this, SLOT(PaletteChanged()));
    m_selectButton = new CColorButton(s, m_bubble->getPalette()->getPalette().select);
    connect(m_selectButton, SIGNAL(colorChanged(QColor)), this, SLOT(PaletteChanged()));
    m_fontButton = new CColorButton(s, m_bubble->getPalette()->getPalette().font);
    connect(m_fontButton, SIGNAL(colorChanged(QColor)), this, SLOT(PaletteChanged()));

    m_name = new QLineEdit("New Palette");

    fl_buttons->addRow("Fill:", m_fillButton);
    fl_buttons->addRow("Outline:", m_lineButton);
    fl_buttons->addRow("Selected:", m_selectButton);
    fl_buttons->addRow("Font:", m_fontButton);

    QPushButton *pb_save = new QPushButton("Save");
    connect(pb_save, SIGNAL(clicked(bool)), this, SLOT(SavePressed()));
    QPushButton *pb_cancel = new QPushButton("Cancel");
    connect(pb_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));

    hl_dialogButtons->addStretch(1);
    hl_dialogButtons->addWidget(pb_save);
    hl_dialogButtons->addWidget(pb_cancel);

    vl_buttonsDialog->addWidget(m_name);
    vl_buttonsDialog->addLayout(fl_buttons);
    vl_buttonsDialog->addStretch(1);
    vl_buttonsDialog->addLayout(hl_dialogButtons);

    hl_viewButtons->addWidget(view, 3);
    hl_viewButtons->addLayout(vl_buttonsDialog, 1);

    setFont(shared().settingsView->font());
}

void CPaletteCreator::setPalette(const Chronicler::CPalette &palette)
{
    m_fillButton->setColor(palette.fill);
    m_lineButton->setColor(palette.line);
    m_selectButton->setColor(palette.select);
    m_fontButton->setColor(palette.font);

    PaletteChanged();
}

const CPalette CPaletteCreator::getPalette() const
{
    return m_bubble->getPalette()->getPalette();
}

void CPaletteCreator::setName(const QString &name)
{
    m_name->setText(name);
}

QString CPaletteCreator::getName()
{
    return m_name->text();
}

void CPaletteCreator::PaletteChanged()
{
    CPalette p;
    p.fill = m_fillButton->getColor();
    p.line = m_lineButton->getColor();
    p.select = m_selectButton->getColor();
    p.font = m_fontButton->getColor();

    m_bubble->setPalette(new CPaletteAction(this, p, m_name->text()));
}

void CPaletteCreator::SavePressed()
{
    emit accepted();
    close();
}
