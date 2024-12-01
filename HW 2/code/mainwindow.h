#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

//        int width;
//        int height;
//          int grayvalueA[1920][1080];
//          int grayvalueB[1920][1080];
//          int grayvalue[1920][1080];



        int delta;
        int countgrayvalue[256]={0};
        int grayvalueA;
        int grayvalueB;
        int grayvalue;
        int r;
        int g;
        int b;
        int mode;
        double pdf[256]={0};
        double pdf_G[256]={0};
        double cdf[256]={0};
        double cdf_G[256]={0};
        int newgrayvalue[256]={0};
        int newgrayvalue_G[256]={0};


        QImage propimage;








private slots:
    void on_read_input_clicked();

    void showhistogram(int histovalue[]);

    void on_grayscaleA_clicked();

    void on_grayscaleB_clicked();

    void on_compare_clicked();

    void on_threshold_valueChanged(int thresholdvalue);

    void on_resolution_valueChanged(int arg);

    void on_graylevel_valueChanged(int value);

    void on_brightness_valueChanged(int value);

    void on_contrast_valueChanged(int value);

    void on_Equalization_clicked();


    void on_Reset_clicked();

private:
    Ui::MainWindow *ui;
//    int grayvalue[1080][1080] = {0};

};
#endif // MAINWINDOW_H
