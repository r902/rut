QT       += core gui widgets
CONFIG += c++17
RUTSRC = ../../src
INCLUDEPATH += $$RUTSRC

SOURCES += \
	$$RUTSRC/ExitHelper.cpp \
	$$RUTSRC/Log.cpp \
	$$RUTSRC/Sets.cpp \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	$$RUTSRC/ExitHelper.h \
	$$RUTSRC/Log.h \
	$$RUTSRC/Sets.h \
	mainwindow.h

FORMS += \
	mainwindow.ui
