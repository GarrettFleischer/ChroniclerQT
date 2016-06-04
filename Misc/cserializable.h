#ifndef CSERIALIZABLE_H
#define CSERIALIZABLE_H

#include <QDataStream>

class CSerializable
{
public:
    friend QDataStream & operator <<(QDataStream &stream, CSerializable &serializable);
    friend QDataStream & operator >>(QDataStream &stream, CSerializable &serializable);

protected:
    CSerializable();

    virtual QDataStream &Read(QDataStream &ds, const QString &version) = 0;
    virtual QDataStream &Write(QDataStream &ds) = 0;
};

#endif // CSERIALIZABLE_H
