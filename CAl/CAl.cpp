#include "CAl.h"
#include <QTableWidget>
#include<QMessageBox>
#include "ui_CAl.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <QSpinBox>
#include <QMainWindow>
#include<QPushButton>
#include <qspinbox.h>



using namespace std;
using namespace Ui;
CAl::CAl(QWidget* parent) :
    QMainWindow(parent)
{
    ui->setupUi(this);




    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(setRows()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(setColumns()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(startDecision()));




}
CAl::~CAl()
{
    delete ui;
    for (int i = 0; i < this->rows; i++)
    {
        delete[] this->c[i];
    }
    delete[] this->c;
    delete[] this->a;
    delete[] this->b;
}
void CAl::setRows()
{
    int rows = ui->spinBox->value();
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget_2->setRowCount(1);
    ui->tableWidget_2->setColumnCount(rows);
}
void CAl::setColumns()
{
    int cols = ui->spinBox_2->value();
    ui->tableWidget->setColumnCount(cols);
    ui->tableWidget_3->setRowCount(1);
    ui->tableWidget_3->setColumnCount(cols);

}
void CAl::startDecision()
{
    Create();
    CostMatrixInitialization();
    TransportationInitialization();
    NWcorner r;

}
void CAl::CostMatrixInitialization()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            this->c[i][j] = ui->tableWidget->item(i, j)->text().toInt();

}
void CAl::TransportationInitialization()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
       this-> a[i] = ui->tableWidget_2->item(1, i)->text().toInt();
    for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        this->b[j] = ui->tableWidget_3->item(1, j)->text().toInt();
}
void CAl::ShowNWcornerMatrix(int** X, int** C, int sum)
{
    QString str, str_1, str_2, str_3, str_4;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if (X[i][j] != 0)
            {
                str_1.setNum(i);
                str_2.setNum(j);
                str_3 = " -> ";
                str_4.setNum(X[i][j]);
                str = str_1 + str_3 + str_2 + '(' + str_4 + ')' + '\n';
                ui->textBrowser_4->setText(str);
                str_1.clear();
                str_2.clear();
                str_4.clear();
            }
        }
    str_1 = "The cost of transportation is  " + str_2.setNum(sum);
    ui->textBrowser_4->setText(str_1);

}
void CAl::Create()
{
    cols = ui->spinBox_2->value();
    rows = ui->spinBox->value();
    c = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        c[i] = new int[cols];
    }
    a = new int[rows];
    b = new int[cols];
}
NWcorner::NWcorner()
{

    Create();
    Calculate();
}
NWcorner::~NWcorner()
{
    for (int i = 0; i < this->rows; i++)
    {
        delete[] this->X[i];
    }
    delete[] this->X;
    for (int i = 0; i < this->rows; i++)
    {
        delete[] this->W[i];
    }
    delete[] this->W;
    delete[] this->u;
    delete[] this->v;
}
void NWcorner::Create()
{
    X = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        X[i] = new int[cols];
    }
    W = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        W[i] = new int[cols];
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            this->X[i][j] = 0;
        }
    }

}
void NWcorner::Calculate()

{

    for (int i = 0, j = 0; (i < rows) && (j < cols); )
    {

        if (b[j] - a[i] < 0)
        {
            X[i][j] = b[j];
            a[i] = a[i] - b[j];
            j++;
        }
        if (b[j] - a[i] > 0)
        {
            X[i][j] = a[i];
            b[j] = b[j] - a[i];
            i++;
        }
        if (b[j] - a[i] == 0)
        {
            if (i != rows - 1 && j != cols - 1)
                X[i + 1][j] = -1;

            X[i][j] = a[i];
            i++;
            j++;
        }
    }

    Potential();
}
void NWcorner::Potential()
{
    u = new int[rows];
    v = new int[cols];
    for (int i = 0; i < rows; i++)
        u[i] = 0;
    for (int j = 0; j < cols; j++)
        v[j] = 0;
    //filling potentials
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {

            if ((i >= 1) && (j == 0))
                for (int cj = 0; cj < cols; cj++)
                {
                    if ((X[i][cj] != 0) && (v[cj] != 0) && (c[i][j] != u[i] + v[j]))
                    {
                        u[i] = c[i][cj] - v[cj];
                        break;
                    }
                }
            if (X[i][j] == -1)
                v[j] = c[i][j] - u[i];
            if (X[i][j] == 0)
                continue;
            if ((v[j] == 0) && (c[i][j] != u[i] + v[j]))
                v[j] = c[i][j] - u[i];

        }
    Optimal();

}
void NWcorner::Optimal()
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            W[i][j] = u[i] + v[j];

    int s = 0;
    int* op;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (W[i][j] > c[i][j])
                s++;
    if (s > 0)
    {
        op = new int[s];
        for (int i = 0; i < s; i++)
            op[i] = 0;
        for (int i = 0, f = 0; (i < rows) && (f < s); i++)
            for (int j = 0; (j < cols) && (f < s); j++)
                if (W[i][j] > c[i][j])
                {
                    op[f] = W[i][j] - c[i][j];
                    f++;
                }
        int maxEl = op[0];
        for (int g = 0; g < s; g++)
            if (op[g] > maxEl)
                maxEl = op[g];

        int Ai = 0;
        int Aj = 0;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (W[i][j] > c[i][j])
                {
                    if ((W[i][j] - c[i][j]) == maxEl)
                    {
                        Ai = i;
                        Aj = j;
                    }
                }

        delete[] op;
        delete[] this->u;
        delete[] this->v;
        Correct(Ai, Aj);
    }

    if (s == 0)
    {


        Sum();
    }



}
void NWcorner::Correct(int i, int j)
{
    int t;
    bool key = false;
    for (int r = 1; r < rows; r++)
    {
        if (key) break;
        for (int q = 1; q < cols; q++)
        {
            if ((i + r < rows) && (j + q < cols) && !key)
            {
                if ((X[i][j + q] != 0) && (X[i + r][j + q] != 0) && (X[i + r][j] != 0))
                {
                    if (X[i][j + q] < 0)
                        X[i][j + q] = 0;
                    else if (X[i + r][j + q] < 0)
                        X[i + r][j + q] = 0;
                    else if (X[i + r][j] < 0)
                        X[i + r][j] = 0;
                    t = min(X[i][j + q], X[i + r][j]);

                    X[i][j] += t;
                    X[i + r][j + q] += t;
                    X[i + r][j] -= t;
                    X[i][j + q] -= t;
                    if (X[i][j + q] == X[i + r][j])
                        X[i][j + q] = -1;


                    key = true;
                    break;
                }

            }
            if ((i + r < rows) && (j - q >= 0) && !key)
            {
                if ((X[i][j - q] != 0) && (X[i + r][j - q] != 0) && (X[i + r][j] != 0))
                {
                    if (X[i][j - q] < 0)
                        X[i][j - q] = 0;
                    else if (X[i + r][j - q] < 0)
                        X[i + r][j - q] = 0;
                    else if (X[i + r][j] < 0)
                        X[i + r][j] = 0;
                    t = min(X[i][j - q], X[i + r][j]);

                    X[i][j] += t;
                    X[i + r][j - q] += t;
                    X[i + r][j] -= t;
                    X[i][j - q] -= t;
                    if (X[i][j - q] == X[i + r][j])
                        X[i][j - q] = -1;

                    key = true;
                    break;
                }


            }
            if ((i - r >= 0) && (j + q < cols))
            {
                if ((X[i][j + q] != 0) && (X[i - r][j + q] != 0) && (X[i - r][j] != 0))
                {
                    if (X[i][j + q] < 0)
                        X[i][j + q] = 0;
                    else if (X[i - r][j + q] < 0)
                        X[i - r][j + q] = 0;
                    else if (X[i - r][j] < 0)
                        X[i - r][j] = 0;
                    t = min(X[i][j + q], X[i - r][j]);

                    X[i][j] += t;
                    X[i - r][j + q] += t;
                    X[i - r][j] -= t;
                    X[i][j + q] -= t;
                    if (X[i - r][j] == X[i][j + q])
                        X[i - r][j] = -1;

                    key = true;
                    break;
                }

            }
            if ((i - r >= 0) && (j - q > 0) && !key)
            {
                if ((X[i][j - q] != 0) && (X[i - r][j - q] != 0) && (X[i - r][j] != 0))
                {
                    if (X[i][j - q] < 0)
                        X[i][j - q] = 0;
                    else if (X[i - r][j - q] < 0)
                        X[i - r][j - q] = 0;
                    else if (X[i - r][j] < 0)
                        X[i - r][j] = 0;
                    t = min(X[i][j - q], X[i - r][j]);

                    X[i][j] += t;
                    X[i - r][j - q] += t;
                    X[i - r][j] -= t;
                    X[i][j - q] -= t;
                    if (X[i - r][j] == X[i][j - q])
                        X[i - r][j] = -1;

                    key = true;
                    break;
                }

            }
        }
    }
    Potential();
}


void NWcorner::Sum()
{
    int sum = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            if (X[i][j] >= 0)
                sum += X[i][j] * c[i][j];
            else
                sum += 0;
        }
    CAl::ShowNWcornerMatrix(X, c, sum);

}
