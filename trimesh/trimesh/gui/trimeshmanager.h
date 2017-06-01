#ifndef TRIMESHMANAGER_H
#define TRIMESHMANAGER_H

#include <QFrame>
#include <viewer/mainwindow.h>
#include "drawable_trimesh.h"
#include "drawableskel.h"

namespace Ui {
    class TrimeshManager;
}

class TrimeshManager : public QFrame
{
        Q_OBJECT

    public:
        explicit TrimeshManager(QWidget *parent = 0);
        ~TrimeshManager();
        void setButtonsTrimeshLoaded(bool b);
        template <typename  T>
        void setTriMesh(const Trimesh<T>& m);

        inline DrawableTrimesh** getTrimesh()
        {
            return &trimesh;
        }

    private slots:
        void on_butLoadTrimesh_clicked();

        void on_butSaveTrimesh_clicked();

        void on_butClearTrimesh_clicked();

        void on_cbWireframe_stateChanged(int arg1);

        void on_rbPoints_toggled(bool checked);

        void on_rbFlat_toggled(bool checked);

        void on_rbSmooth_toggled(bool checked);

        void on_butSetVColor_clicked();

        void on_butSetTcolor_clicked();

        void on_rbVertexColor_toggled(bool checked);

        void on_rbTriangleColor_toggled(bool checked);

        void on_hsWireframeWidth_valueChanged(int value);

        void on_butSetWireframeColor_clicked();

    private:
        Ui::TrimeshManager *ui;
        MainWindow* mainWindow;
        DrawableTrimesh* trimesh;
        DrawableSkel* dsk;

};

template <typename  T>
inline void TrimeshManager::setTriMesh(const Trimesh<T>& m) {
    if (trimesh != nullptr){
        mainWindow->deleteObj(trimesh);
        delete trimesh;
    }
    trimesh = new DrawableTrimesh(m);
    mainWindow->pushObj(trimesh, "TriMesh");
    setButtonsTrimeshLoaded(true);
    mainWindow->updateGlCanvas();
}

#endif // TRIMESHMANAGER_H
