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
#include <iomanip>
#include <cmath>
#include <numbers>
#include <math.h>

#include <QApplication>
#include <QWidget>

#include "opencv2/opencv.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/calib3d.hpp>
#include <opencv2/calib3d/calib3d.hpp>


using namespace std;
using namespace cv;

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


void MainWindow::on_pushButton_clicked()
{
    file = QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW6", tr("Images File (*.bmp *.jpg)"));     //打開檔案並讀作字串
    QImage image(file);
    propimage = image.scaled(512, 512, Qt::KeepAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage);
    ui->input->setPixmap(InputImage.scaled(ui->input->width(),ui->input->height(),Qt::KeepAspectRatio));
}


void MainWindow::on_trapezoidal_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    std::string image_from_file = file.toStdString();
    cv::Mat img = cv::imread(image_from_file, cv::IMREAD_COLOR);

    int offset = 200;

    // 設定變換[之前]與[之後]的坐標 (左上,左下,右下,右上)
    cv::Point2f pts1[] = {cv::Point2f(0, 0), cv::Point2f(0, img.rows), cv::Point2f(img.cols, img.rows), cv::Point2f(img.cols, 0)};
    cv::Point2f pts2[] = {cv::Point2f(0, 0), cv::Point2f(0 + offset, img.rows), cv::Point2f(img.cols - offset, img.rows), cv::Point2f(img.cols, 0)};

    // 透視變換行列計算
    cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
    cv::Mat dst_img;

    // 變換
    cv::warpPerspective(img, dst_img, perspective_matrix, img.size(), cv::INTER_LINEAR);

//    cv::imshow("dst", dst_img);
    QImage trans_img= QImage((uchar*) dst_img.data, dst_img.cols, dst_img.rows, dst_img.step, QImage::Format_BGR888);

    for (int j = 0; j < trans_img.width(); j++)
    {
        for (int i = 0; i < trans_img.height(); i++)
        {
            int gray = qGray(trans_img.pixel(i, j));
            trans_img.setPixel(i, j, QColor(gray, gray, gray).rgb());
        }
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(trans_img);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

}


void MainWindow::on_wavy_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    std::string image_from_file = file.toStdString();
    cv::Mat input = cv::imread(image_from_file, CV_LOAD_IMAGE_GRAYSCALE);

    double xo, yo;
    input.convertTo(input, CV_32FC1);
    copyMakeBorder(input, input, 3, 3, 3, 3, 0);

    int height = input.rows;
    int width = input.cols;

    cv::Mat out = cv::Mat(height, width, input.type());

    for(int j = 0; j < height; j++){
        for(int i =0; i < width; i++){
            xo = (30.0 * sin(2.0 * 3.1415926 * j / 200.0));
            yo = (30.0 * sin(2.0 * 3.1415926 * i / 200.0));
            out.at<float>(j,i) = (float)input.at<float>(((int)(j+yo+height)%height),((int)(i+xo+width)%width));

            int ix = std::min({width-1, std::max({0, (int) (i + xo)})});
            int iy = std::min({height-1, std::max({0, (int) (j + yo)})});
            out.at<float>(j,i) = (float)input.at<float>(iy,ix);

        }
    }
    normalize(out, out,0,255,cv::NORM_MINMAX,CV_8UC1);
//    imshow("output", out);
    QImage trans_img(out.cols, out.rows, QImage::Format_Indexed8);
    trans_img.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        trans_img.setColor(i, qRgb(i, i, i));
    }
    uchar *pSrc = out.data;
    for(int row = 0; row < out.rows; row++)
    {
        uchar *pDest = trans_img.scanLine(row);
        memcpy(pDest, pSrc, out.cols);
        pSrc += out.step;
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(trans_img);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

}


void MainWindow::on_circular_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    QImage image(file);
    propimage = image.scaled(512, 512, Qt::KeepAspectRatio);

    QImage imgbar;
    QImage imgbar2;
    QImage output(512,512,QImage::Format_RGB32);
    int w;

    for(int k = 0; k < 512; k++)
    {
        w = round(2*sqrt(pow(256,2) - pow((256-k),2)));
        imgbar = propimage.copy(0, k, 512, 1);
        imgbar2 = imgbar.scaled(w, 1, Qt::IgnoreAspectRatio);

        for(int i = 0; i < 512; i++)
        {
            if(i < round(256 - w/2))
                output.setPixel(i, k , qRgb(0, 0, 0));
            else if(i >= round(256 - w/2) && i < round(256 + w/2))
            {
                output.setPixel(i, k , qRgb(qGray(imgbar2.pixel(i - round(256 - w/2), 0)), qGray(imgbar2.pixel(i - round(256 - w/2), 0)), qGray(imgbar2.pixel(i - round(256 - w/2), 0))));
            }
            else
            {
                output.setPixel(i, k , qRgb(0, 0, 0));
            }
        }

    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(output);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

}



void MainWindow::on_pushButton_2_clicked()
{
    file2 = QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW6", tr("Images File (*.bmp *.jpg)"));     //打開檔案並讀作字串
    QImage image(file2);
    propimage2 = image.scaled(512, 512, Qt::KeepAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage2);
    ui->input_2->setPixmap(InputImage.scaled(ui->input_2->width(),ui->input_2->height(),Qt::KeepAspectRatio));
}


void MainWindow::on_pushButton_3_clicked()
{
    file3 = QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW6", tr("Images File (*.bmp *.jpg)"));     //打開檔案並讀作字串
    QImage image(file3);
    propimage3 = image.scaled(512, 512, Qt::KeepAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage3);
    ui->input_3->setPixmap(InputImage.scaled(ui->input_3->width(),ui->input_3->height(),Qt::KeepAspectRatio));
}


//--------------------------------DWT-------------------------------------//
// signum
//--------------------------------
float sgn(float x)
{
    float res=0;
    if(x==0)
    {
        res=0;
    }
    if(x>0)
    {
        res=1;
    }
    if(x<0)
    {
        res=-1;
    }
    return res;
}
//--------------------------------
// Soft shrinkage
//--------------------------------
float soft_shrink(float d,float T)
{
    float res;
    if(fabs(d)>T)
    {
        res=sgn(d)*(fabs(d)-T);
    }
    else
    {
        res=0;
    }

    return res;
}
//--------------------------------
// Hard shrinkage
//--------------------------------
float hard_shrink(float d,float T)
{
    float res;
    if(fabs(d)>T)
    {
        res=d;
    }
    else
    {
        res=0;
    }

    return res;
}
//--------------------------------
// Garrot shrinkage
//--------------------------------
float Garrot_shrink(float d,float T)
{
    float res;
    if(fabs(d)>T)
    {
        res=d-((T*T)/d);
    }
    else
    {
        res=0;
    }

    return res;
}
////--------------------------------
//// Wavelet transform
////--------------------------------
//static void cvHaarWavelet(Mat &src,Mat &dst,int NIter)
//{
//    float c,dh,dv,dd;
//    assert( src.type() == CV_32FC1 );
//    assert( dst.type() == CV_32FC1 );
//    int width = src.cols;
//    int height = src.rows;
//    for (int k=0;k<NIter;k++)
//    {
//        for (int y=0;y<(height>>(k+1));y++)
//        {
//            for (int x=0; x<(width>>(k+1));x++)
//            {
//                c=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)+src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
//                dst.at<float>(y,x)=c;

//                dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
//                dst.at<float>(y,x+(width>>(k+1)))=dh;

//                dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
//                dst.at<float>(y+(height>>(k+1)),x)=dv;

//                dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
//                dst.at<float>(y+(height>>(k+1)),x+(width>>(k+1)))=dd;
//            }
//        }
//        dst.copyTo(src);
//    }
//}
//--------------------------------
//Inverse wavelet transform
//--------------------------------
// Filter type

#define NONE 0  // no filter
#define HARD 1  // hard shrinkage
#define SOFT 2  // soft shrinkage
#define GARROT 3  // garrot filter

static void cvInvHaarWavelet(Mat &src,Mat &dst,int NIter, int SHRINKAGE_TYPE=0, float SHRINKAGE_T=50)
{
    float c,dh,dv,dd;
    assert( src.type() == CV_32FC1 );
    assert( dst.type() == CV_32FC1 );
    int width = src.cols;
    int height = src.rows;
    //--------------------------------
    // NIter - number of iterations
    //--------------------------------
    for (int k=NIter;k>0;k--)
    {
        for (int y=0;y<(height>>k);y++)
        {
            for (int x=0; x<(width>>k);x++)
            {
                c=src.at<float>(y,x);
                dh=src.at<float>(y,x+(width>>k));
                dv=src.at<float>(y+(height>>k),x);
                dd=src.at<float>(y+(height>>k),x+(width>>k));

               // (shrinkage)
                switch(SHRINKAGE_TYPE)
                {
                case HARD:
                    dh=hard_shrink(dh,SHRINKAGE_T);
                    dv=hard_shrink(dv,SHRINKAGE_T);
                    dd=hard_shrink(dd,SHRINKAGE_T);
                    break;
                case SOFT:
                    dh=soft_shrink(dh,SHRINKAGE_T);
                    dv=soft_shrink(dv,SHRINKAGE_T);
                    dd=soft_shrink(dd,SHRINKAGE_T);
                    break;
                case GARROT:
                    dh=Garrot_shrink(dh,SHRINKAGE_T);
                    dv=Garrot_shrink(dv,SHRINKAGE_T);
                    dd=Garrot_shrink(dd,SHRINKAGE_T);
                    break;
                }

                //-------------------
                dst.at<float>(y*2,x*2)=0.5*(c+dh+dv+dd);
                dst.at<float>(y*2,x*2+1)=0.5*(c-dh+dv-dd);
                dst.at<float>(y*2+1,x*2)=0.5*(c+dh-dv-dd);
                dst.at<float>(y*2+1,x*2+1)=0.5*(c-dh-dv+dd);
            }
        }
        Mat C=src(Rect(0,0,width>>(k-1),height>>(k-1)));
        Mat D=dst(Rect(0,0,width>>(k-1),height>>(k-1)));
        D.copyTo(C);
    }
}

static void cvHaarWavelet_2(Mat &src1, Mat &src2, Mat &src3, Mat &dst,int NIter)
{
    float c_1, dh_1, dv_1, dd_1;
    float c_2, dh_2, dv_2, dd_2;
    float c_3, dh_3, dv_3, dd_3;
    float c_out, dh_out, dv_out, dd_out;

    assert( src1.type() == CV_32FC1 );
    assert( dst.type() == CV_32FC1 );
    int width = src1.cols;
    int height = src1.rows;
    for (int k=0;k<NIter;k++)
    {
        for (int y=0;y<(height>>(k+1));y++)
        {
            for (int x=0; x<(width>>(k+1));x++)
            {
                c_1=(src1.at<float>(2*y,2*x)+src1.at<float>(2*y,2*x+1)+src1.at<float>(2*y+1,2*x)+src1.at<float>(2*y+1,2*x+1))*0.5;
                c_2=(src2.at<float>(2*y,2*x)+src2.at<float>(2*y,2*x+1)+src2.at<float>(2*y+1,2*x)+src2.at<float>(2*y+1,2*x+1))*0.5;

                dh_1=(src1.at<float>(2*y,2*x)+src1.at<float>(2*y+1,2*x)-src1.at<float>(2*y,2*x+1)-src1.at<float>(2*y+1,2*x+1))*0.5;
                dh_2=(src2.at<float>(2*y,2*x)+src2.at<float>(2*y+1,2*x)-src2.at<float>(2*y,2*x+1)-src2.at<float>(2*y+1,2*x+1))*0.5;

                dv_1=(src1.at<float>(2*y,2*x)+src1.at<float>(2*y,2*x+1)-src1.at<float>(2*y+1,2*x)-src1.at<float>(2*y+1,2*x+1))*0.5;
                dv_2=(src2.at<float>(2*y,2*x)+src2.at<float>(2*y,2*x+1)-src2.at<float>(2*y+1,2*x)-src2.at<float>(2*y+1,2*x+1))*0.5;

                dd_1=(src1.at<float>(2*y,2*x)-src1.at<float>(2*y,2*x+1)-src1.at<float>(2*y+1,2*x)+src1.at<float>(2*y+1,2*x+1))*0.5;
                dd_2=(src2.at<float>(2*y,2*x)-src2.at<float>(2*y,2*x+1)-src2.at<float>(2*y+1,2*x)+src2.at<float>(2*y+1,2*x+1))*0.5;

                if(src3.empty())
                {
                    c_out = c_1*0.5 + c_2*0.5;
                    dh_out = max({dh_1, dh_2});
                    dv_out = max({dv_1, dv_2});
                    dd_out = max({dd_1, dd_2});
                }


                else
                {
                    c_3=(src3.at<float>(2*y,2*x)+src3.at<float>(2*y,2*x+1)+src3.at<float>(2*y+1,2*x)+src3.at<float>(2*y+1,2*x+1))*0.5;
                    dh_3=(src3.at<float>(2*y,2*x)+src3.at<float>(2*y+1,2*x)-src3.at<float>(2*y,2*x+1)-src3.at<float>(2*y+1,2*x+1))*0.5;
                    dv_3=(src3.at<float>(2*y,2*x)+src3.at<float>(2*y,2*x+1)-src3.at<float>(2*y+1,2*x)-src3.at<float>(2*y+1,2*x+1))*0.5;
                    dd_3=(src3.at<float>(2*y,2*x)-src3.at<float>(2*y,2*x+1)-src3.at<float>(2*y+1,2*x)+src3.at<float>(2*y+1,2*x+1))*0.5;


                    c_out = c_1*0.5 + c_2*0.5 + c_3*0.5;
                    dh_out = max({dh_1, dh_2, dh_3});
                    dv_out = max({dv_1, dv_2, dv_3});
                    dd_out = max({dd_1, dd_2, dd_3});
                }


                dst.at<float>(y,x)=c_out;
                dst.at<float>(y,x+(width>>(k+1)))=dh_out;
                dst.at<float>(y+(height>>(k+1)),x)=dv_out;
                dst.at<float>(y+(height>>(k+1)),x+(width>>(k+1)))=dd_out;

            }
        }
        dst.copyTo(src1);
    }
}

void MainWindow::on_fusion_clicked()
{
    if (propimage2.isNull()){
        QMessageBox::warning(this, "warning", "Please select at least two input image to implement image fusion");
        return;
    }

    const int NIter = 1;

    std::string image_from_file = file.toStdString();
    cv::Mat input = cv::imread(image_from_file, IMREAD_COLOR);
    Mat GrayFrame = Mat(input.rows, input.cols, CV_8UC1);
    Mat Src1 = Mat(input.rows, input.cols, CV_32FC1);
    cvtColor(input, GrayFrame, CV_BGR2GRAY);
    GrayFrame.convertTo(Src1,CV_32FC1);
//    Mat Dst1 = Mat(input.rows, input.cols, CV_32FC1);

    std::string image_from_file2 = file2.toStdString();
    cv::Mat input_2 = cv::imread(image_from_file2, IMREAD_COLOR);
    Mat GrayFrame2 = Mat(input_2.rows, input_2.cols, CV_8UC1);
    Mat Src2 = Mat(input_2.rows, input_2.cols, CV_32FC1);
    cvtColor(input_2, input_2, CV_BGR2GRAY);
    input_2.convertTo(Src2,CV_32FC1);
//    Mat Dst2 = Mat(input_2.rows, input_2.cols, CV_32FC1);

    std::string image_from_file3 = file3.toStdString();
    cv::Mat input_3 = cv::imread(image_from_file3, IMREAD_COLOR);
    Mat GrayFrame3 = Mat(input_3.rows, input_3.cols, CV_8UC1);
    Mat Src3 = Mat(input_3.rows, input_3.cols, CV_32FC1);
    if(!input_3.empty())
    {
        cvtColor(input_3, input_3, CV_BGR2GRAY);
        input_3.convertTo(Src3,CV_32FC1);
    }

    Mat Dst=Mat(input.rows, input.cols, CV_32FC1);
    Mat Dst_out=Mat(input.rows, input.cols, CV_32FC1);

    cvHaarWavelet_2(Src1, Src2, Src3, Dst, NIter);

    cvInvHaarWavelet(Dst,Dst_out,NIter,GARROT,30);

// ---------Normalization to 0-1 range (for visualization)---------------------------

    double M=0,m=0;
    minMaxLoc(Dst_out,&m,&M);
    if((M-m)>0) {Dst_out=Dst_out*(1.0/(M-m))-m/(M-m);}
//    imshow("1", input);
//    imshow("2", input_2);
//    imshow("3", input_3);
//    imshow("Filtered", Dst_out);

    normalize(Dst_out, Dst_out,0,255,cv::NORM_MINMAX,CV_8UC1);

    QImage trans_img(Dst_out.cols, Dst_out.rows, QImage::Format_Indexed8);
    trans_img.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        trans_img.setColor(i, qRgb(i, i, i));
    }
    uchar *pSrc = Dst_out.data;
    for(int row = 0; row < Dst_out.rows; row++)
    {
        uchar *pDest = trans_img.scanLine(row);
        memcpy(pDest, pSrc, Dst_out.cols);
        pSrc += Dst_out.step;
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(trans_img);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

}


void MainWindow::on_hough_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    std::string image_from_file = file.toStdString();
    cv::Mat src = cv::imread(image_from_file, IMREAD_GRAYSCALE);

    // Declare the output variables
    Mat dst, cdst, cdstP;

//    const char* default_file = "sudoku.png";
//    const char* filename = argc >=2 ? argv[1] : default_file;
//    // Loads an image
//    Mat src = imread( samples::findFile( filename ), IMREAD_GRAYSCALE );

    // Check if image is loaded fine
    if(src.empty()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    // Edge detection
    Canny(src, dst, 50, 200, 3);
    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();

//    // Standard Hough Line Transform
//    vector<Vec2f> lines;                              // will hold the results of the detection
//    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection

//    // Draw the lines
//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//        float rho = lines[i][0], theta = lines[i][1];
//        Point pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000*(-b));
//        pt1.y = cvRound(y0 + 1000*(a));
//        pt2.x = cvRound(x0 - 1000*(-b));
//        pt2.y = cvRound(y0 - 1000*(a));
//        line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
//    }

    // Probabilistic Line Transform
    vector<Vec4i> linesP;                                // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI/180, 45, 50, 10); // runs the actual detection

    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

//    imshow("Source", src);
//    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
//    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    QImage trans_img= QImage((uchar*) cdstP.data, cdstP.cols, cdstP.rows, cdstP.step, QImage::Format_BGR888);
    QPixmap OuputImage_pixmap = QPixmap::fromImage(trans_img);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

}



