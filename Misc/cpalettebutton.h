#ifndef CPALETTEBUTTON_H
#define CPALETTEBUTTON_H

#include <QToolButton>

class CPaletteCreator;
class CActionMenu;
class CPaletteAction;

#include "Misc/chronicler.h"
using Chronicler::CPalette;

class CPaletteButton : public QToolButton
{
    Q_OBJECT

public:
    CPaletteButton(QWidget *parent = 0);

    const CPalette &getPalette() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *) override;

private:
    CPaletteAction *m_current;

    CPaletteAction *m_editing;

    CActionMenu *m_menu;

    CPaletteCreator *m_creator;

private slots:
    void SelectAction(QAction *action);
    void EditAction(QAction *action);
    void Saved();


};

#endif // CPALETTEBUTTON_H
