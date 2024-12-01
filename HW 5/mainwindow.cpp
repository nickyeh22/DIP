#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "QMessageBox"
#include <algorithm>

#include "qfiledialog.h"

#include "QDebug"
#include <QImage>
#include <QPixmap>
#include <QColor>

#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QColorDialog>

#include "opencv2/opencv.hpp"
#include <opencv2/core/mat.hpp>
#include "opencv2/imgproc.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::show_grayscalebar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    file = QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW5", tr("Images File (*.bmp)"));     //打開檔案並讀作字串
    QImage image(file);
    propimage = image.scaled(512, 512, Qt::KeepAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage);
    ui->input->setPixmap(InputImage.scaled(ui->input->width(),ui->input->height(),Qt::KeepAspectRatio));
}


void MainWindow::show_grayscalebar()
{
    double h = double (ui->grayscalebar->height());
    double w = double (ui->grayscalebar->width());

    QImage grayscalebar(w, h, QImage::Format_RGB32);

    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            grayscalebar.setPixel(i, j, qRgb(255*i/w,255*i/w,255*i/w));
        }
    }

    QPixmap grayscalebar_pixmap = QPixmap::fromImage(grayscalebar);
    ui->grayscalebar->setPixmap(grayscalebar_pixmap.scaled(ui->grayscalebar->width(),ui->grayscalebar->height(),Qt::KeepAspectRatio));

}

void MainWindow::on_comboBox_activated(int index)
{
    double h = double (ui->colorbar->height());
    double w = double (ui->colorbar->width());

    colorbar = QImage (w, h, QImage::Format_RGB32);

    if(index == 1)
    {
        for(int j = 0; j < h; j++)
        {
            for(int i = 0; i < w; i++)
            {
                if(i <= int (w/4))
                    colorbar.setPixel(i, j, qRgb(0,255*4*i/w,255));

                if(i > int (w/4) && i <= int (w/2))
                    colorbar.setPixel(i, j, qRgb(0,255,round(255 * (2 - (4 * (i / w))))));

                if(i > int (w/2) && i <= int ( 3*w/4 ) )
                    colorbar.setPixel(i, j, qRgb(round( ((4*i/w)-2)*255), 255, 0));

                if(i > int (3*w/4))
                    colorbar.setPixel(i, j, qRgb(255,round (255*(4-(4*i/w))), 0) );
            }
        }

        QPixmap colorbar_pixmap = QPixmap::fromImage(colorbar);
        ui->colorbar->setPixmap(colorbar_pixmap.scaled(ui->colorbar->width(),ui->colorbar->height(),Qt::KeepAspectRatio));

    }

    if(index == 2)
    {
        QColor start_color = QColorDialog::getColor(Qt::white, this, "Choose a start color");
        QColor end_color = QColorDialog::getColor(Qt::white, this, "Choose an end color");


        int r1 = start_color.red();
        int g1 = start_color.green();
        int b1 = start_color.blue();

        int r2 = end_color.red();
        int g2 = end_color.green();
        int b2 = end_color.blue();

        for(int j = 0; j < h; j++)
        {
            for(int i = 0; i < w; i++)
            {
                colorbar.setPixel(i, j, qRgb(round(r1+(r2-r1)*i/w), round(g1+(g2-g1)*i/w), round(b1+(b2-b1)*i/w)));
            }
        }

        QPixmap colorbar_pixmap = QPixmap::fromImage(colorbar);
        ui->colorbar->setPixmap(colorbar_pixmap.scaled(ui->colorbar->width(),ui->colorbar->height(),Qt::KeepAspectRatio));
    }

}


void MainWindow::on_pseudo_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    QColor pseudo[256];
    int grayvalue;
    QImage OutputImage(propimage.width(),propimage.height(),QImage::Format_RGB32);

    for(int i = 0; i < 256; i++)
    {
        pseudo[i] = colorbar.pixelColor(round((double(621/256))*i),0);
    }

    for(int j = 0; j < propimage.height() ; j++)
    {
        for(int i = 0; i < propimage.width(); i++)
        {
            QRgb pixcolors = propimage.pixel(i,j);
            grayvalue = qGray(pixcolors);

            OutputImage.setPixel(i, j, qRgb(pseudo[grayvalue].red(),pseudo[grayvalue].green(),pseudo[grayvalue].blue()));
        }
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(OutputImage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));
}


void MainWindow::RGBtoHSV( float r, float g, float b)
{
    float min, max, delta;
    min = std::min({ r, g, b });
    max = std::max({ r, g, b });
    v = max;				// v
    delta = max - min;
    if( max != 0 )
        s = delta / max;		// s
    else {
        // r = g = b = 0		// s = 0, v is undefined
        s = 0;
        h = -1;
        return;
    }
    if( r == max )
        h = ( g - b ) / delta;		// between yellow & magenta
    else if( g == max )
        h = 2 + ( b - r ) / delta;	// between cyan & yellow
    else
        h = 4 + ( r - g ) / delta;	// between magenta & cyan
    h *= 60;				// degrees
    if( h < 0 )
        h += 360;
}

void MainWindow::on_comboBox_2_activated(int index)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    QImage spectrum1(propimage.width(),propimage.height(),QImage::Format_RGB32);
    QImage spectrum2(propimage.width(),propimage.height(),QImage::Format_RGB32);
    QImage spectrum3(propimage.width(),propimage.height(),QImage::Format_RGB32);

    if(index == 1)
    {
        for(int j = 0; j < propimage.height() ; j++)
        {
            for(int i = 0; i < propimage.width(); i++)
            {
                spectrum1.setPixel(i, j, qRgb(qRed(propimage.pixel(i,j)),qRed(propimage.pixel(i,j)),qRed(propimage.pixel(i,j))));
                spectrum2.setPixel(i, j, qRgb(qGreen(propimage.pixel(i,j)),qGreen(propimage.pixel(i,j)),qGreen(propimage.pixel(i,j))));
                spectrum3.setPixel(i, j, qRgb(qBlue(propimage.pixel(i,j)),qBlue(propimage.pixel(i,j)),qBlue(propimage.pixel(i,j))));
            }
        }

        ui->spectrum1->setPixmap(QPixmap::fromImage(spectrum1).scaled(ui->spectrum1->width(),ui->spectrum1->height(),Qt::KeepAspectRatio));
        ui->spectrum2->setPixmap(QPixmap::fromImage(spectrum2).scaled(ui->spectrum2->width(),ui->spectrum2->height(),Qt::KeepAspectRatio));
        ui->spectrum3->setPixmap(QPixmap::fromImage(spectrum3).scaled(ui->spectrum3->width(),ui->spectrum3->height(),Qt::KeepAspectRatio));

        ui->label_i->setText("R");
        ui->label_ii->setText("G");
        ui->label_iii->setText("B");

    }

    if(index == 2)
    {
        for(int j = 0; j < propimage.height() ; j++)
        {
            for(int i = 0; i < propimage.width(); i++)
            {
                spectrum1.setPixel(i, j, qRgb(255-qRed(propimage.pixel(i,j)),255-qRed(propimage.pixel(i,j)),255-qRed(propimage.pixel(i,j))));
                spectrum2.setPixel(i, j, qRgb(255-qGreen(propimage.pixel(i,j)),255-qGreen(propimage.pixel(i,j)),255-qGreen(propimage.pixel(i,j))));
                spectrum3.setPixel(i, j, qRgb(255-qBlue(propimage.pixel(i,j)),255-qBlue(propimage.pixel(i,j)),255-qBlue(propimage.pixel(i,j))));
            }
        }

        ui->spectrum1->setPixmap(QPixmap::fromImage(spectrum1).scaled(ui->spectrum1->width(),ui->spectrum1->height(),Qt::KeepAspectRatio));
        ui->spectrum2->setPixmap(QPixmap::fromImage(spectrum2).scaled(ui->spectrum2->width(),ui->spectrum2->height(),Qt::KeepAspectRatio));
        ui->spectrum3->setPixmap(QPixmap::fromImage(spectrum3).scaled(ui->spectrum3->width(),ui->spectrum3->height(),Qt::KeepAspectRatio));

        ui->label_i->setText("C");
        ui->label_ii->setText("M");
        ui->label_iii->setText("Y");
    }

    if(index == 3)
    {
        for(int j = 0; j < propimage.height() ; j++)
        {
            for(int i = 0; i < propimage.width(); i++)
            {
                float r = qRed(propimage.pixel(i,j));
                float g = qGreen(propimage.pixel(i,j));
                float b = qBlue(propimage.pixel(i,j));

                r/=255;
                g/=255;
                b/=255;

                MainWindow::RGBtoHSV(r, g, b);

                spectrum1.setPixel(i, j, qRgb(h*255/360,h*255/360,h*255/360));
                spectrum2.setPixel(i, j, qRgb(s*255,s*255,s*255));
                spectrum3.setPixel(i, j, qRgb(v*255,v*255,v*255));
            }
        }

        ui->spectrum1->setPixmap(QPixmap::fromImage(spectrum1).scaled(ui->spectrum1->width(),ui->spectrum1->height(),Qt::KeepAspectRatio));
        ui->spectrum2->setPixmap(QPixmap::fromImage(spectrum2).scaled(ui->spectrum2->width(),ui->spectrum2->height(),Qt::KeepAspectRatio));
        ui->spectrum3->setPixmap(QPixmap::fromImage(spectrum3).scaled(ui->spectrum3->width(),ui->spectrum3->height(),Qt::KeepAspectRatio));

        ui->label_i->setText("H");
        ui->label_ii->setText("S");
        ui->label_iii->setText("I");
    }

    if(index == 4)
    {
        for(int j = 0; j < propimage.height() ; j++)
        {
            for(int i = 0; i < propimage.width(); i++)
            {
                float r = qRed(propimage.pixel(i,j));
                float g = qGreen(propimage.pixel(i,j));
                float b = qBlue(propimage.pixel(i,j));

                r/=255;
                g/=255;
                b/=255;

                float x = 0.412453*r + 0.357580*g + 0.180423*b;
                float y = 0.212671*r + 0.715160*g + 0.072169*b;
                float z = 0.019334*r + 0.119193*g + 0.950227*b;

                x*=255;
                y*=255;
                z*=255;

                if(x>255)
                    x=255;
                if(y>255)
                    y=255;
                if(z>255)
                    z=255;

                spectrum1.setPixel(i, j, qRgb(x,x,x));
                spectrum2.setPixel(i, j, qRgb(y,y,y));
                spectrum3.setPixel(i, j, qRgb(z,z,z));
            }
        }

        ui->spectrum1->setPixmap(QPixmap::fromImage(spectrum1).scaled(ui->spectrum1->width(),ui->spectrum1->height(),Qt::KeepAspectRatio));
        ui->spectrum2->setPixmap(QPixmap::fromImage(spectrum2).scaled(ui->spectrum2->width(),ui->spectrum2->height(),Qt::KeepAspectRatio));
        ui->spectrum3->setPixmap(QPixmap::fromImage(spectrum3).scaled(ui->spectrum3->width(),ui->spectrum3->height(),Qt::KeepAspectRatio));

        ui->label_i->setText("X");
        ui->label_ii->setText("Y");
        ui->label_iii->setText("Z");
    }

    if(index == 5)
    {
        for(int j = 0; j < propimage.height() ; j++)
        {
            for(int i = 0; i < propimage.width(); i++)
            {
                float r = qRed(propimage.pixel(i,j));
                float g = qGreen(propimage.pixel(i,j));
                float b = qBlue(propimage.pixel(i,j));

                r/=255;
                g/=255;
                b/=255;

                float x = 0.412453*r + 0.357580*g + 0.180423*b;
                float y = 0.212671*r + 0.715160*g + 0.072169*b;
                float z = 0.019334*r + 0.119193*g + 0.950227*b;

                float t_x = x/0.3127;
                float t_y = y/0.3290;
                float t_z = z/0.3583;
//                qDebug() << t_x <<t_y <<t_z;
                float L;
                float A;
                float B;

                if((t_y) > 0.008856)
                L = 116*pow(t_y,1/3.0)-16;
                else{
                L = 903.3*t_y;
                }

                if((t_x) > 0.008856)
                    t_x = pow(t_x,1/3.0);
                else{
                t_x = 7.787*t_x + 16/116;
                }
                if((t_y) > 0.008856)
                    t_y = pow(t_y,1/3.0);
                else{
                t_y = 7.787*t_y + 16/116;
                }
                if((t_z) > 0.008856)
                    t_z = pow(t_z,1/3.0);
                else{
                t_z = 7.787*t_z + 16/116;
                }

                A = 500*(t_x-t_y);
                B = 200*(t_y-t_z);

                L = (L/100)*255;
                A = ((A+500)/1000)*255;
                B = ((B+200)/400)*255;

                if(L>255)
                    L=255;
                if(A>255)
                    A=255;
                if(B>255)
                    B=255;

                spectrum1.setPixel(i, j, qRgb(L,L,L));
                spectrum2.setPixel(i, j, qRgb(A,A,A));
                spectrum3.setPixel(i, j, qRgb(B,B,B));

            }
        }

        ui->spectrum1->setPixmap(QPixmap::fromImage(spectrum1).scaled(ui->spectrum1->width(),ui->spectrum1->height(),Qt::KeepAspectRatio));
        ui->spectrum2->setPixmap(QPixmap::fromImage(spectrum2).scaled(ui->spectrum2->width(),ui->spectrum2->height(),Qt::KeepAspectRatio));
        ui->spectrum3->setPixmap(QPixmap::fromImage(spectrum3).scaled(ui->spectrum3->width(),ui->spectrum3->height(),Qt::KeepAspectRatio));

        ui->label_i->setText("L");
        ui->label_ii->setText("A");
        ui->label_iii->setText("B");
    }

    if(index == 6)
    {
        for(int j = 0; j < propimage.height() ; j++)
        {
            for(int i = 0; i < propimage.width(); i++)
            {
                float r = qRed(propimage.pixel(i,j));
                float g = qGreen(propimage.pixel(i,j));
                float b = qBlue(propimage.pixel(i,j));

                float y = 0.299*r + 0.587*g + 0.114*b;
                float u = (-0.169)*r + (-0.331)*g + 0.5*b + 128;
                float v = 0.5*r + (-0.419)*g + (-0.081)*b +128;

                spectrum1.setPixel(i, j, qRgb(y,y,y));
                spectrum2.setPixel(i, j, qRgb(u,u,u));
                spectrum3.setPixel(i, j, qRgb(v,v,v));
            }
        }

        ui->spectrum1->setPixmap(QPixmap::fromImage(spectrum1).scaled(ui->spectrum1->width(),ui->spectrum1->height(),Qt::KeepAspectRatio));
        ui->spectrum2->setPixmap(QPixmap::fromImage(spectrum2).scaled(ui->spectrum2->width(),ui->spectrum2->height(),Qt::KeepAspectRatio));
        ui->spectrum3->setPixmap(QPixmap::fromImage(spectrum3).scaled(ui->spectrum3->width(),ui->spectrum3->height(),Qt::KeepAspectRatio));

        ui->label_i->setText("Y");
        ui->label_ii->setText("U");
        ui->label_iii->setText("V");

    }
}

//--------------kmeans-----------------//

void MainWindow::k_means(QString QStr){

    std::string image_from_file = QStr.toStdString();
    cv::Mat Img = imread(image_from_file, cv::IMREAD_COLOR);      //源影象
    const int K = ui -> k_value ->text().toInt();    //聚類個數

    QFile file( QStr );
    if (Img.empty() || K <= 0){
        QMessageBox::information(0, "error_reading", file.errorString());
    }

    cv::Mat ImgHSV;
    cvtColor(Img, ImgHSV, cv::COLOR_BGR2HSV);                 //將RGB空間轉換為HSV空間

    cv::Mat ImgData(Img.rows*Img.cols, 1, CV_32FC3);          //一個3通道的資料域
    cv::Mat_<cv::Vec3b>::iterator itImg = ImgHSV.begin<cv::Vec3b>();  //迭代器
    cv::Mat_<cv::Vec3f>::iterator itData = ImgData.begin<cv::Vec3f>();

    //將源影象的資料輸入給新建的ImgData
    for (; itImg != ImgHSV.end<cv::Vec3b>(); ++itImg, ++itData)
        *itData = *itImg;


    cv::Mat ImgLabel, ImgCenter;
    cv::kmeans(ImgData, K, ImgLabel, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0), 1, cv::KMEANS_PP_CENTERS, ImgCenter);

    cv::Mat ImgRes(Img.size(), CV_8UC3);
    cv::Mat_<cv::Vec3b>::iterator itRes = ImgRes.begin<cv::Vec3b>();
    cv::Mat_<int>::iterator itLabel = ImgLabel.begin<int>();
    for (; itLabel != ImgLabel.end<int>(); ++itLabel, ++itRes)
        *itRes = ImgCenter.at<cv::Vec3f>(*itLabel, 0);

    cvtColor(ImgRes, ImgRes, cv::COLOR_HSV2BGR);
    imwrite("out.jpg", ImgRes);

    QImage imgIn= QImage((uchar*) ImgRes.data, ImgRes.cols, ImgRes.rows, ImgRes.step, QImage::Format_RGB888);
    imgIn = imgIn.rgbSwapped();

    //QImage(const uchar *data, int width, int height, int bytesPerLine, QImage::Format format,
    //       QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)

    QPixmap Clustered_Image_pixmap = QPixmap::fromImage ( imgIn );
    ui -> output -> setPixmap(Clustered_Image_pixmap.scaled(ui -> output ->width() , ui -> output -> height(),Qt::KeepAspectRatio));
}

void MainWindow::on_pushButton_2_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    MainWindow::k_means(file);
}

