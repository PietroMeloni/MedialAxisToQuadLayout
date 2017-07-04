DEFINES += COMMON_DEFINED
CONFIG += COMMON_DEFINED

CONFIG += OPENMP

unix:!macx{
    QMAKE_CXXFLAGS += -std=c++11

    exists(/usr/include/eigen3){
        DEFINES += COMMON_WITH_EIGEN
        MODULES += COMMON_WITH_EIGEN
        INCLUDEPATH += -I /usr/include/eigen3
    }
    else{
        MODULES += COMMON_WITHOUT_EIGEN
    }
}

macx{
    CONFIG += c++11
    exists(/libs/include/eigen3){
        DEFINES += COMMON_WITH_EIGEN
        MODULES += COMMON_WITH_EIGEN
        INCLUDEPATH += -I /libs/include/eigen3/
    }
    else{
        MODULES += COMMON_WITHOUT_EIGEN
    }
}

OPENMP {
    unix:!macx{
        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS +=  -fopenmp
    }
}

HEADERS += \
    $$PWD/common/bounding_box.h \
    $$PWD/common/comparators.h \
    $$PWD/common/point.h \
    $$PWD/common/point2d.h \
    $$PWD/common/serialize.h \
    $$PWD/common/arrays.h \
    $$PWD/common/timer.h \
    $$PWD/common/utils.h \
    $$PWD/common/hashlib.h \
    $$PWD/common/file_conversions.h \
    $$PWD/common/serializable_object.h \
    $$PWD/common/load_save_file.h \
    $$PWD/common/color.h \
    $$PWD/common/trischar.h

SOURCES += \
    $$PWD/common/load_save_file.cpp \
    $$PWD/common/trischar.cpp

INCLUDEPATH += $$PWD


