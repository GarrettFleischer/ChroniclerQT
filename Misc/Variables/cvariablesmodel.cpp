#include "cvariablesmodel.h"

#include <QStringList>

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"
#include "cgraphicsview.h"
#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "Bubbles/cchoicebubble.h"
#include "Bubbles/cchoice.h"
#include "Bubbles/cactionbubble.h"
#include "Misc/cstringlistmodel.h"
#include "Bubbles/cconditionbubble.h"
#include "Bubbles/ccodebubble.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesModel::CVariablesModel(QObject *parent)
    : QAbstractTableModel(parent)
{}


int CVariablesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_variables.length();
}

int CVariablesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant CVariablesModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(index.column() == 0)
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                return (m_variables[index.row()].scene()) ? m_variables[index.row()].scene()->name()
                        : shared().globalVariableTitle;
                }
            }
            else if(index.column() == 1)
            {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return m_variables[index.row()].name();
            }
            else
            {
                if(role == Qt::DisplayRole || role == Qt::EditRole)
                    return m_variables[index.row()].data();
            }
        }

        return QVariant();
    }

    bool CVariablesModel::setData(const QModelIndex &index, const QVariant &variant, int role)
    {
        if(index.isValid())
        {
            if(index.column() == 0)
            {
                if(role == Qt::EditRole)
                {
                    if(variant.isValid())
                        m_variables[index.row()].setScene(shared().projectView->model()->sceneWithName(variant.toString()));
                    else
                        m_variables[index.row()].setScene(Q_NULLPTR);

                    emit dataChanged(index, index, {Qt::EditRole, Qt::DisplayRole});
                    return true;
                }
            }
            else if(index.column() == 1)
            {
                if(role == Qt::EditRole)
                {
                    RefactorBubbles(m_variables[index.row()], variant.toString());
                    m_variables[index.row()].setName(variant.toString());

                    emit dataChanged(index, index, {Qt::EditRole, Qt::DisplayRole});
                    return true;
                }
            }
            else
            {
                if(role == Qt::EditRole)
                {
                    m_variables[index.row()].setData(variant.toString());

                    emit dataChanged(index, index, {Qt::EditRole, Qt::DisplayRole});
                    return true;
                }
            }
        }

        return false;
    }

    QVariant CVariablesModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if(section == 0)
                return "Scene";
            else if(section == 1)
                return "Name";
            else
                return "Data";
        }

        return QVariant();
    }

    Qt::ItemFlags CVariablesModel::flags(const QModelIndex &index) const
    {
        Q_UNUSED(index)

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }

    void CVariablesModel::AddItem(const CVariable &item)
    {
        int row = rowCount();

        beginInsertRows(QModelIndex(), row, row);
        m_variables.append(item);
        endInsertRows();
    }

    void CVariablesModel::RemoveItem(const CVariable &item)
    {
        int row = m_variables.indexOf(item);

        beginRemoveRows(QModelIndex(), row, row);
        m_variables.removeAll(item);
        endRemoveRows();
    }

    void CVariablesModel::Reset()
    {
        beginResetModel();
        m_variables.clear();
        endResetModel();
    }

    QList<CVariable> CVariablesModel::variables() const
    {
        return m_variables;
    }

    void CVariablesModel::RefactorBubbles(const CVariable &current, QString newname)
    {
        for(CGraphicsView *view : shared().projectView->model()->views())
        {
            // if global, replace everywhere, else replace only in local scene
            if(!current.scene() || current.scene() == view->cScene())
            {
                for(CBubble *b : view->cScene()->bubbles())
                {
                    if(b->getType() == Chronicler::StoryBubble)
                    {
                        CStoryBubble *bbl = static_cast<CStoryBubble *>(b);
                        bbl->setStory(bbl->getStory().replace("${" + current.name() + "}", "${" + newname + "}"));
                    }
                    else if(b->getType() == Chronicler::ChoiceBubble)
                    {
                        CChoiceBubble *bbl = static_cast<CChoiceBubble *>(b);
                        for(CChoice *choice : bbl->choiceBubbles())
                            choice->setChoice(choice->text().replace("${" + current.name() + "}", "${" + newname + "}"));
                    }
                    else if(b->getType() == Chronicler::ActionBubble)
                    {
                        CActionBubble *bbl = static_cast<CActionBubble *>(b);
                        QStringList actions = bbl->actions()->stringList();
                        for(int i = 0; i < actions.length(); ++i)
                            actions.replace(i, QString(actions.at(i)).replace(current.name(), newname));
                    }
                    else if(b->getType() == Chronicler::ConditionBubble)
                    {
                        CConditionBubble *bbl = static_cast<CConditionBubble *>(b);
                        bbl->setCondition(bbl->getCondition().replace(current.name(), newname));
                    }
                    else if(b->getType() == Chronicler::CodeBubble)
                    {
                        CCodeBubble *bbl = static_cast<CCodeBubble *>(b);
                        bbl->setCode(bbl->getCode().replace(current.name(), newname));
                    }
                }
            }
        }
    }


    bool CVariablesModel::removeRows(int row, int count, const QModelIndex &parent)
    {
        Q_UNUSED(parent)

        beginRemoveRows(QModelIndex(), row, row + count - 1);

        for(int i = row; i < row + count; ++i)
            m_variables.removeAt(i);

        endRemoveRows();

        return true;
    }

    void CVariablesModel::setVariables(const QList<CVariable> &variables)
    {
        beginResetModel();
        m_variables = variables;
        endResetModel();
    }
