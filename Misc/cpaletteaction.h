#ifndef CPALETTEACTION_H
#define CPALETTEACTION_H

#include <QAction>

#include "Misc/chronicler.h"
using Chronicler::CPalette;
using Chronicler::t_uid;

class CPaletteAction : public QAction
{
public:
    CPaletteAction(QObject *parent = 0, const CPalette &palette = CPalette(), const QString &name = "Default", t_uid uid = generateUID());

    CPalette getPalette() const;
    void setPalette(const CPalette &palette);

    t_uid getUID() const;

private:
    QIcon PaletteIcon();
    static t_uid generateUID();

    t_uid m_uid;
    CPalette m_palette;

    static QList<t_uid> m_UIDs;
};

#endif // CPALETTEACTION_H
