/*
 * @author    Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @copyright Alessandro Muntoni 2016.
 */

#include "dcelmanager.h"
#include "ui_dcelmanager.h"

#include <QFileDialog>
#include <QColorDialog>
#include <iostream>

/**
 * @brief Crea un DcelManager
 * @param parent
 */
DcelManager::DcelManager(QWidget *parent) : QFrame(parent), ui(new Ui::DcelManager), mainWindow((MainWindow*)parent), drawableDcel(nullptr) {
    ui->setupUi(this);
}

/**
 * @brief Elimina un DcelManager la DrawableDcel contenuta in esso
 */
DcelManager::~DcelManager() {
    delete ui;
    if (drawableDcel!=nullptr)
        delete drawableDcel;
}

/**
 * @brief restituisce il puntatore alla drawableDcel contenuta nel manager
 * @return la drawableDcel presente nel manager
 */
DrawableDcel *DcelManager::getDcel() {
    return drawableDcel;
}

/**
 * @brief Setta la Dcel contenuta nel manager, creando una \i copia della Dcel passata come parametro.
 *
 * Aggiorna in automatico la canvas, e setta i pulsanti del manager in modalità "dcelLoaded".
 * Elimina eventuali altre Dcel presenti precedentemente nel manager.
 *
 * @param[in] dcel: la dcel che verrà copiata all'interno del manager
 * @param[in] b: se true, viene visualizzata una dcel in base alla segmentazione
 */
void DcelManager::setDcel(const Dcel &dcel, const std::string &name, bool b) {
    cleanDcel();
    drawableDcel = new DrawableDcel(dcel);
    mainWindow->pushObj(drawableDcel, name);
    if (b){
        drawableDcel->setFlatShading();
        ui->smoothDcelRadioButton->setChecked(true);
    }
    mainWindow->updateGlCanvas();

    setButtonsDcelLoaded();
}

/**
 * @brief Elimina la Dcel contenuta nel manager.
 *
 * Aggiorna in automatico la canvasm e setta i pulsanti del manager in modalità "dcelNotLoaded".
 */
void DcelManager::cleanDcel() {
    setButtonsDcelNotLoaded();

    if (drawableDcel != nullptr){
        resetDefaults();
        drawableDcel->setVisible(false);
        mainWindow->updateGlCanvas();
        mainWindow->deleteObj(drawableDcel);
        delete drawableDcel;
        drawableDcel = nullptr;

    }
}

/**
 * @brief Applica le modifiche effettuate nella Dcel alla DrawableDcel.
 * Vedi DrawableDcel::update().
 */
void DcelManager::updateDcel() {
    if (drawableDcel != nullptr){
        drawableDcel->update();
    }
}

/**
 * @brief setta ai valori predefiniti tutti gli oggetti presenti nel QFrame del DcelManager
 */
void DcelManager::resetDefaults() {
    ui->wireframeDcelCheckBox->setChecked(false);
    ui->drawDcelCheckBox->setChecked(true);
    ui->flatDcelRadioButton->setChecked(true);
    ui->triangleColorDcelRadioButton->setChecked(true);
    ui->wireframeWidthDcelSlider->setValue(1);
}

/**
 * @brief Setta tutti i pulsandi del QFrame del DcelManager in modalità "dcelLoaded".
 *
 * Tutti i pulsanti che permettono di effettuare operazioni di modifica visualizzazione sulla Dcel
 * vengono attivati.
 * @warning tutti questi pulsanti assumono che all'interno della DcelManager sia presente una Dcel, per cui
 * non chiamare mai questo metodo se non è presente una dcel all'interno del manager.
 */
void DcelManager::setButtonsDcelLoaded() {
    ui->wireframeColorDcelButton->setEnabled(true);
    ui->drawDcelCheckBox->setEnabled(true);
    ui->wireframeDcelCheckBox->setEnabled(true);
    ui->wireframeWidthDcelSlider->setEnabled(true);
    ui->flatDcelRadioButton->setEnabled(true);
    ui->pointsDcelRadioButton->setEnabled(true);
    ui->smoothDcelRadioButton->setEnabled(true);
    ui->triangleColorDcelRadioButton->setEnabled(true);
    ui->vertexColorDcelRadioButton->setEnabled(true);
    ui->cleanDcelButton->setEnabled(true);
    ui->loadDcelButton->setEnabled(false);
    ui->saveDcelButton->setEnabled(true);
}

/**
 * @brief Setta tutti i pulsandi del QFrame del DcelManager in modalità "dcelNotLoaded".
 *
 * Tutti i pulsanti che permettono di effettuare operazioni di modifica visualizzazione sulla Dcel
 * vengono disattivati.
 */
void DcelManager::setButtonsDcelNotLoaded() {
    ui->wireframeColorDcelButton->setEnabled(false);
    ui->drawDcelCheckBox->setEnabled(false);
    ui->wireframeDcelCheckBox->setEnabled(false);
    ui->wireframeWidthDcelSlider->setEnabled(false);
    ui->flatDcelRadioButton->setEnabled(false);
    ui->pointsDcelRadioButton->setEnabled(false);
    ui->smoothDcelRadioButton->setEnabled(false);
    ui->triangleColorDcelRadioButton->setEnabled(false);
    ui->vertexColorDcelRadioButton->setEnabled(false);
    ui->cleanDcelButton->setEnabled(false);
    ui->loadDcelButton->setEnabled(true);
    ui->saveDcelButton->setEnabled(false);
}
/**
 * @brief Metodo chiamato quando viene premuto il pulsante LoadDcel
 */
void DcelManager::on_loadDcelButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "Open DCEL",
                       ".",
                       "OBJ(*.obj);;DCEL(*.dcel);;PLY(*.ply)");

    if (!filename.isEmpty())
    {
        if (drawableDcel != nullptr) { //Non dovrebbe mai succedere, in quanto il pulsante load dovrebbe essere disattivato quando è presente una dcel
            mainWindow->deleteObj(drawableDcel);
            delete drawableDcel;
        }
        drawableDcel = new DrawableDcel();
        std::string s = filename.toStdString();
        if(s.substr(s.find_last_of(".") + 1) == "obj") { //obj file
            drawableDcel->loadFromObjFile(s);
        }
        else if(s.substr(s.find_last_of(".") + 1) == "ply") { //ply file
            drawableDcel->loadFromPlyFile(s);
        }
        else if(s.substr(s.find_last_of(".") + 1) == "dcel") { //dcel file
            drawableDcel->loadFromDcelFile(s);
            if (! drawableDcel->isTriangleMesh())
                std::cerr << "No Triangle Mesh\n";
            else
                std::cerr << "Triangle Mesh\n";

        }
        else {
            assert(0);
        }
        std::cout << "load: " << filename.toStdString() << std::endl;
        //drawableDcel->setFacesWireframe(true);
        //drawableDcel->setSmoothShading();
        drawableDcel->update();
        mainWindow->pushObj(drawableDcel, filename.toStdString().substr(filename.toStdString().find_last_of("/") + 1));
        mainWindow->updateGlCanvas();

        setButtonsDcelLoaded();

    }
}

/**
 * @brief Metodo chiamato quando viene premuto il pulsante cleanDcel
 */
void DcelManager::on_cleanDcelButton_clicked() {
    cleanDcel();
}

/**
 * @brief Metodo chiamato quando viene premuto il pulsante saveDcel
 */
void DcelManager::on_saveDcelButton_clicked() {
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(nullptr,
                       "Save DCEL",
                       ".",
                       "DCEL(*.dcel);;PLY(*.ply);;OBJ(*.obj)", &selectedFilter);

    std::cout << "save: " << filename.toStdString() << std::endl;

    if (selectedFilter == "PLY(*.ply)") {
        drawableDcel->saveOnPlyFile(filename.toStdString());
    }
    else  if (selectedFilter == "OBJ(*.obj)") {
        drawableDcel->saveOnObjFile(filename.toStdString());
    }
    else {
        drawableDcel->saveOnDcelFile(filename.toStdString());
    }
}

void DcelManager::on_wireframeDcelCheckBox_stateChanged(int state) {
    drawableDcel->setWireframe(state == Qt::Checked);
    mainWindow->updateGlCanvas();
}

void DcelManager::on_wireframeWidthDcelSlider_valueChanged(int value) {
    drawableDcel->setWireframeWidth(value);
    mainWindow->updateGlCanvas();
}

void DcelManager::on_wireframeColorDcelButton_clicked() {
    QColor color = QColorDialog::getColor(Qt::white, this);

    drawableDcel->setWireframeColor(color.redF(), color.greenF(), color.blueF());
    mainWindow->updateGlCanvas();
}

void DcelManager::on_drawDcelCheckBox_stateChanged(int state) {
    drawableDcel->setVisible(state == Qt::Checked);
    mainWindow->updateGlCanvas();
}

void DcelManager::on_pointsDcelRadioButton_toggled(bool checked) {
    if (checked)
        drawableDcel->setPointsShading();
    mainWindow->updateGlCanvas();
}

void DcelManager::on_flatDcelRadioButton_toggled(bool checked) {
    if (checked)
        drawableDcel->setFlatShading();
    mainWindow->updateGlCanvas();
}

void DcelManager::on_smoothDcelRadioButton_toggled(bool checked) {
    if (checked)
        drawableDcel->setSmoothShading();
    mainWindow->updateGlCanvas();
}

void DcelManager::on_vertexColorDcelRadioButton_toggled(bool checked) {
    if (checked)
        drawableDcel->setEnableVertexColor();
    mainWindow->updateGlCanvas();
}

void DcelManager::on_triangleColorDcelRadioButton_toggled(bool checked) {
    if (checked)
        drawableDcel->setEnableTriangleColor();
    mainWindow->updateGlCanvas();
}
