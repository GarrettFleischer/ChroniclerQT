#ifndef CPALETTEBUTTON_H
#define CPALETTEBUTTON_H

#include <QToolButton>

class CPaletteCreator;
class QActionMenu;
class CPaletteAction;

#include "Misc/cserializable.h"

#include "Misc/chronicler.h"
using Chronicler::CPalette;

class CPaletteButton : public QToolButton, public CSerializable
{
    Q_OBJECT

public:
    CPaletteButton(QWidget *parent = 0);

    CPaletteAction *getPalette();

    CPaletteAction *getPaletteWithUID(Chronicler::t_uid uid);

    void setCurrent(CPaletteAction *palette);
    CPaletteAction *getCurrent();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *) override;

    virtual QDataStream &Deserialize(QDataStream &ds, const QString &) override;
    virtual QDataStream &Serialize(QDataStream &ds) override;

private:
    CPaletteAction *m_current;

    CPaletteAction *m_editing;

    QActionMenu *m_menu;

    CPaletteCreator *m_creator;

private slots:
    void SelectAction(QAction *action);
    void EditAction(QAction *action);
    void Saved();

};

#endif // CPALETTEBUTTON_H
