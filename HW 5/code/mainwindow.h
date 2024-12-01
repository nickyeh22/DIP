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

    QString file;
    QImage propimage;
    QImage colorbar;

    float h;
    float s;
    float v;



private slots:

    void show_grayscalebar();

    void on_comboBox_activated(int index);

    void on_pushButton_clicked();

    void on_pseudo_clicked();

    void on_comboBox_2_activated(int index);

    void RGBtoHSV(float r, float g, float b);

    void k_means(QString QStr);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
