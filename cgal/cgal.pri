DEFINES += CGAL_DEFINED
CONFIG += CGAL_DEFINED
MODULES += CGAL

!contains(DEFINES, COMMON_DEFINED){
    error(Cgal module requires common module!)
}

unix:!macx{
    LIBS += -lmpfr -lgmp -lCGAL -frounding-math
    LIBS += -lboost_system -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread
    #DEFINES+=CGAL_DISABLE_ROUNDING_MATH_CHECK=ON
}

macx{
    DEFINES += CGAL_DEFINED
    INCLUDEPATH += -I /libs/include/CGAL/
    INCLUDEPATH += -I /libs/include/boost/
    LIBS += -frounding-math
    LIBS += -L/libs/lib/gmp -lgmp
    LIBS += -L/libs/lib/CGAL -lCGAL
    LIBS += -L/libs/lib/boost -lboost_system-mt -DBOOST_LOG_DYN_LINK -lboost_log-mt -lboost_thread-mt -lpthread
    LIBS += -L/libs/lib/mpfr -lmpfr
}

contains(DEFINES, COMMON_WITH_EIGEN){
    DEFINES += CGAL_EIGEN3_ENABLED
}

HEADERS += \
    $$PWD/cgal/aabbtree.h \
    $$PWD/cgal/cgalinterface.h

SOURCES += \
    $$PWD/cgal/aabbtree.cpp \
    $$PWD/cgal/cgalinterface.cpp

INCLUDEPATH += $$PWD
