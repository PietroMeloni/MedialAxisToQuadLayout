#ifndef SKELMANAGER_H
#define SKELMANAGER_H


#include <QFrame>
#include <QFileDialog>
#include <viewer/mainwindow.h>
#include "drawableskel.h"
#include "trimesh/trimesh.h"
#include "cgalskeletoninterface.h"
#include <QErrorMessage>
#include <QDebug>
#include <QString>

namespace Ui {
    class SkeletonManager;
}

class SkeletonManager : public QFrame
{
        Q_OBJECT

    public:
        explicit SkeletonManager(QWidget *parent = 0);
        ~SkeletonManager();
        void setButtonsSkelLoaded(bool b);
        void setSkel(const Skel* m);



    private:

        Ui::SkeletonManager *ui;
        MainWindow* mainWindow;
        DrawableTrimesh* trimesh;
        DrawableSkel* dsk;
        const char* pathOfMyOFFFile;

    public slots:

        void on_butMakeSkel_clicked();
        void on_butNumCompr_clicked();
        void on_butDeleteDTri_clicked();
};


inline void SkeletonManager::setSkel(const Skel* m)
{
    if (dsk != nullptr)
    {
        mainWindow->deleteObj(dsk);
        delete dsk;
    }
    dsk = new DrawableSkel(m);
    mainWindow->pushObj(dsk, "Skel");
    setButtonsSkelLoaded(true);
    mainWindow->updateGlCanvas();
}

#endif // SKELMANAGER_H
