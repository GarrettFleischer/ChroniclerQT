QT += core gui network xml webkit webkitwidgets widgets multimedia multimediawidgets

RESOURCES   =	chroniclernext.qrc

#win32{
#	LIBS += -LC:/Development/OpenSSL-Win32/lib -lubsec
#        INCLUDEPATH += C:/Development/OpenSSL-Win32/include
#}

RC_ICONS += GameSmith.ico
ICON = GameSmith.icns

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
    Misc/Bubbles/cactionmodel.h \
    Misc/clistbuttons.h \
    Misc/Bubbles/cactionedit.h \
    Misc/History/cmovebubblecommand.h \
    Misc/History/cremovebubblescommand.h \
    Misc/History/caddbubblescommand.h \
    Misc/History/cresizebubblecommand.h \
    Misc/History/cremovescenecommand.h \
    Misc/Bubbles/cconditionedit.h \
    Misc/Bubbles/cchoiceedit.h \
    Misc/Bubbles/cchoicedelegate.h \
    Bubbles/ccodebubble.h \
    Properties/Widgets/ccodeproperties.h \
    Misc/Bubbles/ccodeedit.h \
    Misc/Stats/cstaticon.h \
    Properties/Stats/cstatseditor.h \
    Properties/Stats/cstatsmodel.h \
    Models/cchoicescriptmodel.h \
    Models/cprojectmodel.h \
    Bubbles/cstartherebubble.h \
    Misc/Bubbles/cstartheremodel.h \
    Misc/Bubbles/cstartheredelegate.h \
    Misc/Bubbles/cstarthereedit.h \
    Properties/Widgets/cstarthereproperties.h \
    Misc/cshighlighter.h \
    Misc/Palette/cpalettemodel.h \
    hunspell/affentry.hxx \
    hunspell/affixmgr.hxx \
    hunspell/atypes.hxx \
    hunspell/baseaffix.hxx \
    hunspell/csutil.hxx \
    hunspell/dictmgr.hxx \
    hunspell/filemgr.hxx \
    hunspell/hashmgr.hxx \
    hunspell/htypes.hxx \
    hunspell/hunspell.h \
    hunspell/hunspell.hxx \
    hunspell/hunzip.hxx \
    hunspell/langnum.hxx \
    hunspell/phonet.hxx \
    hunspell/suggestmgr.hxx \
    hunspell/w_char.hxx \
    Misc/SpellTextEdit.h \
    Misc/highlighter.h

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
    Misc/Bubbles/cactionmodel.cpp \
    Misc/clistbuttons.cpp \
    Misc/Bubbles/cactionedit.cpp \
    Misc/History/cmovebubblecommand.cpp \
    Misc/History/cremovebubblescommand.cpp \
    Misc/History/caddbubblescommand.cpp \
    Misc/History/cresizebubblecommand.cpp \
    Misc/History/cremovescenecommand.cpp \
    Misc/Bubbles/cconditionedit.cpp \
    Misc/Bubbles/cchoiceedit.cpp \
    Misc/Bubbles/cchoicedelegate.cpp \
    Bubbles/ccodebubble.cpp \
    Properties/Widgets/ccodeproperties.cpp \
    Misc/Bubbles/ccodeedit.cpp \
    Misc/Stats/cstaticon.cpp \
    Properties/Stats/cstatseditor.cpp \
    Properties/Stats/cstatsmodel.cpp \
    Models/cchoicescriptmodel.cpp \
    Models/cprojectmodel.cpp \
    Bubbles/cstartherebubble.cpp \
    Misc/Bubbles/cstartheremodel.cpp \
    Misc/Bubbles/cstartheredelegate.cpp \
    Misc/Bubbles/cstarthereedit.cpp \
    Properties/Widgets/cstarthereproperties.cpp \
    Misc/cshighlighter.cpp \
    Misc/Palette/cpalettemodel.cpp \
    hunspell/affentry.cxx \
    hunspell/affixmgr.cxx \
    hunspell/csutil.cxx \
    hunspell/dictmgr.cxx \
    hunspell/filemgr.cxx \
    hunspell/hashmgr.cxx \
    hunspell/hunspell.cxx \
    hunspell/hunzip.cxx \
    hunspell/phonet.cxx \
    hunspell/suggestmgr.cxx \
    hunspell/utf_info.cxx \
    Misc/SpellTextEdit.cpp \
    Misc/highlighter.cpp

DISTFILES += \
    hunspell/license.hunspell \
    hunspell/license.myspell
