#ifndef CPALETTECREATOR_H
#define CPALETTECREATOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QColorDialog;
class QLineEdit;
QT_END_NAMESPACE

class CStoryBubble;
class CColorButton;

#include "Misc/chronicler.h"
using Chronicler::CPalette;

class CPaletteCreator : public QDialog
{
    Q_OBJECT

public:
    CPaletteCreator(QWidget *parent = 0);

    void setPalette(const CPalette &palette);
    CPalette getPalette();

    void setName(const QString &name);
    QString getName();

private:
    CStoryBubble *m_bubble;

    QColorDialog *m_colorDialog;

    CColorButton *m_fillButton;
    CColorButton *m_lineButton;
    CColorButton *m_selectButton;
    CColorButton *m_fontButton;

    QLineEdit *m_name;

private slots:
    void PaletteChanged();

    void SavePressed();

};

#endif // CPALETTECREATOR_H
