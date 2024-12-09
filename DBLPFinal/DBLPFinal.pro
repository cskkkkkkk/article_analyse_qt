QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Cluster_New.cpp \
    afauthorsorting.cpp \
    analyzecluster.cpp \
    analyzehot.cpp \
    author_sorting.cpp \
    basicsearch.cpp \
    cluster.cpp \
    data.cpp \
    fuzzy_search.cpp \
    initsystem.cpp \
    main.cpp \
    mainwindow.cpp \
    myhome.cpp \
    options_search.cpp

HEADERS += \
    AllHeader.h \
    afauthorsorting.h \
    analyzecluster.h \
    analyzehot.h \
    author_sorting.h \
    basicsearch.h \
    cluster.h \
    data.h \
    fuzzy_search.h \
    initsystem.h \
    mainwindow.h \
    myhome.h \
    options_search.h

FORMS += \
    analyzecluster.ui \
    analyzehot.ui \
    author_sorting.ui \
    initsystem.ui \
    mainwindow.ui \
    myhome.ui \
    options_search.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
