#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtablewidget.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage propimage;
    QImage *outputimage;

    int maskindex = 0;
    int regionindex = 0;
    double gaussian_K;
    double gaussian_sigma;
    int parameter_c;

    double ** mask;

    int ** array_zero;

    int laplacian = 0;


//    int r;
//    int g;
//    int b;

//    int ** array_r;
//    int ** array_g;
//    int ** array_b;




private slots:
    void on_selectinput_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_convolution_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_gaussian_K_valueChanged(double arg1);

    void on_gaussian_sigma_valueChanged(double arg2);

    void on_Laplacian_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_localenhancement_clicked();

    void on_parameter_c_valueChanged(int arg1);

    void on_sobeloperators_clicked();

    void on_zerocrossing_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
