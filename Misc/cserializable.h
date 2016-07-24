#ifndef CSERIALIZABLE_H
#define CSERIALIZABLE_H

#include <QDataStream>

#include "Misc/chronicler.h"
using Chronicler::CVersion;

class CSerializable
{
public:
    friend QDataStream & operator <<(QDataStream &stream, CSerializable &serializable);
    friend QDataStream & operator >>(QDataStream &stream, CSerializable &serializable);

    virtual ~CSerializable();

protected:
    CSerializable();

    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) = 0;
    virtual QDataStream &Serialize(QDataStream &ds) = 0;
};

#endif // CSERIALIZABLE_H
