#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Text to Baudot (ITA2)");
    w.resize(800, 500);
    w.show();
    return app.exec();
}