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
    QString file2;
    QString file3;
    QImage propimage;
    QImage propimage2;
    QImage propimage3;

private slots:
    void on_pushButton_clicked();

    void on_trapezoidal_clicked();

    void on_wavy_clicked();

    void on_circular_clicked();

    void on_pushButton_2_clicked();

    void on_fusion_clicked();

    void on_hough_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
