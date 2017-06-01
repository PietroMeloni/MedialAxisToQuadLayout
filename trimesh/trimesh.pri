DEFINES += TRIMESH_DEFINED
MODULES +=  TRIMESH

!contains(DEFINES, COMMON_DEFINED){
    error(Trimesh module requires common module!)
}

HEADERS += \
    $$PWD/trimesh/trimesh.h \
    $$PWD/trimesh/load_save_trimesh.h \
    #$$PWD/skel.h \
    #$$PWD/drawableskel.h \
    #$$PWD/cgalskeletoninterface.h \
    #$$PWD/skelmanager.h

SOURCES += \
    $$PWD/trimesh/load_save_trimesh.cpp \
    #$$PWD/skel.cpp \
    #$$PWD/drawableskel.cpp \
    #$$PWD/cgalskeletoninterface.cpp \
    #$$PWD/skelmanager.cpp

contains(DEFINES, VIEWER_DEFINED){
    #WARNING: BUG on qmake: comment the following line if viewer is not included
    include ($$PWD/trimesh/gui/trimeshmanager.pri)
}

INCLUDEPATH += $$PWD
