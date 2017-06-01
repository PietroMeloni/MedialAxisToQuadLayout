///Test common.pri
#ifdef COMMON_DEFINED
#include <common/arrays.h>
#include <common/bounding_box.h>
#include <common/comparators.h>
#include <common/timer.h>
#endif

///Test viewer.pri
#ifdef VIEWER_DEFINED
#include <viewer/interfaces/drawable_object.h>
#include <viewer/interfaces/pickable_object.h>
#include <viewer/mainwindow.h>
#include <viewer/managers/windowmanager.h>
#endif

///Test dcel.pri
#ifdef DCEL_DEFINED
#include <dcel/dcel.h>
///Test dcelviewer.pri: uncomment to test dcel module
#ifdef VIEWER_DEFINED
#include <dcel/gui/dcelmanager.h>
#endif
#endif

//Test trimesh.pri
#ifdef TRIMESH_DEFINED
#include <trimesh/trimesh.h>
///Test trimeshviewer.pri: uncomment to test trimesh module
#ifdef VIEWER_DEFINED
#include <trimesh/gui/trimeshmanager.h>
#endif
#endif

//Test eigenmesh.pri
#ifdef EIGENMESH_DEFINED
#include <eigenmesh/eigenmesh.h>
///Test trimeshviewer.pri: uncomment to test trimesh module
#ifdef VIEWER_DEFINED
#include <eigenmesh/gui/eigenmeshmanager.h>
#endif
#endif

//Test booleansmanager.pri
#if defined(IGL_DEFINED) && defined(CGAL_DEFINED)&& defined(VIEWER_DEFINED)
#include <eigenmesh/gui/booleansmanager.h>
#endif



//test skeleton.pri
#ifdef SKELETON_DEFINED
#include <skel.h>
#ifdef TRIMESH_DEFINED
#include <skelmanager.h>
#endif
#endif


int main(int argc, char *argv[]) {

    ///Test viewer.pri:
    #ifdef VIEWER_DEFINED
    QApplication app(argc, argv);

    MainWindow gui;  // finestra principale, contiene la canvas di QGLViewer


    WindowManager wm(&gui); // Creo un window manager e lo aggiungo alla mainwindow
    int id = gui.addManager(&wm, "Window");

    //Test trimeshmanager.pri
    #ifdef TRIMESH_DEFINED
    TrimeshManager tm(&gui);
    id = gui.addManager(&tm, "Trimesh");
    #endif

    //Test eigenmeshmanager.pri
    #ifdef EIGENMESH_DEFINED
    EigenMeshManager em(&gui);
    id = gui.addManager(&em, "EigenMesh");
    #endif

    //Test booleansmanager.pri
    #ifdef IGL_DEFINED
    BooleansManager bm(&gui);
    id = gui.addManager(&bm, "Booleans");
    #endif

    //Test dcelmanager.pri
    #ifdef DCEL_DEFINED
    DcelManager dm(&gui);
    id = gui.addManager(&dm, "Dcel");
    #endif

    //Test dcelmanager.pri
    #ifdef SKELETON_DEFINED
    #ifdef TRIMESH_DEFINED
    SkeletonManager sk(&gui);
    id = gui.addManager(&sk, "Skeleton");
    #endif
    #endif

    gui.setCurrentIndexToolBox(id); // il window manager sarà quello visualizzato di default
    gui.updateGlCanvas();
    gui.show();

    return app.exec();
    #else
    std::cout << "Hello World!" << std::endl;
    return 0;
    #endif
}
