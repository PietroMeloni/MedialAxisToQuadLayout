DEFINES += EIGENMESH_DEFINED
CONFIG += EIGENMESH_DEFINED
MODULES += EIGENMESH

!contains(DEFINES, COMMON_WITH_EIGEN){
    error(EigenMesh module requires common module with eigen!)
}

unix:!macx{
    LIBS += -lboost_system -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread
}

exists($$(LIBIGL_HOME)){
    DEFINES += IGL_DEFINED
    CONFIG += IGL_DEFINED
    MODULES += IGL

    #comment next line if libigl is not used in static mode
    exists($$(LIBIGL_STATIC_HOME)){
        CONFIG += IGL_STATIC_LIBRARY
    }

    !contains(DEFINES, COMMON_DEFINED){
        error(Igl module requires common module!)
    }

    unix:!macx{
        LIBS += -lboost_system -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread
        INCLUDEPATH += $$(LIBIGL_HOME)/include/

        #newest versions of eigen are not supported by libigl
        INCLUDEPATH -= /usr/include/eigen3
        INCLUDEPATH += $$(LIBIGL_HOME)/external/nanogui/ext/eigen/
        message()

        IGL_STATIC_LIBRARY {
            DEFINES += IGL_STATIC_LIBRARY
            LIBS += -L$$(LIBIGL_STATIC_HOME) -ligl_cgal -ligl
        }
    }
}

HEADERS += \
    $$PWD/eigenmesh/eigenmesh.h \
    $$PWD/eigenmesh/algorithms/eigenmesh_algorithms.h

SOURCES += \
    $$PWD/eigenmesh/eigenmesh.cpp \
    $$PWD/eigenmesh/algorithms/eigenmesh_algorithms.cpp

#Note: QtCreator always shows this module included, however files included in it are compiled only if Viewer module is included
VIEWER_DEFINED{
    include ($$PWD/eigenmesh/gui/eigenmeshmanager.pri)
    CGAL_DEFINED {
        IGL_DEFINED {
            include ($$PWD/eigenmesh/gui/booleansmanager.pri)
        }
    }
}

INCLUDEPATH += $$PWD
