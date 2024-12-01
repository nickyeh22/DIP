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
#include <QImage>
#include <QPixmap>
#include <QColor>

#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

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




void MainWindow::on_read_input_clicked()
{
    mode=0;
    QString file= QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW2", tr("Images File (*.JPEG)"));     //打開檔案並讀作字串
    QImage image(file);
    propimage = image.scaled(1920, 1080, Qt::IgnoreAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage);
    ui->input->setPixmap(InputImage.scaled(ui->input->width(),ui->input->height(),Qt::KeepAspectRatio));

//    int width = propimage.width();
//    int height = propimage.height();
//    qDebug() << "hi";
//    qDebug() << width;
//    qDebug() << height;

//    QImage OutputImage(1920,1080,QImage::Format_RGB32);

//    for(int j = 0; j < 1080; j++)
//    {
//      for(int i = 0; i < 1920; i++)
//      {
//          QRgb pixcolors = propimage.pixel(i,j);
//              int r = qRed(pixcolors);
//              int g = qGreen(pixcolors);
//              int b = qBlue(pixcolors);

//          int grayvalue=  (r+g+b)/3;

//          OutputImage.setPixel(i, j, qRgb(grayvalue,grayvalue,grayvalue));
//      }
//    }

//    QPixmap OnputImage_pixmap = QPixmap::fromImage(OutputImage);
//    ui->output->setPixmap(OnputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

}


void MainWindow::showhistogram(int countgrayvalue[])
{
    QBarSet *set0 = new QBarSet("Histogram");           //建立QChart資料的set

    for(int i=0; i<256; i++)
    *set0 << countgrayvalue[i];                         //建立每個set的數值

    QBarSeries *series = new QBarSeries();
    series->append(set0);                               //把每筆set data加到不同的series(此處只有一個series)

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::AllAnimations);  //也可以選擇NoAnimation, GridAxisAnimations, SeriesAnimations
    chart->legend()->setVisible(false);                 //把圖例隱藏

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    delete ui->histogram->takeAt(0);             //把原本的histogram刪掉，不然每當數值調動，就會多跑出一個histogram
    ui->histogram->addWidget(chartView);

}

void MainWindow::on_grayscaleA_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    //    int grayvalue[1920][1080];

    //    int ** grayvalue;
    //    grayvalue = new int * [height];     //此矩陣有 n 個列(rows); 先 new 出 n 個 int *

    //    for (int i=0; i<=height; i++)
    //    {   grayvalue[i] = new int [width];     //每一列有 m 行(columns); array[i] 指向新 new 出的 m 個 int

    //    }
//        qDebug() << width;
//        qDebug() << height;

//        int **grayvalueA = (int **) malloc(sizeof(int *) *width );
//           for (int i = 0; i < width; i++) {
//               grayvalueA[i] = (int *) malloc(sizeof(int) * height);
//           }


        QImage OutputImage(1920,1080,QImage::Format_RGB32);
        mode=1;

        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA=  (int)(r+g+b)/3;

              OutputImage.setPixel(i, j, qRgb(grayvalueA,grayvalueA,grayvalueA));

              countgrayvalue[grayvalueA]++;

//              qDebug() << "i"<< i<<"/" <<j<<":" << grayvalue[i][j];

          }
        }

    //          qDebug() << grayvalue[300][300];

//        for(int j = 0; j < 1080; j++){
//          for(int i = 0; i < 1920; i++){

//               OutputImage.setPixel(i, j, qRgb(grayvalueA[i][j],grayvalueA[i][j],grayvalueA[i][j]));
//            }
//        }


        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

//        for (int i = 0; i < width; i++) {
//                free(grayvalueA[i]);
//            }
//            free(grayvalueA);
//            return;

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

}



void MainWindow::on_grayscaleB_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    mode=2;
//    int **grayvalueB = (int **) malloc(sizeof(int *) *width );
//       for (int i = 0; i < width; i++) {
//           grayvalueB[i] = (int *) malloc(sizeof(int) * height);
//       }


    QImage OutputImage(1920,1080,QImage::Format_RGB32);

    for(int j = 0; j < 1080; j++)
    {
      for(int i = 0; i < 1920; i++)
      {
          QRgb pixcolors = propimage.pixel(i,j);
              r = qRed(pixcolors);
              g = qGreen(pixcolors);
              b = qBlue(pixcolors);

           grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);

          OutputImage.setPixel(i, j, qRgb(grayvalueB,grayvalueB,grayvalueB));

          countgrayvalue[grayvalueB]++;

      }
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    MainWindow::MainWindow::showhistogram(countgrayvalue);
    for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;


}


void MainWindow::on_compare_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage OutputImage(1920,1080,QImage::Format_RGB32);
    mode=3;

    for(int j = 0; j < 1080; j++)
    {
      for(int i = 0; i < 1920; i++)
      {
          QRgb pixcolors = propimage.pixel(i,j);
              r = qRed(pixcolors);
              g = qGreen(pixcolors);
              b = qBlue(pixcolors);

           grayvalueA=  (int)(r+g+b)/3;
           grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);

          if(grayvalueA-grayvalueB>=0)
              delta = grayvalueA-grayvalueB;
          else{
              delta = grayvalueB-grayvalueA;
          }

          OutputImage.setPixel(i, j, qRgb(delta,delta,delta));

          countgrayvalue[delta]++;

      }
    }


    QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    MainWindow::MainWindow::showhistogram(countgrayvalue);
    for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

}



void MainWindow::on_threshold_valueChanged(int thresholdvalue)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage OutputImage(1920,1080,QImage::Format_RGB32);

    if(mode == 1){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA=  (int)(r+g+b)/3;
               if(grayvalueA>=thresholdvalue)
                   grayvalueA=255;
               else {
                   grayvalueA=0;}

               OutputImage.setPixel(i, j, qRgb(grayvalueA,grayvalueA,grayvalueA));

               countgrayvalue[grayvalueA]++;
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else if(mode == 2){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);
               if(grayvalueB>=thresholdvalue)
                   grayvalueB=255;
               else {
                   grayvalueB=0;}

              OutputImage.setPixel(i, j, qRgb(grayvalueB,grayvalueB,grayvalueB));

              countgrayvalue[grayvalueB]++;

          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;


    }

    else if (mode == 3){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA=  (int)(r+g+b)/3;
               grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);

              if(grayvalueA-grayvalueB>=0)
                  delta = grayvalueA-grayvalueB;
              else{
                  delta = grayvalueB-grayvalueA;
              }
//              qDebug() <<"delta=" << delta;
              if(delta>=thresholdvalue)
                  delta=255;
              else {
                  delta=0;}

              OutputImage.setPixel(i, j, qRgb(delta,delta,delta));

              countgrayvalue[delta]++;
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else {
    QMessageBox::warning(this, "warning", "Threshold can only be applied on grayscale image");
    }
}




void MainWindow::on_resolution_valueChanged(int arg)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage res_image = propimage.scaled((1920*pow(2,arg)), (1080*pow(2,arg)));

//    int height = res_image.height();
//    qDebug()<< height;

    QPixmap OuputImage_pixmap = QPixmap::fromImage(res_image);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::IgnoreAspectRatio));
}





void MainWindow::on_graylevel_valueChanged(int value)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage OutputImage(1920,1080,QImage::Format_RGB32);

    if(mode==1){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA= (r+g+b)/3;

               int newvalue= (round((grayvalueA*value/255.0f)))*255/value;



               OutputImage.setPixel(i, j, qRgb(newvalue,newvalue,newvalue));

               countgrayvalue[newvalue]++;
          }
        }

//        qDebug()<<(round)(254*2/255.0f);
//        qDebug()<<(round)(254*2/255)= 1


        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;
    }

    else if(mode==2){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);

               int newvalue= (round((grayvalueB*value/255.0f)))*255/value;

               OutputImage.setPixel(i, j, qRgb(newvalue,newvalue,newvalue));

               countgrayvalue[newvalue]++;
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else if(mode==0){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

              int newvaluer= (round((r*value/255.0f)))*255/value;
              int newvalueg= (round((g*value/255.0f)))*255/value;
              int newvalueb= (round((b*value/255.0f)))*255/value;

              OutputImage.setPixel(i, j, qRgb(newvaluer,newvalueg,newvalueb));

             }
           }

           QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
           ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

       }

}



void MainWindow::on_brightness_valueChanged(int bright)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage OutputImage(1920,1080,QImage::Format_RGB32);

    if(mode==1){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA= (r+g+b)/3;
               grayvalueA += bright;

               if(grayvalueA>=255)
                   grayvalueA=255;
               else if(grayvalueA<=0)
                   grayvalueA=0;
               else{
                   grayvalueA=grayvalueA;
               }

               OutputImage.setPixel(i, j, qRgb(grayvalueA,grayvalueA,grayvalueA));

               countgrayvalue[grayvalueA]++;

          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else if(mode==2){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);
               grayvalueB += bright;

               if(grayvalueB>=255)
                   grayvalueB=255;
               else if(grayvalueB<=0)
                   grayvalueB=0;
               else{
                   grayvalueB=grayvalueB;
               }

               OutputImage.setPixel(i, j, qRgb(grayvalueB,grayvalueB,grayvalueB));

               countgrayvalue[grayvalueB]++;

          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else if (mode==0){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

                  r += bright;
                  g += bright;
                  b += bright;

                  if(r>=255)
                      r=255;
                  else if(r<=0)
                      r=0;
                  else{
                      r=r;
                  }

                  if(g>=255)
                      g=255;
                  else if(g<=0)
                      g=0;
                  else{
                      g=g;
                  }

                  if(b>=255)
                      b=255;
                  else if(b<=0)
                      b=0;
                  else{
                      b=b;
                  }

                  OutputImage.setPixel(i, j, qRgb(r,g,b));
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    }


}


void MainWindow::on_contrast_valueChanged(int contrast)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage OutputImage(1920,1080,QImage::Format_RGB32);

    if(mode==1){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA= (r+g+b)/3;
               grayvalueA *= (contrast/10.0);

               if(grayvalueA>=255)
                   grayvalueA=255;
               else if(grayvalueA<=0)
                   grayvalueA=0;
               else{
                   grayvalueA=grayvalueA;
               }

               OutputImage.setPixel(i, j, qRgb(grayvalueA,grayvalueA,grayvalueA));

               countgrayvalue[grayvalueA]++;

          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else if(mode==2){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);
               grayvalueB *= (contrast/10.0);

               if(grayvalueB>=255)
                   grayvalueB=255;
               else if(grayvalueB<=0)
                   grayvalueB=0;
               else{
                   grayvalueB=grayvalueB;
               }

               OutputImage.setPixel(i, j, qRgb(grayvalueB,grayvalueB,grayvalueB));

               countgrayvalue[grayvalueB]++;

          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;

    }

    else if (mode==0){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

                  r *= (contrast/10.0);
                  g *= (contrast/10.0);
                  b *= (contrast/10.0);

                  if(r>=255)
                      r=255;
                  else if(r<=0)
                      r=0;
                  else{
                      r=r;
                  }

                  if(g>=255)
                      g=255;
                  else if(g<=0)
                      g=0;
                  else{
                      g=g;
                  }

                  if(b>=255)
                      b=255;
                  else if(b<=0)
                      b=0;
                  else{
                      b=b;
                  }

                  OutputImage.setPixel(i, j, qRgb(r,g,b));
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    }
}





void MainWindow::on_Equalization_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }
    QImage OutputImage(1920,1080,QImage::Format_RGB32);

    if(mode == 1){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA=  (int)(r+g+b)/3;

               pdf[grayvalueA]++;
          }
        }

        for(int i = 0; i < 256; i++)
        {
              pdf[i]/=2073600;
          }

        cdf[0]=pdf[0];

        for(int i = 1; i < 256; i++)
        {
              cdf[i]=cdf[i-1]+pdf[i];
              newgrayvalue[i]=round(cdf[i]*255);
          }

        newgrayvalue[0]=round(cdf[0]*255);

        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueA=  (int)(r+g+b)/3;
               int k = grayvalueA;

              OutputImage.setPixel(i, j, qRgb(newgrayvalue[k],newgrayvalue[k],newgrayvalue[k]));
              countgrayvalue[newgrayvalue[k]]++;
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;
    }

    else if(mode == 2){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                    r = qRed(pixcolors);
                    g = qGreen(pixcolors);
                    b = qBlue(pixcolors);

                grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);

                pdf[grayvalueB]++;
          }
        }

        for(int i = 0; i < 256; i++)
        {
              pdf[i]/=2073600;
          }

        cdf[0]=pdf[0];

        for(int i = 1; i < 256; i++)
        {
              cdf[i]=cdf[i-1]+pdf[i];
              newgrayvalue[i]=round(cdf[i]*255);
          }

        newgrayvalue[0]=round(cdf[0]*255);

        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

               grayvalueB=  (int)(0.299*r+0.587*g+0.114*b);
               int k = grayvalueB;

              OutputImage.setPixel(i, j, qRgb(newgrayvalue[k],newgrayvalue[k],newgrayvalue[k]));
              countgrayvalue[newgrayvalue[k]]++;
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        MainWindow::MainWindow::showhistogram(countgrayvalue);
        for(int i=0; i<256; i++ ) countgrayvalue[i] = 0;
    }

    else if (mode==0){
        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  int G = qGray(pixcolors);

                  pdf_G[G]++;

          }
        }

        for(int i = 0; i < 256; i++)
        {
              pdf_G[i]/=2073600;
          }

        cdf_G[0]=pdf_G[0];

        for(int i = 1; i < 256; i++)
        {
              cdf_G[i]=cdf_G[i-1]+pdf_G[i];
              newgrayvalue_G[i]=round(cdf_G[i]*255);
          }

        newgrayvalue_G[0]=round(cdf_G[0]*255);

        for(int j = 0; j < 1080; j++)
        {
          for(int i = 0; i < 1920; i++)
          {
              QRgb pixcolors = propimage.pixel(i,j);
                  r = qRed(pixcolors);
                  g = qGreen(pixcolors);
                  b = qBlue(pixcolors);

              OutputImage.setPixel(i, j, qRgb(newgrayvalue_G[r],newgrayvalue_G[g],newgrayvalue_G[b]));
          }
        }

        QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

   }

}





void MainWindow::on_Reset_clicked()
{
    mode=0;
    QPixmap OuputImage_pixmap = QPixmap::fromImage(propimage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));
}

