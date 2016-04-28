/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QGraphicsItem;
class QTabWidget;
class QSettings;
QT_END_NAMESPACE

class CGraphicsScene;
class CBubble;
class CGraphicsView;
class CDockManager;
class CSettingsView;


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
   CMainWindow(QSettings *settings);

   void LoadProject(const QString & filepath);

protected:
   virtual void keyPressEvent(QKeyEvent *evt);
   virtual void keyReleaseEvent(QKeyEvent *evt);


private slots:
    void DeleteItem();
    void PointerGroupClicked(int id);
    void ItemInserted(CBubble *);
    void ItemSelected(QGraphicsItem *selectedItem);
    void ShowSettings();
    void ShowAbout();
    void SceneLeftPressed();
    void SceneLeftReleased();
    void TabClosed(int);
    void DockAreaChanged(Qt::DockWidgetArea);
    void ToolBarAreaChanged(bool);
    void SettingsChanged();


private:
    void CreateActions();
    void CreateMenus();
    void CreateToolbars();
    QIcon CreateColorToolButtonIcon(const QString &image, QColor color);
    QIcon CreateColorIcon(QColor color);

    bool m_ShiftHeld;

    QTabWidget *m_tabView;
    CGraphicsScene *m_scene;
    CGraphicsView *m_view;

    QAction *m_exitAction;
    QAction *m_deleteAction;
    QAction *m_aboutAction;
    QAction *m_settingsAction;
    QAction *m_newProjectAction;

    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_itemMenu;
    QMenu *m_aboutMenu;

    QToolBar *m_pointerToolBar;
    QButtonGroup *m_pointerTypeGroup;

    QDockWidget *m_dock;
    CDockManager *m_dockManager;

    QSettings *m_settings;
    CSettingsView *m_settingsView;

};

#endif // MAINWINDOW_H
