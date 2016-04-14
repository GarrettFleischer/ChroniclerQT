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

#include "Properties/cpropertiesview.h"


class CGraphicsScene;
class CBubble;
class CGraphicsView;

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
QT_END_NAMESPACE


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
   CMainWindow();

   void LoadProject(const QString & filename);

protected:
   virtual void keyPressEvent(QKeyEvent *evt);
   virtual void keyReleaseEvent(QKeyEvent *evt);

public slots:
void TabClosed(int);

private slots:
    void DeleteItem();
    void PointerGroupClicked(int id);
    void ItemInserted(CBubble *);
    void CurrentFontChanged(const QFont &font);
    void FontSizeChanged(const QString &size);
    void HandleFontChange();
    void SceneScaleChanged(const QString &scale);
    void TextColorChanged();
    void ItemColorChanged();
    void LineColorChanged();
    void TextButtonTriggered();
    void FillButtonTriggered();
    void LineButtonTriggered();
    void ItemSelected(QGraphicsItem *selectedItem);
    void About();
    void SceneLeftPressed();
    void SceneLeftReleased();


private:
    void CreateActions();
    void CreateMenus();
    void CreateToolbars();
    QMenu *CreateColorMenu(const char *slot, QColor defaultColor);
    QIcon CreateColorToolButtonIcon(const QString &image, QColor color);
    QIcon CreateColorIcon(QColor color);
    void UpdateSceneScale();

    bool m_ShiftHeld;
    float m_scale;

    QTabWidget *m_tabView;
    CGraphicsScene *m_scene;
    CGraphicsView *m_view;

    QAction *m_exitAction;
    QAction *m_addAction;
    QAction *m_deleteAction;

    QAction *m_toFrontAction;
    QAction *m_sendBackAction;
    QAction *m_aboutAction;

    QMenu *m_fileMenu;
    QMenu *m_itemMenu;
    QMenu *m_aboutMenu;

    QToolBar *m_textToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_colorToolBar;
    QToolBar *m_pointerToolBar;
    QToolBar *m_bubbleToolBar;

    QComboBox *m_sceneScaleCombo;
    QComboBox *m_itemColorCombo;
    QComboBox *m_textColorCombo;
    QSpinBox *m_fontSizeCombo;
    QFontComboBox *m_fontCombo;

    QToolBox *m_toolBox;
    QButtonGroup *m_buttonGroup;
    QButtonGroup *m_pointerTypeGroup;
    QButtonGroup *m_backgroundButtonGroup;
    QButtonGroup *m_bubbleTypeGroup;

    QToolButton *m_fontColorToolButton;
    QToolButton *m_fillColorToolButton;
    QToolButton *m_lineColorToolButton;

    QAction *m_boldAction;
    QAction *m_underlineAction;
    QAction *m_italicAction;
    QAction *m_textAction;
    QAction *m_fillAction;
    QAction *m_lineAction;

    QDockWidget *m_dock;
    CPropertiesView *m_properties;

};

#endif // MAINWINDOW_H
