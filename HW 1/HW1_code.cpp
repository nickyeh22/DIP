#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "QMessageBox"

#include <QtCharts>
#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>

#include "qfiledialog.h"
#include "QDebug"
#include <QByteArray>

#include <QImage>
#include <QPixmap>
#include <QValueAxis>
#include <QSlider>
#include <QList>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton1_clicked()
{
    QString file64= QFileDialog::getOpenFileName(this,tr("Select file"), "1", tr("Images File (*.64)"));     //打開檔案並讀作字串
    QFile file(file64);                                                                                      //change the readed file into QFile
    QByteArray reader64;                                                                                     //create an 1-D Array(以二進位方式讀取值)
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, "warning", "failed reading");
        return;
    }


    while (!file.atEnd()) {
    reader64.append(file.readLine());                // 將每行讀入的字元串在一起，一一存入 reader64(bytearray) 裡面
    reader64 = reader64.simplified();                // 將 reader64 中去掉首尾的空格，中間連續的空格也用一個空格替換。
    }


    for(int i=0; i < 64; i++){
        for(int j=0; j<64; j++){
            location[i][j] = reader64[i*64+j];        //change 1-D Arrary to 2-D Array
            if(location[i][j] < 58)
                location[i][j] = location[i][j] - 48; //把ASCII碼轉成灰階值
            else {
                location[i][j] = location[i][j] - 55;
            }
        }
    }


    //用QImage把灰階值一一轉成灰階pixel
    QImage InputImage(64,64,QImage::Format_RGB32);
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
           InputImage.setPixel(j, i, qRgb(location[i][j]*8,location[i][j]*8,location[i][j]*8)); //pixel value*8
        }
    }


    //show the image on the ui label
    QPixmap InputImage_pixmap = QPixmap::fromImage(InputImage);
    ui->inputlabel1->setPixmap(InputImage_pixmap.scaled(ui->inputlabel1->width(),ui->inputlabel1->height(),Qt::KeepAspectRatio));


    //計算每個灰階值的數量
    for(int i=0; i<32; i++ ) grayscalevalue[i] = 0;	//先令一個陣列grayscalevalue[]，使得grayscalevalue[i]代表i這個灰階值出現的次數。而計算前要先把這個陣列歸零(從0開始算)
    for(int i=0; i<64; i++)
    {
        for(int j=0; j<64; j++)
        {
            grayscalevalue[ location[i][j] ]++;
        }
    }


    //畫 Histogram
    QBarSet *set0 = new QBarSet("Histogram");               //建立QChart資料的set

    for(int i=0; i<32; i++)
    *set0 << grayscalevalue[i];                             //建立每個set的數值

    QBarSeries *series = new QBarSeries();
    series->append(set0);                                   //把每筆set data加到不同的series(此處只有一個series)

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::AllAnimations);      //也可以選擇NoAnimation, GridAxisAnimations, SeriesAnimations
    chart->legend()->setVisible(false);                     //把圖例隱藏

    QStringList axisvalues;
    for(int i=0;i<32;i++){
        QString values=QString::number(i);
        axisvalues<<values;}
    QBarCategoryAxis *axisX = new QBarCategoryAxis();       //x-axis value
    axisX->append(axisvalues);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();                   //y-axis value
    axisY->setRange(0,4096);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    delete ui->horizontalLayout->takeAt(0);                 //把原本的histogram刪掉，不然每當數值調動，就會多跑出一個histogram
    ui->horizontalLayout->addWidget(chartView);

}


void MainWindow::on_pushButton2_clicked()
{
    QString file64= QFileDialog::getOpenFileName(this,tr("Select file"), "1", tr("Images File (*.64)"));
    QFile file(file64);
    QByteArray reader64;
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, "warning", "failed reading");
        return;
    }


    while (!file.atEnd()) {
    reader64.append(file.readLine());
    reader64 = reader64.simplified();
    }


    for(int i=0; i < 64; i++){
        for(int j=0; j<64; j++){
            location2[i][j] = reader64[i*64+j];         //change 1-D Arrary to 2-D Array
            if(location2[i][j] < 58)
                location2[i][j] = location2[i][j] - 48; //把ASCII碼轉成灰階值
            else {
                location2[i][j] = location2[i][j] - 55;
            }
        }
    }

    QImage InputImage(64,64,QImage::Format_RGB32);
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
           InputImage.setPixel(j, i, qRgb(location2[i][j]*8,location2[i][j]*8,location2[i][j]*8)); //pixel value*8
        }
    }

    QPixmap InputImage_pixmap = QPixmap::fromImage(InputImage);
    ui->inputlabel2->setPixmap(InputImage_pixmap.scaled(ui->inputlabel2->width(),ui->inputlabel2->height(),Qt::KeepAspectRatio));

}



//因為後面會很常用QImage把灰階值一一轉成灰階pixel，故把這個步驟直接宣告為一個function
void MainWindow::printimage(int outputArray[64][64]){
    QImage OutputImage(64,64,QImage::Format_RGB32);
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
           OutputImage.setPixel(j, i, qRgb(outputArray[i][j]*8,outputArray[i][j]*8,outputArray[i][j]*8)); //pixel value*8
        }
    }
    QPixmap OutputImage_pixmap=QPixmap::fromImage(OutputImage);
    ui->outputlabel->setPixmap(OutputImage_pixmap.scaled(ui->outputlabel->width(),ui->outputlabel->height(),Qt::KeepAspectRatio));//show the image on OutputLabel

}


//把"畫outputlabel中影像的Histogram"這個步驟直接宣告為一個function
void MainWindow::showhistogram(int grayscalevalue[])
{
    for(int i=0; i<32; i++ ) grayscalevalue[i] = 0;
    for(int i=0; i<64; i++)
    {
        for(int j=0; j<64; j++)
        {
            grayscalevalue[ outputArray[i][j] ]++;
        }
    }


    QBarSet *set0 = new QBarSet("Histogram");           //建立QChart資料的set

    for(int i=0; i<32; i++)
    *set0 << grayscalevalue[i];                         //建立每個set的數值

    QBarSeries *series = new QBarSeries();
    series->append(set0);                               //把每筆set data加到不同的series(此處只有一個series)

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::AllAnimations);  //也可以選擇NoAnimation, GridAxisAnimations, SeriesAnimations
    chart->legend()->setVisible(false);                 //把圖例隱藏

    QStringList axisvalues;
    for(int i=0;i<32;i++){
        QString values=QString::number(i);
        axisvalues<<values;}
    QBarCategoryAxis *axisX = new QBarCategoryAxis();   //x-axis value
    axisX->append(axisvalues);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();               //y-axis value
    axisY->setRange(0,4096);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    delete ui->horizontalLayout->takeAt(0);             //把原本的histogram刪掉，不然每當數值調動，就會多跑出一個histogram
    ui->horizontalLayout->addWidget(chartView);

}



void MainWindow::on_slider1_valueChanged(int delta)
{

    for(int i=0; i<64; i++){
        for(int j=0;j<64; j++){
            newlocation[i][j] = location[i][j] + delta;
            if(newlocation[i][j]>=31)                       //如果原灰階值加減一定值後超出0-31的範圍，就用最接近的極值替代(-3 -> 0)
                outputArray[i][j] = 31;
            else if (newlocation[i][j]<=0)
                outputArray[i][j] = 0;
            else if (newlocation[i][j]>0 && newlocation[i][j]<31)
                outputArray[i][j] = newlocation[i][j];
        }
    }

     ui->currentvalue1->setText(QString::number(delta));

     MainWindow::MainWindow::printimage(outputArray);   // call the OutputImage function

     MainWindow::MainWindow::showhistogram(grayscalevalue);

}



void MainWindow::on_slider2_valueChanged(int multiplicator)
{
    for(int i=0; i<64; i++){
        for(int j=0;j<64; j++){
            newlocation[i][j] = location[i][j] * multiplicator/10   ;
            if(newlocation[i][j]>=31)
                outputArray[i][j] = 31;
            else if (newlocation[i][j]<=0)
                outputArray[i][j] = 0;
            else if (newlocation[i][j]>0 && newlocation[i][j]<31)
                outputArray[i][j] = newlocation[i][j];
        }
    }

     ui->currentvalue2->setText(QString::number(multiplicator));

     MainWindow::MainWindow::printimage(outputArray);   // call the OutputImage function


     MainWindow::MainWindow::showhistogram(grayscalevalue);

}


void MainWindow::on_AverageButton_clicked()
{
    for(int i=0; i<64; i++){
        for(int j=0;j<64; j++){
            outputArray[i][j] = (location[i][j] +  location2[i][j])/ 2;
        }
    }

     MainWindow::MainWindow::printimage(outputArray);   // call the OutputImage function


     MainWindow::MainWindow::showhistogram(grayscalevalue);

}


void MainWindow::on_FunctionalButton_clicked()
{
    for(int i=1; i<64; i++){
        for(int j=0;j<64; j++){
            newlocation[i][j] = location[i][j] - location[i-1][j] ;
            if (newlocation[i][j]<=0){
                outputArray[i][j] = 0;}
            else{
                outputArray[i][j] = newlocation[i][j];
            }
        }
    }

    MainWindow::MainWindow::printimage(outputArray);    // call the OutputImage function

    MainWindow::MainWindow::showhistogram(grayscalevalue);


}

