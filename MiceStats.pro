#-------------------------------------------------
#
# Project created by QtCreator 2011-04-30T22:59:36
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = MiceStats
TEMPLATE = app


SOURCES += main.cpp\
        micestats.cpp \
    micenamevalidator.cpp \
    micetitle.cpp

HEADERS  += micestats.h \
    micenamevalidator.h \
    micetitle.h

FORMS    += micestats.ui

RESOURCES += \
    resources.qrc

win32:RC_FILE += \
    win32rc.rc

OTHER_FILES += \
    micestats_en.ts \
    win32rc.rc
