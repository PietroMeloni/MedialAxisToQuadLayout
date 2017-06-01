MODULES +=  BOOLEANS_MANAGER

!contains(DEFINES, VIEWER_DEFINED){
    error(BooleansManager module requires Viewer module!)
}

!contains(DEFINES, CGAL_DEFINED){
    error(BooleansManager module requires Cgal module!)
}

!contains(DEFINES, IGL_DEFINED){
    error(BooleansManager module requires LibIGL installed!)
}

FORMS += \
    $$PWD/booleansmanager.ui

HEADERS += \
    $$PWD/booleansmanager.h

SOURCES += \
    $$PWD/booleansmanager.cpp
