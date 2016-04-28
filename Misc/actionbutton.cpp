#include "actionbutton.h"

#include <QAction>


ActionButton::ActionButton(QWidget *parent, QAction *action)
    : QPushButton(parent), m_action(0)
{
    setAction(action);
}

void ActionButton::setAction(QAction *action)
{

    // if I've got already an action associated to the button
    // remove all connections
    if( m_action && m_action != action ){
        disconnect(m_action, SIGNAL(changed()),
                   this, SLOT(updateButtonStatusFromAction()) );

        disconnect(this, SIGNAL(clicked()),
                   m_action, SLOT(trigger()) );
    }

    // store the action
    m_action = action;

    // ensure action is not null
    if(m_action)
    {
        // configure the button
        updateButtonStatusFromAction();

        // connect the action and the button
        // so that when the action is changed the
        // button is changed too!
        connect(m_action, SIGNAL(changed()),
                this, SLOT(updateButtonStatusFromAction()));

        // connect the button to the slot that forwards the
        // signal to the action
        connect(this, SIGNAL(clicked()),
                m_action, SLOT(trigger()));
    }
}

void ActionButton::updateButtonStatusFromAction()
{
    setText(m_action->text());
    setStatusTip(m_action->statusTip());
    setToolTip(m_action->toolTip());
    setIcon(m_action->icon());
    setEnabled(m_action->isEnabled());
    setCheckable(m_action->isCheckable());
    setChecked(m_action->isChecked());
}
