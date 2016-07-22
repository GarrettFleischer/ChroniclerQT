QT += core gui network xml webkit webkitwidgets widgets multimedia multimediawidgets

RESOURCES   =	chroniclernext.qrc

win32{
	LIBS += -LC:/Development/OpenSSL-Win32/lib -lubsec
        INCLUDEPATH += C:/Development/OpenSSL-Win32/include
}

RC_ICONS += GameSmith.ico

TRANSLATIONS += chroniclernext_es.ts

PKGCONFIG += openssl

CONFIG += c++11

# install
TARGET = Chronicler-Next

FORMS +=

HEADERS += \
    Properties/cpalettecreator.h \
    Bubbles/cactionbubble.h \
    Bubbles/cbubble.h \
    Bubbles/cchoice.h \
    Bubbles/cchoicebubble.h \
    Bubbles/cconditionbubble.h \
    Bubbles/csinglelinkbubble.h \
    Bubbles/cstartbubble.h \
    Bubbles/cstorybubble.h \
    Connections/cconnection.h \
    Connections/cline.h \
    Misc/Bubbles/cactiondelegate.h \
    Misc/Bubbles/cchoicemodel.h \
    Misc/Palette/ccolorbutton.h \
    Misc/Palette/cpaletteaction.h \
    Misc/Palette/cpalettebutton.h \
    Misc/Variables/cscenecombobox.h \
    Misc/Variables/cvariable.h \
    Misc/Variables/cvariablesdelegate.h \
    Misc/Variables/cvariablesmodel.h \
    Misc/cfiledownloader.h \
    Misc/choicescriptdata.h \
    Misc/chronicler.h \
    Misc/clineedit.h \
    Misc/cscenemodel.h \
    Misc/cserializable.h \
    Misc/cstringlistmodel.h \
    Misc/ctextedit.h \
    Misc/ctextitem.h \
    Misc/qactionbutton.h \
    Properties/Widgets/cactionproperties.h \
    Properties/Widgets/cchoiceproperties.h \
    Properties/Widgets/cconditionproperties.h \
    Properties/Widgets/clistpropertieswidget.h \
    Properties/Widgets/cpropertieswidget.h \
    Properties/Widgets/cstoryproperties.h \
    Properties/cdockmanager.h \
    Properties/cindentselectiondialog.h \
    Properties/cprojectview.h \
    Properties/cpropertiesmanager.h \
    Properties/cvariablesview.h \
    cgraphicsscene.h \
    cgraphicsview.h \
    chomepage.h \
    cmainwindow.h \
    csettingsview.h \
    Misc/qactionmenu.h \
    Misc/Bubbles/cactionmodel.h

SOURCES += \
    Bubbles/cactionbubble.cpp \
    Bubbles/cbubble.cpp \
    Bubbles/cchoice.cpp \
    Bubbles/cchoicebubble.cpp \
    Bubbles/cconditionbubble.cpp \
    Bubbles/csinglelinkbubble.cpp \
    Bubbles/cstartbubble.cpp \
    Bubbles/cstorybubble.cpp \
    Connections/cconnection.cpp \
    Connections/cline.cpp \
    Misc/Bubbles/cactiondelegate.cpp \
    Misc/Bubbles/cchoicemodel.cpp \
    Misc/Palette/ccolorbutton.cpp \
    Misc/Palette/cpaletteaction.cpp \
    Misc/Palette/cpalettebutton.cpp \
    Misc/Variables/cscenecombobox.cpp \
    Misc/Variables/cvariable.cpp \
    Misc/Variables/cvariablesdelegate.cpp \
    Misc/Variables/cvariablesmodel.cpp \
    Misc/cfiledownloader.cpp \
    Misc/choicescriptdata.cpp \
    Misc/chronicler.cpp \
    Misc/clineedit.cpp \
    Misc/cscenemodel.cpp \
    Misc/cserializable.cpp \
    Misc/cstringlistmodel.cpp \
    Misc/ctextedit.cpp \
    Misc/ctextitem.cpp \
    Misc/qactionbutton.cpp \
    Properties/Widgets/cactionproperties.cpp \
    Properties/Widgets/cchoiceproperties.cpp \
    Properties/Widgets/cconditionproperties.cpp \
    Properties/Widgets/clistpropertieswidget.cpp \
    Properties/Widgets/cpropertieswidget.cpp \
    Properties/Widgets/cstoryproperties.cpp \
    Properties/cdockmanager.cpp \
    Properties/cindentselectiondialog.cpp \
    Properties/cpalettecreator.cpp \
    Properties/cprojectview.cpp \
    Properties/cpropertiesmanager.cpp \
    Properties/cvariablesview.cpp \
    cgraphicsscene.cpp \
    cgraphicsview.cpp \
    chomepage.cpp \
    cmainwindow.cpp \
    csettingsview.cpp \
    main.cpp \
    Misc/qactionmenu.cpp \
    Misc/Bubbles/cactionmodel.cpp

