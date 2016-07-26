#include "cactiondelegate.h"

#include <QComboBox>
#include <QLineEdit>

#include <QAbstractTableModel>

#include "Misc/Bubbles/cactionmodel.h"

#include "Misc/Variables/cvariable.h"
#include "Properties/cvariablesview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CActionDelegate::CActionDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QWidget *CActionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    QWidget *editor = Q_NULLPTR;

    if(index.isValid())
    {
        if(index.column() == 0)
        {
            QComboBox *box = new QComboBox(parent);
            connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(PersistentEditorChanged()));
            connect(box, SIGNAL(activated(int)), this, SLOT(PersistentEditorChanged()));
            box->addItems({"*set", "*page_break", "*input_text", "*label", "*goto", "*finish", "*purchase", "*check_purchase", "*advertisement"});

            editor = box;
        }
        else
        {
            QString first_column = index.model()->index(index.row(), 0).data().toString();

            if(first_column == "*set")
            {
                if(index.column() == 1)
                {
                    editor = createVariablesBox(parent);
                }
                else if(index.column() == 2)
                {
                    QComboBox *box = new QComboBox(parent);
                    connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(PersistentEditorChanged()));
                    connect(box, SIGNAL(activated(int)), this, SLOT(PersistentEditorChanged()));
                    box->addItems({"=", "%+", "%-", "+=", "-="});

                    editor = box;
                }
                else if(index.column() == 3)
                {
                    QLineEdit *line = new QLineEdit(parent);
                    connect(line, SIGNAL(textChanged(QString)), this, SLOT(PersistentEditorChanged()));

                    editor = line;
                }
            }
            else if(first_column == "*input_text")
            {
                if(index.column() == 1)
                    editor = createVariablesBox(parent);
            }
            else if(first_column == "*label" || first_column == "*goto" ||
                    first_column == "*purchase" || first_column == "*check_purchase")
            {
                if(index.column() == 1)
                {
                    QLineEdit *line = new QLineEdit(parent);
                    connect(line, SIGNAL(textChanged(QString)), this, SLOT(PersistentEditorChanged()));

                    editor = line;
                }
            }
        }
    }

    return editor;
}

void CActionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{   
    if(index.isValid())
    {
        if(index.column() == 0)
        {
            QComboBox *box = static_cast<QComboBox *>(editor);
            box->setCurrentText(index.data(Qt::EditRole).toString());
        }
        else
        {
            QString first_column = index.model()->index(index.row(), 0).data().toString();

            if(first_column == "*set")
            {
                if(index.column() < 3)
                {
                    QComboBox *box = dynamic_cast<QComboBox *>(editor);
                    if(box)
                        box->setCurrentText(index.data(Qt::EditRole).toString());
                }
                else
                {
                    QLineEdit *line = dynamic_cast<QLineEdit *>(editor);
                    if(line)
                        line->setText(index.data(Qt::EditRole).toString());
                }
            }
            else if(first_column == "*input_text")
            {
                if(index.column() == 1)
                {
                    QComboBox *box = dynamic_cast<QComboBox *>(editor);
                    if(box)
                        box->setCurrentText(index.data(Qt::EditRole).toString());
                }
            }
            else if(first_column == "*label" || first_column == "*goto" ||
                    first_column == "*purchase" || first_column == "*check_purchase")
            {
                if(index.column() == 1)
                {
                    QLineEdit *line = dynamic_cast<QLineEdit *>(editor);
                    if(line)
                        line->setText(index.data(Qt::EditRole).toString());
                }
            }
        }
    }
}

void CActionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid())
    {
        QAbstractTableModel *actionModel = static_cast<QAbstractTableModel *>(model);

        if(index.column() == 0)
        {
            QComboBox *box = static_cast<QComboBox *>(editor);
            actionModel->setData(index, box->currentText());
        }
        else
        {
            QString first_column = index.model()->index(index.row(), 0).data().toString();

            if(first_column == "*set")
            {
                if(index.column() < 3)
                {
                    QComboBox *box = dynamic_cast<QComboBox *>(editor);
                    if(box)
                        actionModel->setData(index, box->currentText());
                }
                else
                {
                    QLineEdit *line = dynamic_cast<QLineEdit *>(editor);
                    if(line)
                        actionModel->setData(index, line->text());
                }
            }
            else if(first_column == "*input_text")
            {
                if(index.column() == 1)
                {
                    QComboBox *box = dynamic_cast<QComboBox *>(editor);
                    if(box)
                        actionModel->setData(index, box->currentText());
                }
            }
            else if(first_column == "*label" || first_column == "*goto" ||
                    first_column == "*purchase" || first_column == "*check_purchase")
            {
                if(index.column() == 1)
                {
                    QLineEdit *line = dynamic_cast<QLineEdit *>(editor);
                    if(line)
                        actionModel->setData(index, line->text());
                }
            }
        }
    }
}

void CActionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QComboBox *CActionDelegate::createVariablesBox(QWidget *parent) const
{
    QComboBox *editor = new QComboBox(parent);
    connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(PersistentEditorChanged()));
    connect(editor, SIGNAL(activated(int)), this, SLOT(PersistentEditorChanged()));

    QList<CVariable> variables = shared().variablesView->getVariablesForScene(Q_NULLPTR);
    for(const CVariable &v : variables)
        editor->addItem(v.name());

    return editor;
}

void CActionDelegate::PersistentEditorChanged()
{
    emit commitData(static_cast<QWidget *>(sender()));
}
