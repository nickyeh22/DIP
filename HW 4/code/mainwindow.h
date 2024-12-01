#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <complex.h>

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
    QImage *SpectrumImage;
    QImage *PhaseAngleImage;

    double **input_array;
    double ** spectrum_array;
    double ** phaseangle_array;
    double **output_array;
    double ** FreqReal;
    double ** FreqImag;
    double f_max;
    double f_min;
    int H_L_pass = 0;
    int filter_index = 0;
    int n = 1;
    double **z_array;
    int gamma_H;
    int gamma_L;
    bool rev = false;

private slots:
    void on_pushButton_clicked();

    void FastFourierTransform(double ** InputImg, double ** spectrum, double ** phaseangle, int h, int w);

    void show_processed_spec_pa(Complex ** output);

    void on_reversetrans_clicked();

    void on_subtraction_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_comboBox_currentIndexChanged(int index);

    void on_filters_currentIndexChanged(int index);

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
