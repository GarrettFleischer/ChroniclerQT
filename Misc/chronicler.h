#ifndef CUTILS_H
#define CUTILS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QColor;
QT_END_NAMESPACE


namespace Chronicler
{

    enum BubbleType { Story, Condition, Choice};


    struct CPalette
    {
        QColor fill = QColor(124, 140, 230);
        QColor line = Qt::black;
        QColor select = QColor(255,200,0);
        QColor font = Qt::white;
    };

}


#endif // CUTILS_H
