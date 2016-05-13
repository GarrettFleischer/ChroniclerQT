#ifndef CUTILS_H
#define CUTILS_H

#include <QColor>

namespace Chronicler
{

    enum BubbleType { Story, Choice, Action, Condition  };

    enum Anchor { Right, Down, Left, Up, None };
    enum AllowedAnchors { NoAnchor = 0, RightAnchor = 1, DownAnchor = 2, LeftAnchor = 4, UpAnchor = 8 };

    struct CPalette
    {
        QColor fill = QColor(124, 140, 230);
        QColor line = Qt::black;
        QColor select = QColor(255,200,0);
        QColor font = Qt::white;
    };

}


#endif // CUTILS_H
