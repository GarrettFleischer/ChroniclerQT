#include "cvariablesview.h"

#include <QVBoxLayout>

#include <QTableView>

//#include <QLineEdit>
//#include <QDataWidgetMapper>
//#include <QComboBox>

#include "Misc/cvariablesmodel.h"
#include "Misc/cvariablesdelegate.h"
//#include "Properties/cprojectview.h"

//#include "Misc/chronicler.h"
//using Chronicler::shared;

CVariablesView::CVariablesView(QWidget *parent)
    : QWidget(parent)
{
    m_model = new CVariablesModel(this);

    m_view = new QTableView(this);
    m_view->setModel(m_model);
    m_view->setItemDelegate(new CVariablesDelegate(this));

//    QComboBox *box = new QComboBox(this);
//    box->setModel(shared().projectView->model());

//    QLineEdit *line = new QLineEdit(this);

//    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
//    mapper->setModel(m_model);
//    mapper->addMapping(box, 0);
//    mapper->addMapping(line, 1);
//    mapper->addMapping(line, 2);

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addWidget(m_view);

    m_model->AddItem(CVariable(0, "name", "John"));
    m_model->AddItem(CVariable(0, "occupation", "Smith"));
    m_model->AddItem(CVariable(0, "age", "23"));
}
