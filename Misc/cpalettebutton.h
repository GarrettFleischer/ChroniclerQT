#ifndef CPALETTEBUTTON_H
#define CPALETTEBUTTON_H

#include <QToolButton>

class CPaletteCreator;

#include "Misc/chronicler.h"
using Chronicler::CPalette;

class CPaletteButton : public QToolButton
{
    Q_OBJECT

public:
    CPaletteButton(QWidget *parent = 0);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *) override;

private:
    CPalette m_palette;
    QMenu *m_menu;

    CPaletteCreator *m_creator;

private slots:
    void Saved();

};

#endif // CPALETTEBUTTON_H
