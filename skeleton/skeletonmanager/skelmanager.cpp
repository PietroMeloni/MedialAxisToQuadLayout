#include "skelmanager.h"
#include "ui_skelmanager.h"

SkeletonManager::SkeletonManager(QWidget* parent) :
    QFrame(parent),
    ui(new Ui::SkeletonManager),
    mainWindow((MainWindow*)parent),
    trimesh(nullptr)
{
    ui->setupUi(this);
}

SkeletonManager::~SkeletonManager()
{

}

void SkeletonManager::setButtonsSkelLoaded(bool b)
{
    ui->butMakeSkel->setEnabled(b);

}
/*
 * Se clicco il pulsande MakeSkel, scegli la trimesh con estensione obj,
 * creane, costruisci il DrawableSkel corrispondente e passalo alla
 * main widow
 */
void SkeletonManager::on_butMakeSkel_clicked()
{
     QString filename = QFileDialog::getOpenFileName(mainWindow,
                        "Open Trimesh",
                        ".",
                        "OFF(*.off)");

     if(filename != NULL)
     {
       // qInfo(filename.toStdString().c_str());

         if (!filename.isEmpty())
         {
             dsk = new DrawableSkel(filename.toStdString().c_str());

             mainWindow->pushObj(dsk, "Skeleton");
             setButtonsSkelLoaded(true);
             mainWindow->updateGlCanvas();
         }
     }

}

