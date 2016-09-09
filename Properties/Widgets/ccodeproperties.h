#ifndef CCODEPROPERTIES_H
#define CCODEPROPERTIES_H

#include "cpropertieswidget.h"

class CCodeBubble;
class CCodeEdit;


class CCodeProperties : public CPropertiesWidget
{
    Q_OBJECT

public:
    CCodeProperties(QWidget *parent = Q_NULLPTR);

    virtual void setBubble(CBubble *bubble) Q_DECL_OVERRIDE;

private:
    CCodeBubble *m_codeBubble;
    CCodeEdit *m_codeEdit;
    QStringListModel *m_completionModel;

private slots:
    void CodeChanged();

};

#endif // CCODEPROPERTIES_H
