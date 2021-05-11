#pragma once
#ifndef CAl_H
#define CAl_H
#include <QtWidgets/QMainWindow>
#include "ui_CAl.h"
#include <QTableWidget>
#include<QMessageBox>
#include <iostream>
#include <QMainWindow>
#include <QSpinBox>
#include <cstdlib>
#include <algorithm>
using namespace Ui;
using namespace std;
class CAl : public QMainWindow
{
    Q_OBJECT;

public:
    explicit  CAl(QWidget* parent = 0);
    Ui::CAlClass* ui;
    ~CAl();
    virtual void Create();
    void CostMatrixInitialization();
    void TransportationInitialization();
    void ShowNWcornerMatrix(int** X, int** C, int sum);
public slots:
    void setRows();
    void setColumns();
    void startDecision();
protected:
    int** c;//������� ��������� 
    int* a;//���������� ��������, ������ 
    int* b;//���������� ����������� ���������
    int rows;//������� �������
    int cols;//�������
};




class NWcorner :public CAl
{
private:
    int** X; //������� ��
    int** W; //������� �����������
    int* u;
    int* v;
public:
    NWcorner();
    ~NWcorner();

    void Potential(); // ������ �����������
    void Calculate(); // ������� 
    void Correct(int i, int j);// ����� �� t
    void print();
    void Optimal();
    void Create();
    void Sum();
};



#endif// CAl_H

