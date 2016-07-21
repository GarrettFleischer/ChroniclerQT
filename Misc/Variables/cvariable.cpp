#include "cvariable.h"

#include "cgraphicsscene.h"
#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;


CVariable::CVariable()
    : m_scene(Q_NULLPTR)
{}

CVariable::CVariable(QString name, QString data, CGraphicsScene *scene)
    : m_name(name), m_data(data), m_scene(scene)
{}

CVariable::~CVariable()
{}

bool CVariable::operator ==(const CVariable &rhs)
{
    return m_name == rhs.m_name && m_data == rhs.m_data && m_scene == rhs.m_scene;
}


QDataStream &CVariable::Deserialize(QDataStream &ds, const QString &version)
{
    Q_UNUSED(version)

    QString sceneName;
    ds >> sceneName;
    if(sceneName.length())
        m_scene = shared().projectView->model()->sceneWithName(sceneName);

    return ds >> m_name >> m_data;
}

QDataStream &CVariable::Serialize(QDataStream &ds)
{
    return ds << (m_scene ? m_scene->name() : QString()) << m_name << m_data;
}

CGraphicsScene *CVariable::scene() const
{
    return m_scene;
}

void CVariable::setScene(CGraphicsScene *scene)
{
    m_scene = scene;
}

QString CVariable::data() const
{
    return m_data;
}

void CVariable::setData(const QString &data)
{
    m_data = data;
}

QString CVariable::name() const
{
    return m_name;
}

void CVariable::setName(const QString &name)
{
    m_name = name;
}
