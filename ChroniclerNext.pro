QT += widgets webkitwidgets

RESOURCES   =	chroniclernext.qrc

LIBS += -LC:/Development/OpenSSL-Win32/lib -lubsec
INCLUDEPATH += C:/Development/OpenSSL-Win32/include

PKGCONFIG += openssl

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
    Properties/cconditionproperties.h \
    Properties/cpropertiesview.h \
    Properties/cstoryproperties.h \
    Connections/cline.h \
    cgraphicsview.h \
    cgraphicsscene.h \
    chomepage.h \
    cmainwindow.h

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
    Properties/cconditionproperties.cpp \
    Properties/cpropertiesview.cpp \
    Properties/cstoryproperties.cpp \
    main.cpp \
    Connections/cline.cpp \
    cgraphicsview.cpp \
    cgraphicsscene.cpp \
    chomepage.cpp \
    cmainwindow.cpp
