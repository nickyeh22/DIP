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

#include <QTableWidget>
#include <QTableWidgetItem>

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


void MainWindow::on_selectinput_clicked()
{
    QString file= QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW3", tr("Images File (*.JPG)"));     //打開檔案並讀作字串
    QImage image(file);
    propimage = image.scaled(512, 512, Qt::IgnoreAspectRatio);

    outputimage = new QImage(512,512,QImage::Format_RGB32);
    *outputimage = image.scaled(512, 512, Qt::IgnoreAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage);
    ui->input->setPixmap(InputImage.scaled(ui->input->width(),ui->input->height(),Qt::KeepAspectRatio));

//    qDebug()<<maskindex;

}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->tableWidget->setColumnCount(index*2+1);
    ui->tableWidget->setRowCount(index*2+1);
    maskindex = index;

    //    mask = new double * [maskindex*2+1];            //此矩陣有 n 個列(rows); 先 new 出 n 個 int *
    //    for (int i=0; i<maskindex*2+1; i++)
    //    {
    //        mask[i] = new double [maskindex*2+1];      //每一列有 m 行(columns); array[i] 指向新 new 出的 m 個 int
    //    }

    mask = (double**)malloc((maskindex*2+1) * sizeof(double*));
    for (int i = 0; i < maskindex*2+1; i++){
        mask[i] = (double*)malloc((maskindex*2+1) * sizeof(double));}

    for(int j = 0; j < maskindex*2+1; j++)
    {
        for(int i = 0; i < maskindex*2+1 ; i++)
        {
            mask[i][j]=0;
        }
    }

}


void MainWindow::on_convolution_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    if (maskindex == 0){
        QMessageBox::warning(this, "warning", "Please select the mask-size");
        return;
    }

    QImage paddedimage(512+maskindex*2,512+maskindex*2,QImage::Format_RGB32);
    //    int diagonal = maskindex;

    //    //左上補
    //    for(int j = 0; j < maskindex; j++)
    //    {
    //      for(int i = 0; i < maskindex; i++)
    //      {
    //          QRgb paddedimagergb = propimage.pixel(i,j);

    //          if(i!=j){
    //              paddedimage.setPixel(j, i, paddedimagergb);
    //          }
    //          else{
    //              paddedimage.setPixel(diagonal, diagonal, paddedimagergb);
    //              diagonal = maskindex-1;
    //          }
    //      }
    //    }

    //上
    for(int j = 0; j < maskindex; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(maskindex+i, maskindex-1-j, paddedimagergb);
        }
    }

    //下
    for(int j = 512-maskindex; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(maskindex+i, 512+2*maskindex-1-i, paddedimagergb);
        }
    }

    //左
    for(int j = 0; j < 512+2*maskindex; j++)
    {
        int k=1;

        for(int i = maskindex; i <= maskindex*2-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(maskindex - k, j, paddedimagergb);
            k++;
        }
    }

    //右

    for(int j = 0; j < 512+2*maskindex; j++)
    {
        int m=0;
        for(int i = 1079; i <= 512+maskindex-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(512+maskindex*2-1-m, j, paddedimagergb);
            m++;
        }
    }

    //中
    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(i+maskindex, j+maskindex, paddedimagergb);
        }
    }

    //        QPixmap OuputImage_pixmap = QPixmap::fromImage(paddedimage);
    //        ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));



    //    array_r = new int * [512+maskindex*2];            //此矩陣有 n 個列(rows); 先 new 出 n 個 int *
    //    for (int i=0; i<512+maskindex*2; i++)
    //    {
    //        array_r[i] = new int [512+maskindex*2];      //每一列有 m 行(columns); array[i] 指向新 new 出的 m 個 int
    //    }

    //    array_g = new int * [512+maskindex*2];            //此矩陣有 n 個列(rows); 先 new 出 n 個 int *
    //    for (int i=0; i<512+maskindex*2; i++)
    //    {
    //        array_r[i] = new int [512+maskindex*2];      //每一列有 m 行(columns); array[i] 指向新 new 出的 m 個 int
    //    }

    //    array_b = new int * [512+maskindex*2];            //此矩陣有 n 個列(rows); 先 new 出 n 個 int *
    //    for (int i=0; i<512+maskindex*2; i++)
    //    {
    //        array_r[i] = new int [512+maskindex*2];      //每一列有 m 行(columns); array[i] 指向新 new 出的 m 個 int
    //    }

    //    int (*array_r)[512+maskindex*2] = (int(*)[512+maskindex*2])malloc((512+maskindex*2)*(512+maskindex*2)*sizeof(int));
    //    int (*array_g)[512+maskindex*2] = (int(*)[512+maskindex*2])malloc((512+maskindex*2)*(512+maskindex*2)*sizeof(int));
    //    int (*array_b)[512+maskindex*2] = (int(*)[512+maskindex*2])malloc((512+maskindex*2)*(512+maskindex*2)*sizeof(int));

    //    qDebug() << mask[1][1];

//    int **array_r = (int**)malloc((512+maskindex*2) * sizeof(int*));
//    for (int i = 0; i < 512+maskindex*2; i++){
//        array_r[i] = (int*)malloc((512+maskindex*2) * sizeof(int));}

    int **array_g = (int**)malloc((512+maskindex*2) * sizeof(int*));
    for (int i = 0; i < 512+maskindex*2; i++){
        array_g[i] = (int*)malloc((512+maskindex*2) * sizeof(int));}

//    int **array_b = (int**)malloc((512+maskindex*2) * sizeof(int*));
//    for (int i = 0; i < 512+maskindex*2; i++){
//        array_b[i] = (int*)malloc((512+maskindex*2) * sizeof(int));}

    //qDebug()<<"hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
    //qDebug()<<512+maskindex*2;

    for(int j = 0; j < 512+maskindex*2; j++)
    {
        for(int i = 0; i < 512+maskindex*2 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
//            array_r[i][j] = qRed(paddedimagergb);
            array_g[i][j] = qGray(paddedimagergb);
//            array_b[i][j] = qBlue(paddedimagergb);

        }
    }
    //    qDebug()<< r;
    //    qDebug()<< array_r[1081][1081];

    //    QImage OutputImage(512+maskindex*2,512+maskindex*2,QImage::Format_RGB32);

    //    for(int j = 0; j < 512+maskindex*2; j++)
    //    {
    //      for(int i = 0; i < 512+maskindex*2 ; i++)
    //        {
    //          OutputImage.setPixel(i, j, qRgb(array_r[i][j],array_g[i][j],array_b[i][j]));
    //      }
    //    }

    //    qDebug()<< array_r[1081][1081];


    int g = 0;
    double weight = 0;

    for(int j = 0; j < maskindex*2+1 ; j++)
    {
        for(int i = 0; i < maskindex*2+1 ; i++)
        {
            weight += mask[i][j];
        }
    }

    outputimage = new QImage(512,512,QImage::Format_RGB32);

    for(int j = maskindex; j < 512+maskindex; j++)
    {
        for(int i = maskindex; i < 512+maskindex ; i++)
        {
            for(int k = 0 ; k < maskindex*2+1 ; k++)
            {
                for(int l = 0 ; l < maskindex*2+1 ; l++)
                {
                    g += round(mask[l][k]*array_g[i-maskindex+l][j-maskindex+k]/weight);
                }
            }

            if (g > 255)
                g = 255;
            if(g < 0)
                g = 0;

            outputimage->setPixel(i-maskindex, j-maskindex, qRgb(g,g,g));

            g = 0;
        }
    }

//    qDebug()<< "hiiiiiiiiiiiiiiiiiiii";


    QPixmap OuputImage_pixmap = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));


//    for (int i = 0; i < 512+maskindex*2; i++)
//        free(array_r[i]);
//    free(array_r);

    for (int i = 0; i < 512+maskindex*2; i++)
        free(array_g[i]);
    free(array_g);

//    for (int i = 0; i < 512+maskindex*2; i++)
//        free(array_b[i]);
//    free(array_b);

    //    for (int i = 0; i < maskindex*2+1; i++)
    //      free(mask[i]);
    //    free(mask);


}



void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    double cellvalue = item->text().toDouble();
    //    qDebug()<< cellvalue;
    int i = item->column();
    int j = item->row();
    //    qDebug()<< i;
    mask[i][j]=cellvalue;
    //    qDebug()<< mask[1][2];
}



void MainWindow::on_gaussian_K_valueChanged(double arg1)
{
    gaussian_K=arg1;

    for(int j = 0; j < maskindex*2+1; j++)
    {
        for(int i = 0; i < maskindex*2+1 ; i++)
        {
            double K = gaussian_K*exp(-(pow((i-maskindex),2)+pow((j-maskindex),2))/2/pow(gaussian_sigma,2));

            QTableWidgetItem *item;
            item = new QTableWidgetItem;
            item->setText(QString::number(K,'d',4));

            ui->tableWidget->setItem(i,j,item);

        }
    }

}


void MainWindow::on_gaussian_sigma_valueChanged(double arg2)
{
    gaussian_sigma=arg2;

    for(int j = 0; j < maskindex*2+1; j++)
    {
        for(int i = 0; i < maskindex*2+1 ; i++)
        {
            double K = gaussian_K*exp(-(pow((i-maskindex),2)+pow((j-maskindex),2))/2/pow(gaussian_sigma,2));

            QTableWidgetItem *item;
            item = new QTableWidgetItem;
            item->setText(QString::number(K,'d',4));

            ui->tableWidget->setItem(i,j,item);

        }
    }
}




void MainWindow::on_Laplacian_clicked()
{    
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    QImage paddedimage(512+2,512+2,QImage::Format_RGB32);

    //上
    for(int j = 0; j < 1; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = outputimage->pixel(i,j);
            paddedimage.setPixel(1+i, 1-1-j, paddedimagergb);
        }
    }

    //下
    for(int j = 512-1; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = outputimage->pixel(i,j);
            paddedimage.setPixel(1+i, 512+2*1-1-i, paddedimagergb);
        }
    }

    //左
    for(int j = 0; j < 512+2*1; j++)
    {
        int k=1;

        for(int i = 1; i <= 1*2-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(1 - k, j, paddedimagergb);
            k++;
        }
    }

    //右

    for(int j = 0; j < 512+2*1; j++)
    {
        int m=0;
        for(int i = 1079; i <= 512+1-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(512+1*2-1-m, j, paddedimagergb);
            m++;
        }
    }

    //中
    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb paddedimagergb = outputimage->pixel(i,j);
            paddedimage.setPixel(i+1, j+1, paddedimagergb);
        }
    }

    int mask_laplacian[3][3]=
    {
        {1, 1, 1},
        {1, -8, 1},
        {1, 1, 1}
    };


//    qDebug() <<mask_laplacian[1][0]<<mask_laplacian[1][1];

//    int **array_r = (int**)malloc((512+1*2) * sizeof(int*));
//    for (int i = 0; i < 512+1*2; i++){
//        array_r[i] = (int*)malloc((512+1*2) * sizeof(int));}

    int **array_g = (int**)malloc((512+1*2) * sizeof(int*));
    for (int i = 0; i < 512+1*2; i++){
        array_g[i] = (int*)malloc((512+1*2) * sizeof(int));}

//    int **array_b = (int**)malloc((512+1*2) * sizeof(int*));
//    for (int i = 0; i < 512+1*2; i++){
//        array_b[i] = (int*)malloc((512+1*2) * sizeof(int));}

    array_zero = (int**)malloc((512) * sizeof(int*));
    for (int i = 0; i < 512; i++){
        array_zero[i] = (int*)malloc((512) * sizeof(int));}

    laplacian = 1;

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
//            array_r[i][j] = qRed(paddedimagergb);
            array_g[i][j] = qGray(paddedimagergb);
//            array_b[i][j] = qBlue(paddedimagergb);
        }
    }

//    int r = 0;
    int g = 0;
//    int b = 0;

    for(int j = 1; j < 512+1; j++)
    {
        for(int i = 1; i < 512+1 ; i++)
        {
            for(int k = 0 ; k < 3 ; k++)
            {
                for(int l = 0 ; l < 3 ; l++)
                {
//                    r += (mask_laplacian[l][k]* array_r[i-1+l][j-1+k]);
                    g += (mask_laplacian[l][k]* array_g[i-1+l][j-1+k]);
//                    b += (mask_laplacian[l][k]* array_b[i-1+l][j-1+k]);
                }
            }



//            if (r > 255)
//                r = 255;
//            else if (r < 0)
//                r = 0;

            if (g > 255)
                g = 255;
            else if (g < 0)
                g = 0;

            array_zero[i-1][j-1] = g;

//            if (b > 255)
//                b = 255;
//            else if (b < 0)
//                b = 0;

            outputimage->setPixel(i-1, j-1, qRgb(g,g,g));

//            r = 0;
            g = 0;ㄉ
//            b = 0;
        }
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));


//    for (int i = 0; i < 512+1*2; i++)
//        free(array_r[i]);
//    free(array_r);

    for (int i = 0; i < 512+1*2; i++)
        free(array_g[i]);
    free(array_g);

//    for (int i = 0; i < 512+1*2; i++)
//        free(array_b[i]);
//    free(array_b);


}


void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    regionindex = index;

}


void MainWindow::on_localenhancement_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    if (regionindex == 0){
        QMessageBox::warning(this, "warning", "Please select the region-size");
        return;
    }

    int g;
    double m=0;
    double pdf[256]={0};
    double sigma2=0;
    double sigma;
    //    double m_local[512][512];
    //    double sigma2_local[512][512];
    //    double sigma_local[512][512];
    double **m_local = (double**)malloc(512 * sizeof(double*));
    for (int i = 0; i < 512; i++){
        m_local[i] = (double*)malloc(512 * sizeof(double));}

    double **sigma2_local = (double**)malloc(512 * sizeof(double*));
    for (int i = 0; i < 512; i++){
        sigma2_local[i] = (double*)malloc(512 * sizeof(double));}

    double **sigma_local = (double**)malloc(512 * sizeof(double*));
    for (int i = 0; i < 512; i++){
        sigma_local[i] = (double*)malloc(512 * sizeof(double));}

    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb pixcolors = propimage.pixel(i,j);

            g = qGray(pixcolors);

            pdf[g]++;
        }
    }

    for(int i = 0; i < 256; i++)
    {
        pdf[i]/=1166400;
    }

    for(int i = 0; i < 256; i++)
    {
        m += i*pdf[i];
    }

    for(int i = 0; i < 256; i++)
    {
        sigma2 += pow((i-m),2)*pdf[i];
    }
    sigma = sqrt(sigma2);

    //    qDebug() << m << "," << sigma ;

    QImage paddedimage(512+regionindex*2,512+regionindex*2,QImage::Format_RGB32);

    //上
    for(int j = 0; j < regionindex; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(regionindex+i, regionindex-1-j, paddedimagergb);
        }
    }

    //下
    for(int j = 512-regionindex; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(regionindex+i, 512+2*regionindex-1-i, paddedimagergb);
        }
    }

    //左
    for(int j = 0; j < 512+2*regionindex; j++)
    {
        int k=1;

        for(int i = regionindex; i <= regionindex*2-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(regionindex - k, j, paddedimagergb);
            k++;
        }
    }

    //右

    for(int j = 0; j < 512+2*regionindex; j++)
    {
        int m=0;
        for(int i = 1079; i <= 512+regionindex-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(512+regionindex*2-1-m, j, paddedimagergb);
            m++;
        }
    }

    //中
    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(i+regionindex, j+regionindex, paddedimagergb);
        }
    }

    //    int **array_m = (int**)malloc((512+regionindex*2) * sizeof(int*));
    //    for (int i = 0; i < 512+regionindex*2; i++){
    //      array_m[i] = (int*)malloc((512+regionindex*2) * sizeof(int));}

    for(int i = 0; i < 256; i++)
    {
        pdf[i]=0;
    }

    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            m_local[i][j]=0;
            sigma2_local[i][j]=0;
            sigma_local[i][j]=0;
        }
    }

    //    qDebug() <<pdf[3] ;

    for(int j = regionindex; j < 512+regionindex; j++)
    {
        for(int i = regionindex; i < 512+regionindex ; i++)
        {
            for(int k = 0 ; k < regionindex*2+1 ; k++)
            {
                for(int l = 0 ; l < regionindex*2+1 ; l++)
                {
                    QRgb paddedimagergb = paddedimage.pixel(i-regionindex+l,j-regionindex+l);
                    g = qGray(paddedimagergb);
                    pdf[g]++;
                }
            }

            for(int i = 0; i < 256; i++)
            {
                pdf[i]/=pow((regionindex*2+1),2);
            }

            for(int n = 0; n < 256; n++)
            {
                m_local[i-regionindex][j-regionindex] += (n*pdf[n]);
            }

            for(int n = 0; n < 256; n++)
            {
                sigma2_local[i-regionindex][j-regionindex] += (pow((n-(m_local[i-regionindex][j-regionindex])),2)*pdf[n]);
            }
            sigma_local[i-regionindex][j-regionindex] = sqrt((sigma2_local[i-regionindex][j-regionindex]));

            for(int i = 0; i < 256; i++)
            {
                pdf[i]=0;
            }

        }
    }

    //    qDebug() << m_local[1000][1000]<< sigma_local[1000][1000] ;

    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb pixcolors = propimage.pixel(i,j);
            g = qGray(pixcolors);

            if ((m_local[i][j]>=0) && (m_local[i][j]<=(0.2799*m)) && (sigma_local[i][j]>=0) && (sigma_local[i][j]<=(0.2799*sigma)))
            {
                g *= parameter_c;

                if(g>255){
                    g=255;
                }

                else if (g<0){
                    g=0;
                }


                outputimage->setPixel(i, j, qRgb(g,g,g));

            }
        }
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    for (int i = 0; i < 512 ; i++)
        free(m_local[i]);
    free(m_local);

    for (int i = 0; i < 512; i++)
        free(sigma2_local[i]);
    free(sigma2_local);

    for (int i = 0; i < 512; i++)
        free(sigma_local[i]);
    free(sigma_local);


}


void MainWindow::on_parameter_c_valueChanged(int arg1)
{
    parameter_c = arg1;
}


void MainWindow::on_sobeloperators_clicked()
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    QImage paddedimage(512+2,512+2,QImage::Format_RGB32);

    //上
    for(int j = 0; j < 1; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(1+i, 1-1-j, paddedimagergb);
        }
    }

    //下
    for(int j = 512-1; j < 512; j++)
    {
        for(int i = 0; i < 512; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(1+i, 512+2*1-1-i, paddedimagergb);
        }
    }

    //左
    for(int j = 0; j < 512+2*1; j++)
    {
        int k=1;

        for(int i = 1; i <= 1*2-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(1 - k, j, paddedimagergb);
            k++;
        }
    }

    //右

    for(int j = 0; j < 512+2*1; j++)
    {
        int m=0;
        for(int i = 1079; i <= 512+1-1 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);
            paddedimage.setPixel(512+1*2-1-m, j, paddedimagergb);
            m++;
        }
    }

    //中
    for(int j = 0; j < 512; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb paddedimagergb = propimage.pixel(i,j);
            paddedimage.setPixel(i+1, j+1, paddedimagergb);
        }
    }

    int mask_sobel_horizontal[3][3]=
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    int mask_sobel_vertical[3][3]=
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };


//    qDebug() <<mask_laplacian[1][0]<<mask_laplacian[1][1];

    int **array_g = (int**)malloc((512+1*2) * sizeof(int*));
    for (int i = 0; i < 512+1*2; i++){
        array_g[i] = (int*)malloc((512+1*2) * sizeof(int));}


    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb paddedimagergb = paddedimage.pixel(i,j);

            array_g[i][j] = qGray(paddedimagergb);
        }
    }

    int g_x = 0;
    int g_y = 0;


    for(int j = 1; j < 512+1; j++)
    {
        for(int i = 1; i < 512+1 ; i++)
        {
            for(int k = 0 ; k < 3 ; k++)
            {
                for(int l = 0 ; l < 3 ; l++)
                {
                    g_x += (mask_sobel_horizontal[l][k]* array_g[i-1+l][j-1+k]);
                    g_y += (mask_sobel_vertical[l][k]* array_g[i-1+l][j-1+k]);
                }
            }

            int g_t = sqrt(pow(g_x,2)+pow(g_y,2));

            if (g_t > 255)
                g_t = 255;
            else if (g_t < 0)
                g_t = 0;

            outputimage->setPixel(i-1, j-1, qRgb(g_t,g_t,g_t));

            g_x = 0;
            g_y = 0;
        }
    }

    QPixmap OuputImage_pixmap = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));


    for (int i = 0; i < 512+1*2; i++)
        free(array_g[i]);
    free(array_g);


}


void MainWindow::on_zerocrossing_clicked()
{
    if (laplacian == 0){
        QMessageBox::warning(this, "warning", "Please do the Laplacian of Gaussian (LoG) operators first");
        return;
    }

    for(int j = 1; j < 512; j++)
    {
        for(int i = 1; i < 512 ; i++)
        {
            if ((array_zero[i][j] != 0))
                array_zero[i][j] = 255;

            outputimage->setPixel(i, j, qRgb(array_zero[i][j],array_zero[i][j],array_zero[i][j]));
        }
    }


    QPixmap OuputImage_pixmap = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(OuputImage_pixmap.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    for (int i = 0; i < 512; i++)
        free(array_zero[i]);
    free(array_zero);

    laplacian = 0;

}

