#ifndef CPALETTEACTION_H
#define CPALETTEACTION_H

#include <QAction>

#include "Misc/chronicler.h"
using Chronicler::CPalette;

class CPaletteAction : public QAction
{
public:
    CPaletteAction(QObject *parent = 0, const CPalette &palette = CPalette(), const QString &name = "Default");

    const CPalette &getPalette() const;
    void setPalette(const CPalette &palette);

private:
    QIcon PaletteIcon();

    CPalette m_palette;
};

#endif // CPALETTEACTION_H
