#include "CAl.h"
#include <QtWidgets/QApplication>
#include <QTableWidget>
#include<QMessageBox>
#include <iostream>
#include<QApplication>
using namespace std;
int main(int argc, char **argv)
{
    
    QApplication a(argc, argv);
   
    CAl w;
   
    w.show();
    
    return a.exec();
}

