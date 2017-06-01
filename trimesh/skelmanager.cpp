#include "skelmanager.h"


SkeletonManager::SkeletonManager(QWidget* parent) :
    QFrame(parent),
    ui(new Ui::SkeletonManager),
    mainWindow((MainWindow*)parent),
    trimesh(nullptr) {
    ui->setupUi(this);
}

SkeletonManager::~SkeletonManager()
{

}

void SkeletonManager::setButtonsSkelLoaded(bool b)
{

}

template<typename T>
void SkeletonManager::setSkel(const Trimesh<SkeletonManager::T> &m)
{

}
