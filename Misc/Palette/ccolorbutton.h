#ifndef CCOLORBUTTON_H
#define CCOLORBUTTON_H

#include <QPushButton>

QT_BEGIN_NAMESPACE
class QIcon;
class QColorDialog;
QT_END_NAMESPACE

class CColorButton : public QPushButton
{
    Q_OBJECT

public:
    CColorButton(const QSize &size, const QColor &color, QWidget *parent = 0);

    void setColor(const QColor &color);
    QColor getColor();

//protected:
//    virtual void resizeEvent(QResizeEvent *evt) override;

private:
    QIcon ColorIcon();

    QColor m_color;
    QColorDialog *m_dialog;

signals:
    void colorChanged(const QColor &color);

private slots:
    void ShowDialog();
    void ColorChanged(const QColor &color);
};

#endif // CCOLORBUTTON_H
