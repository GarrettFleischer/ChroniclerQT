QT += widgets webkitwidgets

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
    Connections/arrow.h \
    Connections/cconnection.h \
    Connections/clink.h \
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
    Misc/chronicler.h

SOURCES += \
    Bubbles/cbubble.cpp \
    Bubbles/cchoice.cpp \
    Bubbles/cchoicebubble.cpp \
    Bubbles/cconditionbubble.cpp \
    Bubbles/cstorybubble.cpp \
    Connections/arrow.cpp \
    Connections/cconnection.cpp \
    Connections/clink.cpp \
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
    Misc/chronicler.cpp
