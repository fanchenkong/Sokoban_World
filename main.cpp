/**
 * @Name: Fanchen Kong
 * @UID: 404506008
 * @This is a project for sokoban. The user can choose difficulty and then try to push the boxes to the star position.
*/

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
