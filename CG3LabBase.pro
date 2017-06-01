CONFIG(debug, debug|release){
    DEFINES += DEBUG
}
CONFIG(release, debug|release){
    DEFINES -= DEBUG
    #just uncomment next lines if you want to ignore asserts and got a more optimized binary
    #CONFIG += FINAL_RELEASE
}
FINAL_RELEASE {
    unix:!macx{
        QMAKE_CXXFLAGS_RELEASE -= -g -O2
        QMAKE_CXXFLAGS += -O3 -DNDEBUG
    }
}
DEFINES+=CGAL_EIGEN3_ENABLED
    LIBS += -lmpfr -lgmp -lCGAL -frounding-math
    LIBS += -lboost_system -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread
#Add or remove all the modules you need
#Before pushing the project with your new module, please double check that everything works keeping uncommentend
#only the modules that are required by your module. Also please write here required and optional modules for your module

#Common module: contains classes and common functions used on all the other modules
#Optional: Eigen
include (common/common.pri)

#Viewer module: contains classes for a simple viewer
#Requires: Common module, libQGLViewer, boost
include (viewer/viewer.pri)

#Dcel module: contains a Double Connected-Edge List data structure
#Requires: Common module, boost;
#Optional: Cgal module, viewer module
include (dcel/dcel.pri)

#Cgal module: contains an interface to some functionalities of CGAL library
#Requires: Common module, libCgal; Optional: Dcel module
include (cgal/cgal.pri)

#Trimesh module: contains a Trimesh data structure
#Requires: Common module
#Optional: Viewer module
include (trimesh/trimesh.pri)

#EigenMesh module: contains a EigenMesh data structure
#Requires: Common module with Eigen
#Optional: Viewer module
#include (eigenmesh/eigenmesh.pri)

#Igl module: coontaint an intergace to some functionalities of libIGL
#Requires: Common module, libIGL (an environment variable named LIBIGL containing the root directory of the library must be setted)
#Optional: Viewer module, Cgal module, Dcel module
#include (igl/igl.pri)

include(skeleton/skeleton.pri)

#comment next line if you don't want to see the message that shows included modules
message(Included modules: $$MODULES)

SOURCES += \
    main.cpp \

