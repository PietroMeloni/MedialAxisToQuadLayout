#include "eigenmeshmanager.h"
#include "ui_eigenmeshmanager.h"

#include <QFileDialog>

EigenMeshManager::EigenMeshManager(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EigenMeshManager),
    mainWindow((MainWindow*)parent),
    mesh (nullptr) {
    ui->setupUi(this);
}

void EigenMeshManager::setButtonsMeshLoaded(bool b) {
    ui->loadMeshButton->setEnabled(!b);
    ui->clearMeshButton->setEnabled(b);
    ui->saveMeshButton->setEnabled(b);
    ui->pointsMeshRadioButton->setEnabled(b);
    ui->flatMeshRadioButton->setEnabled(b);
    ui->smoothMeshRadioButton->setEnabled(b);
    ui->wireframeMeshCheckBox->setEnabled(b);
    ui->verticesColorRadioButton->setEnabled(b);
    ui->faceColorRadioButton->setEnabled(b);
    ui->wireframeMeshCheckBox->setChecked(false);
    ui->smoothMeshRadioButton->setChecked(true);
    ui->faceColorRadioButton->setChecked(true);
    ui->verticesColorRadioButton->setChecked(false);
}

void EigenMeshManager::setEigenMesh(const EigenMesh& m) {
    if (mesh != nullptr){
        mainWindow->deleteObj(mesh);
        delete mesh;
    }
    mesh = new DrawableEigenMesh(m);
    mainWindow->pushObj(mesh, "IGLMesh");
    setButtonsMeshLoaded(true);
    mainWindow->updateGlCanvas();

}

EigenMeshManager::~EigenMeshManager() {
    delete ui;
    if (mesh != nullptr){
        mainWindow->deleteObj(mesh);
        delete mesh;
    }
}

void EigenMeshManager::on_loadMeshButton_clicked() {
        QString filename = QFileDialog::getOpenFileName(nullptr,
                           "Open Eigen Mesh",
                           ".",
                           "OBJ(*.obj)");
        if (!filename.isEmpty()) {
            mesh = new DrawableEigenMesh();
            mesh->readFromObj(filename.toStdString());
            mesh->setEnableTriangleColor();
            mainWindow->pushObj(mesh, filename.toStdString().substr(filename.toStdString().find_last_of("/") + 1));
            setButtonsMeshLoaded(true);
            mainWindow->updateGlCanvas();
        }
}

void EigenMeshManager::on_clearMeshButton_clicked() {
    setButtonsMeshLoaded(false);
    mainWindow->deleteObj(mesh);
    delete mesh;
    mesh = nullptr;
}

void EigenMeshManager::on_saveMeshButton_clicked() {
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(nullptr,
                       "Save Eigen Mesh",
                       ".",
                       "PLY(*.ply);;OBJ(*.obj)", &selectedFilter);

    std::cout << "save: " << filename.toStdString() << std::endl;

    if (selectedFilter == "PLY(*.ply)") {
        mesh->saveOnPly(filename.toStdString());
    }
    else  if (selectedFilter == "OBJ(*.obj)") {
        //mesh->saveOnObj(filename.toStdString());
    }
}

void EigenMeshManager::on_pointsMeshRadioButton_toggled(bool checked){
    if (checked){
        mesh->setPointsShading();
        mainWindow->updateGlCanvas();
    }
}

void EigenMeshManager::on_flatMeshRadioButton_toggled(bool checked) {
    if (checked){
        mesh->setFlatShading();
        mainWindow->updateGlCanvas();
    }
}

void EigenMeshManager::on_smoothMeshRadioButton_toggled(bool checked) {
    if (checked){
        mesh->setSmoothShading();
        mainWindow->updateGlCanvas();
    }
}

void EigenMeshManager::on_wireframeMeshCheckBox_stateChanged(int arg1) {
    mesh->setWireframe(arg1 == Qt::Checked);
    mainWindow->updateGlCanvas();
}

void EigenMeshManager::on_verticesColorRadioButton_toggled(bool checked) {
    if (checked){
        mesh->setEnableVertexColor();
        mainWindow->updateGlCanvas();
    }
}

void EigenMeshManager::on_faceColorRadioButton_toggled(bool checked) {
    if (checked){
        mesh->setEnableTriangleColor();
        mainWindow->updateGlCanvas();
    }
}
