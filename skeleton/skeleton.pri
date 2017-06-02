DEFINES += SKELETON_DEFINED
MODULES +=  SKELETON

!contains(DEFINES, TRIMESH_DEFINED){
    error(skeleton module requires trimesh module!)
}

HEADERS += \
    $$PWD/skeleton/skel.h \
    $$PWD/skeletonmanager/skelmanager.h \
    $$PWD/cgalskeletoninterface.h \
    $$PWD/skeletonmanager/drawableskel.h \
    $$PWD/skeletoncreator.h

SOURCES += \
    $$PWD/skeleton/skel.cpp \
    $$PWD/skeletonmanager/skelmanager.cpp \
    $$PWD/cgalskeletoninterface.cpp \
    $$PWD/skeletonmanager/drawableskel.cpp \
    $$PWD/skeletoncreator.cpp

FORMS += \
    $$PWD/skelmanager.ui
