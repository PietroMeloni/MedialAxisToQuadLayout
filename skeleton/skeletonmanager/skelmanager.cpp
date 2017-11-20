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
             //pathOfMyOFFFile = filename.toStdString().c_str();
             dsk = new DrawableSkel(filename.toStdString().c_str());

             mainWindow->pushObj(dsk, "Skeleton");
             setButtonsSkelLoaded(true);
             mainWindow->updateGlCanvas();
         }
     }

}

void SkeletonManager::on_butNumCompr_clicked()
{
    QString compressions = ui->lineNumCompr->text();
    QString trHo = ui->lineAngleTH->text();
    double th = trHo.toDouble();
    //qDebug() <<th;
    int compr = compressions.toInt();
    QString path1 = QFileDialog::getOpenFileName(mainWindow,
                       "Open Trimesh",
                       ".",
                       "OFF(*.off)");
    if(!path1.isEmpty())
    {

         dsk = new DrawableSkel(path1.toStdString().c_str(), compr, th);

         mainWindow->pushObj(dsk, "Skeleton");
         setButtonsSkelLoaded(true);
         mainWindow->updateGlCanvas();


    }
    else
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
}

void SkeletonManager::on_butDeleteDTri_clicked()
{
    QString compressions = ui->lineNumCompr->text();
    QString trHo = ui->lineAngleTH->text();
    double th = trHo.toDouble();
    //qDebug() <<th;
    int compr = compressions.toInt();
    QString path1 = QFileDialog::getOpenFileName(mainWindow,
                       "Open Trimesh",
                       ".",
                       "OFF(*.off)");
    if(!path1.isEmpty())
    {

         dsk = new DrawableSkel(path1.toStdString().c_str(), compr, th);
         dsk->compressSkeletonUntilConverge();

         mainWindow->pushObj(dsk, "Skeleton");
         setButtonsSkelLoaded(true);
         mainWindow->updateGlCanvas();


    }
    else
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
                dsk->compressSkeletonUntilConverge();
                mainWindow->pushObj(dsk, "Skeleton");
                setButtonsSkelLoaded(true);
                mainWindow->updateGlCanvas();
            }
        }
    }
}

void SkeletonManager::on_butTESTDeleteDTri_clicked()
{
    QString compressions = ui->lineNumCompr->text();
    QString trHo = ui->lineAngleTH->text();
    double th = trHo.toDouble();
    //qDebug() <<th;
    int compr = compressions.toInt();
    QString path1 = QFileDialog::getOpenFileName(mainWindow,
                                                   "Open Trimesh",
                                                   ".",
                                                   "OBJ(*.obj)");
    dsk = new DrawableSkel(path1.toStdString().c_str(), compr, th,0);
    dsk->compressSkeletonUntilConverge(1);
    mainWindow->pushObj(dsk, "Skeleton");
    setButtonsSkelLoaded(true);
    mainWindow->updateGlCanvas();




}

void SkeletonManager::on_butTESTNewIteration_clicked()
{

    dsk->compressSkeletonUntilConverge(1);
    mainWindow->updateGlCanvas();
}
