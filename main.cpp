#include "mainwindow.h"
#include "welcomescreen.h"
#include <QApplication>
#include "dbConnexion.h"

#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeScreen w;
    w.show();

    return a.exec();
}
