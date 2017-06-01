#ifndef SKELMANAGER_H
#define SKELMANAGER_H


#include <QFrame>
#include <viewer/mainwindow.h>
#include "drawableskel.h"
#include "trimesh/trimesh.h"

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
        template <typename  T>
        void setSkel(const Trimesh<T>& m);

        inline DrawableSkel** getSkel()
        {
            return &dsk;
        }
   private:

        Ui::SkeletonManager *ui;
        MainWindow* mainWindow;
        DrawableTrimesh* trimesh;
        DrawableSkel* dsk;
};

#endif // SKELMANAGER_H
