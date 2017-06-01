DEFINES += VIEWER_DEFINED
CONFIG += VIEWER_DEFINED
MODULES += VIEWER

!contains(DEFINES, COMMON_DEFINED){
    error(Viewer module requires common module!)
}

QT += core gui opengl xml widgets

unix:!macx{
    QMAKE_CXXFLAGS += -std=c++11
    LIBS += /usr/lib/x86_64-linux-gnu/libGLU.so
    LIBS += -lQGLViewer
}

macx{
    CONFIG += c++11
    INCLUDEPATH += -I /libs/include/boost
    INCLUDEPATH += /libs/frameworks/QGLViewer/QGLViewer.framework/Headers
    LIBS += -F/libs/frameworks/QGLViewer -framework QGLViewer
}

HEADERS += \
    $$PWD/viewer/glcanvas.h \
    $$PWD/viewer/mainwindow.h \
    $$PWD/viewer/objects/drawabledebugobjects.h \
    $$PWD/viewer/managers/windowmanager.h \
    $$PWD/viewer/objects/objects.h \
    $$PWD/viewer/interfaces/drawable_object.h \
    $$PWD/viewer/interfaces/pickable_object.h \
    $$PWD/viewer/wait_dialog.h \
    $$PWD/viewer/interfaces/drawable_mesh.h

SOURCES += \
    $$PWD/viewer/glcanvas.cpp \
    $$PWD/viewer/mainwindow.cpp \
    $$PWD/viewer/objects/drawabledebugobjects.cpp \
    $$PWD/viewer/managers/windowmanager.cpp \
    $$PWD/viewer/wait_dialog.cpp \
    $$PWD/viewer/interfaces/drawable_mesh.cpp

FORMS += \
    $$PWD/viewer/mainwindow.ui \
    $$PWD/viewer/managers/windowmanager.ui

INCLUDEPATH += $$PWD
