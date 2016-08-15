#include "cstoryproperties.h"

#include <QAction>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QStringListModel>
#include <QTabWidget>

#include "Misc/ctextedit.h"
#include "Misc/qactionbutton.h"
#include "Bubbles/cstorybubble.h"

#include "cgraphicsview.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"

#include "Misc/chronicler.h"
using Chronicler::shared;


CStoryProperties::CStoryProperties(QWidget *parent)
    : CPropertiesWidget(parent), m_storyBubble(Q_NULLPTR), m_storyEdit(Q_NULLPTR)
{
    m_completionModel = new QStringListModel(this);

    m_boldAction = new QAction(QIcon(":/images/icn_bold.png"), tr(""), Q_NULLPTR);
    m_boldAction->setShortcut(tr("Ctrl+B"));
    m_boldAction->setToolTip(tr("Toggle bold tags around selection"));
    connect(m_boldAction, SIGNAL(triggered()),
            this, SLOT(BoldTriggered()));

    m_italicAction = new QAction(QIcon(":/images/icn_italic.png"), tr(""), Q_NULLPTR);
    m_italicAction->setShortcut(tr("Ctrl+I"));
    m_italicAction->setToolTip(tr("Toggle italic tags around selection"));
    connect(m_italicAction, SIGNAL(triggered()),
            this, SLOT(ItalicTriggered()));

    QActionButton *btn_bold = new QActionButton(Q_NULLPTR, m_boldAction);
    QActionButton *btn_italic = new QActionButton(Q_NULLPTR, m_italicAction);

    QHBoxLayout *hl_font = new QHBoxLayout();
    hl_font->addWidget(btn_bold);
    hl_font->addWidget(btn_italic);
    hl_font->addStretch(1);

    // Story Widget
    m_storyEdit = new CTextEdit(this, m_completionModel);
    m_storyEdit->addAction(m_boldAction);
    m_storyEdit->addAction(m_italicAction);
    connect(m_storyEdit, SIGNAL(textChanged()),
            this, SLOT(StoryChanged()));

    m_layout->addLayout(hl_font);
    m_layout->addWidget(m_storyEdit);

    setBubble(0);
}


void CStoryProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_storyBubble = qgraphicsitem_cast<CStoryBubble *>(bbl);
    if(m_storyBubble)
    {
        UpdateModel();

        m_storyEdit->setText(m_storyBubble->getStory());
        m_storyEdit->setFont(m_storyBubble->getFont());
    }
    else
    {
        m_storyEdit->setText(tr(""));
    }
}

void CStoryProperties::TagSelectedText(const QString &tag)
{
    QTextCursor c = m_storyEdit->textCursor();
    QString txt = c.selectedText();
    QString tag_start = "[" + tag + "]";
    QString tag_end = "[/" + tag + "]";
    int end = (txt.length() - tag_end.length());

    // if selected text starts and ends with the given tag
    // then remove those tags.
    if(txt.indexOf(tag_start) == 0 && txt.mid(end) == tag_end)
    {
        txt = txt.remove(end, tag_end.length());
        txt = txt.remove(0, tag_start.length());

        c.insertText(txt);
    }
    // otherwise wrap the selected text with the given tag.
    else
    {
        c.insertText(tag_start + txt + tag_end);
        c.setPosition(c.position() - tag_end.length());
    }

    m_storyEdit->setTextCursor(c);

    m_storyEdit->setFocus();
}

void CStoryProperties::UpdateModel()
{
    QStringList lst;

    // grab the currently visible scene
    CGraphicsView *view = dynamic_cast<CGraphicsView *>(shared().sceneTabs->currentWidget());

    for(const CVariable &v : shared().variablesView->getVariablesForScene((view ? view->cScene() : Q_NULLPTR)))
            lst.append("${" + v.name() + "}");

    m_completionModel->setStringList(lst);
}


void CStoryProperties::StoryChanged()
{
    if(m_storyBubble)
        m_storyBubble->setStory(m_storyEdit->toPlainText());
}

void CStoryProperties::BoldTriggered()
{
    TagSelectedText("b");
}

void CStoryProperties::ItalicTriggered()
{
    TagSelectedText("i");
}

