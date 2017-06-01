#ifndef EIGENMESHMANAGER_H
#define EIGENMESHMANAGER_H

#include <QFrame>
#include <viewer/mainwindow.h>
#include "drawableeigenmesh.h"

namespace Ui {
    class EigenMeshManager;
}

class EigenMeshManager : public QFrame
{
        Q_OBJECT

    public:
        explicit EigenMeshManager(QWidget *parent = 0);
        void setButtonsMeshLoaded(bool b);
        void setEigenMesh(const EigenMesh &m);
        ~EigenMeshManager();

    private slots:

        void on_loadMeshButton_clicked();

        void on_clearMeshButton_clicked();

        void on_saveMeshButton_clicked();

        void on_pointsMeshRadioButton_toggled(bool checked);

        void on_flatMeshRadioButton_toggled(bool checked);

        void on_smoothMeshRadioButton_toggled(bool checked);

        void on_wireframeMeshCheckBox_stateChanged(int arg1);

        void on_verticesColorRadioButton_toggled(bool checked);

        void on_faceColorRadioButton_toggled(bool checked);

    private:
        Ui::EigenMeshManager *ui;
        MainWindow* mainWindow;
        DrawableEigenMesh* mesh;


};

#endif // EIGENMESHMANAGER_H
