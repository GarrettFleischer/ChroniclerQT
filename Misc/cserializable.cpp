#include "cserializable.h"

#include "Properties/cprojectview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

QDataStream &operator <<(QDataStream &stream, const CSerializable &serializable)
{
    return serializable.Serialize(stream);
}

QDataStream &operator >>(QDataStream &stream, CSerializable &serializable)
{
    return serializable.Deserialize(stream, shared().projectView->getVersion());
}

CSerializable::~CSerializable()
{}

CSerializable::CSerializable()
{}
