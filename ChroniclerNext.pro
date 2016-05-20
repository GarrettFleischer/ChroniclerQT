QT += core gui network webkit webkitwidgets widgets multimedia multimediawidgets

RESOURCES   =	chroniclernext.qrc

LIBS += -LC:/Development/OpenSSL-Win32/lib -lubsec
INCLUDEPATH += C:/Development/OpenSSL-Win32/include

PKGCONFIG += openssl

CONFIG += c++11

# install
TARGET = Chronicler-Next

FORMS +=

HEADERS += \
    Bubbles/cbubble.h \
    Bubbles/cchoice.h \
    Bubbles/cchoicebubble.h \
    Bubbles/cconditionbubble.h \
    Bubbles/cstorybubble.h \
    Connections/cconnection.h \
    Misc/clineedit.h \
    Misc/ctextedit.h \
    Misc/ctextitem.h \
    Connections/cline.h \
    cgraphicsview.h \
    cgraphicsscene.h \
    chomepage.h \
    cmainwindow.h \
    Properties/cprojectview.h \
    Properties/cdockmanager.h \
    Properties/cpropertiesmanager.h \
    Properties/Widgets/cchoiceproperties.h \
    Properties/Widgets/cconditionproperties.h \
    Properties/Widgets/cpropertieswidget.h \
    Properties/Widgets/cstoryproperties.h \
    Misc/chronicler.h \
    csettingsview.h \
    Misc/qactionbutton.h \
    Bubbles/csinglelinkbubble.h \
    Bubbles/cactionbubble.h \
    Misc/cstringlistmodel.h \
    Properties/Widgets/cactionproperties.h \
    Misc/cchoicemodel.h \
    Properties/Widgets/clistpropertieswidget.h \
    Misc/cscenemodel.h \
    Misc/filedownloader.h \
    Bubbles/cstartbubble.h

SOURCES += \
    Bubbles/cbubble.cpp \
    Bubbles/cchoice.cpp \
    Bubbles/cchoicebubble.cpp \
    Bubbles/cconditionbubble.cpp \
    Bubbles/cstorybubble.cpp \
    Connections/cconnection.cpp \
    Misc/clineedit.cpp \
    Misc/ctextedit.cpp \
    Misc/ctextitem.cpp \
    main.cpp \
    Connections/cline.cpp \
    cgraphicsview.cpp \
    cgraphicsscene.cpp \
    chomepage.cpp \
    cmainwindow.cpp \
    Properties/cprojectview.cpp \
    Properties/cdockmanager.cpp \
    Properties/cpropertiesmanager.cpp \
    Properties/Widgets/cchoiceproperties.cpp \
    Properties/Widgets/cconditionproperties.cpp \
    Properties/Widgets/cpropertieswidget.cpp \
    Properties/Widgets/cstoryproperties.cpp \
    Misc/chronicler.cpp \
    csettingsview.cpp \
    Misc/qactionbutton.cpp \
    Bubbles/csinglelinkbubble.cpp \
    Bubbles/cactionbubble.cpp \
    Misc/cstringlistmodel.cpp \
    Properties/Widgets/cactionproperties.cpp \
    Misc/cchoicemodel.cpp \
    Properties/Widgets/clistpropertieswidget.cpp \
    Misc/cscenemodel.cpp \
    Misc/filedownloader.cpp \
    Bubbles/cstartbubble.cpp
