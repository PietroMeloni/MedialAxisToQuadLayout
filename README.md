# CG3LabBase

How to install:

Ubuntu (at the moment, for every module except igl module):
sh install.sh

How to: creare un manager e collegarlo alla mainwindow
Includere il modulo del viewer
Se è necessario che il manager comunichi con la mainwindow seguire tutti i passi, altrimenti è sufficiente seguire solamente i passi 1 e 4.

1) File->New->Qt->Qt Designer Form Class; verranno creati tre file: mymanager.h, mymanager.cpp, mymanager.ui (se il nostro manager si chiama "MyManager")
2) Nel .h: includere "viewer/mainwindow.h" e
aggiungere un attributo privato: un puntatore a MainWindow, che chiameremo mainWindow
3) Nel .cpp: aggiungere nel costruttore l'inizializzazione del puntatore alla mainwindow:
mainWindow((MainWindow*)parent)
4) Aggiungere il manager alla MainWindow nel main:
MyManager m(&gui); //gui è una variabile di tipo MainWindow
int idManager = gui.addManager(m);



