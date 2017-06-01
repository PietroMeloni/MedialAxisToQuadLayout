MODULES +=  EIGEN_MESH_MANAGER

!contains(DEFINES, VIEWER_DEFINED){
    error(EigenMeshManager module requires Viewer module!)
}

FORMS += \
    $$PWD/eigenmeshmanager.ui

HEADERS += \
    $$PWD/eigenmeshmanager.h \
    $$PWD/drawableeigenmesh.h

SOURCES += \
    $$PWD/eigenmeshmanager.cpp \
    $$PWD/drawableeigenmesh.cpp
