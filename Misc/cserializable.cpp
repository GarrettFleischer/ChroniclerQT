#include "cserializable.h"

#include "Properties/cprojectview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

QDataStream &operator <<(QDataStream &stream, CSerializable &serializable)
{
    return serializable.Write(stream);
}

QDataStream &operator >>(QDataStream &stream, CSerializable &serializable)
{
    return serializable.Read(stream, shared().projectView->version());
}

CSerializable::CSerializable()
{}
