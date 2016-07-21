#ifndef CVARIABLE_H
#define CVARIABLE_H

#include "Misc/cserializable.h"

class CGraphicsScene;

class CVariable : public CSerializable
{
public:
    CVariable();
    CVariable(QString name, QString data, CGraphicsScene *scene);

    virtual ~CVariable();

    bool operator ==(const CVariable &rhs);

    QString name() const;
    void setName(const QString &name);

    QString data() const;
    void setData(const QString &data);

    CGraphicsScene *scene() const;
    void setScene(CGraphicsScene *scene);



protected:
    virtual QDataStream &Deserialize(QDataStream &ds, const QString &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) Q_DECL_OVERRIDE;

private:
    QString m_name;
    QString m_data;

    CGraphicsScene *m_scene;
};

#endif // CVARIABLE_H
