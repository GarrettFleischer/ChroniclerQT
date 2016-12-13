#include "cprojectmodel.h"

#include "Misc/cstringlistmodel.h"
#include "Properties/cprojectview.h"
#include "cgraphicsscene.h"
#include "cgraphicsview.h"
#include "Bubbles/cbubble.h"
#include "Bubbles/cactionbubble.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CProjectModel::CProjectModel(QObject *parent)
    : QObject(parent)
{

}

QList<QString> CProjectModel::getLabels()
{
    QList<QString> labels;

    for (CGraphicsView *view : shared().projectView->getViews())
    {
        labels.append(getLabels(view->cScene()));
    }

    return labels;
}

QList<QString> CProjectModel::getLabels(CGraphicsScene *scene)
{
    QList<QString> labels;

    if(scene)
    {
        for(CBubble *bubble : scene->bubbles())
        {
            if(bubble->getLabel().length())
                labels.append(bubble->getLabel());

            if(bubble->getType() == Chronicler::ActionBubble)
            {
                CActionBubble *ab = static_cast<CActionBubble *>(bubble);
                for(QString action : ab->actions()->stringList())
                {
                    QStringList words = action.split(" ", QString::SkipEmptyParts);
                    if(words.length() > 1 && words[0] == "*label")
                        labels.append(words[1]);
                }
            }
        }
    }

    return labels;
}
