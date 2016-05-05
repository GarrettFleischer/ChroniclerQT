#ifndef CUTILS_H
#define CUTILS_H

#include <QColor>

namespace Chronicler
{

    enum BubbleType { Story, Condition, Action, Choice };


    struct CPalette
    {
        QColor fill = QColor(124, 140, 230);
        QColor line = Qt::black;
        QColor select = QColor(255,200,0);
        QColor font = Qt::white;
    };

}


#endif // CUTILS_H
