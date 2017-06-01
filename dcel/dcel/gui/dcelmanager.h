/*
 * @author    Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @copyright Alessandro Muntoni 2016.
 */

#ifndef DCELMANAGER_H
#define DCELMANAGER_H

#include <QFrame>
#include <QColor>
#include "drawable_dcel.h"
#include <viewer/mainwindow.h>

namespace Ui {
    class DcelManager;
}

/**
 * @brief QFrame che gestisce una DrawableDcel visualizzata nella mainWindow
 */
class DcelManager : public QFrame {
        Q_OBJECT

    public:
        explicit DcelManager(QWidget* parent = 0);
        ~DcelManager();

        DrawableDcel* getDcel();
        void setDcel(const Dcel& dcel, const std::string& name = "Dcel", bool b = false);
        void cleanDcel();
        void updateDcel();
        void resetDefaults();
        void setButtonsDcelLoaded();
        void setButtonsDcelNotLoaded();

    private slots:
        void on_loadDcelButton_clicked();
        void on_cleanDcelButton_clicked();
        void on_saveDcelButton_clicked();
        void on_wireframeDcelCheckBox_stateChanged(int state);
        void on_wireframeWidthDcelSlider_valueChanged(int value);
        void on_wireframeColorDcelButton_clicked();
        void on_drawDcelCheckBox_stateChanged(int state);
        void on_pointsDcelRadioButton_toggled(bool checked);
        void on_flatDcelRadioButton_toggled(bool checked);
        void on_smoothDcelRadioButton_toggled(bool checked);
        void on_vertexColorDcelRadioButton_toggled(bool checked);
        void on_triangleColorDcelRadioButton_toggled(bool checked);

    private:
        Ui::DcelManager* ui; //puntatore al QFrame (da cui è possibile richiamare gli oggetti della ui)
        MainWindow* mainWindow; //puntatore alla mainWindow
        DrawableDcel* drawableDcel; //puntatore alla DrawableDcel gestita dal DcelManager
        int meshStackIndex; // indice identificativo del manager
};

#endif // DCELMANAGER_H
