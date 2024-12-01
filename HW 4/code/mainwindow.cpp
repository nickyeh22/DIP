#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "QMessageBox"

#include <QTableWidget>
#include <QTableWidgetItem>

#include "qfiledialog.h"

#include "QDebug"
#include <QImage>
#include <QPixmap>
#include <QColor>

#include <complex>
#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "complex.h"

//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/mat.hpp>
#include <iostream>



//using namespace cv;
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


void MainWindow::on_pushButton_clicked()
{
    QString file= QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW4", tr("Images File (*.JPG)"));     //打開檔案並讀作字串
    QImage image(file);
    propimage = image.scaled(512, 512, Qt::IgnoreAspectRatio);

    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    outputimage = new QImage(512,512,QImage::Format_RGB32);
    *outputimage = image.scaled(512, 512, Qt::IgnoreAspectRatio);

    QPixmap InputImage = QPixmap::fromImage(propimage);
    ui->input->setPixmap(InputImage.scaled(ui->input->width(),ui->input->height(),Qt::KeepAspectRatio));

    SpectrumImage = new QImage(512,512,QImage::Format_RGB32);
    *SpectrumImage = image.scaled(512, 512, Qt::IgnoreAspectRatio);

    PhaseAngleImage = new QImage(512,512,QImage::Format_RGB32);
    *PhaseAngleImage = image.scaled(512, 512, Qt::IgnoreAspectRatio);

    input_array = (double**)malloc((512) * sizeof(double*));
    for (int i = 0; i < 512; i++){
        input_array[i] = (double*)malloc((512) * sizeof(double));}

    spectrum_array = (double**)malloc((512) * sizeof(double*));
    for (int i = 0; i < 512; i++){
        spectrum_array[i] = (double*)malloc((512) * sizeof(double));}

    phaseangle_array = (double**)malloc((512) * sizeof(double*));
    for (int i = 0; i < 512; i++){
        phaseangle_array[i] = (double*)malloc((512) * sizeof(double));}

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb inputimagergb = propimage.pixel(i,j);
            input_array[i][j] = qGray(inputimagergb);
        }
    }

    MainWindow::MainWindow::FastFourierTransform(input_array, spectrum_array, phaseangle_array, 512, 512);

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            spectrum_array[i][j] = 255*(log(1+abs(spectrum_array[i][j])))/(f_max - f_min);
            if(spectrum_array[i][j]>255)
                spectrum_array[i][j] = 255;
            SpectrumImage->setPixel(i, j, qRgb(spectrum_array[i][j],spectrum_array[i][j],spectrum_array[i][j]));
            PhaseAngleImage->setPixel(i, j, qRgb(phaseangle_array[i][j],phaseangle_array[i][j],phaseangle_array[i][j]));
        }
    }

    QPixmap spectrum_map = QPixmap::fromImage(*SpectrumImage);
    ui->spectrum->setPixmap(spectrum_map.scaled(ui->spectrum->width(),ui->spectrum->height(),Qt::KeepAspectRatio));

    QPixmap phaseangle_map = QPixmap::fromImage(*PhaseAngleImage);
    ui->phaseangle->setPixmap(phaseangle_map.scaled(ui->phaseangle->width(),ui->phaseangle->height(),Qt::KeepAspectRatio));

    for (int i = 0; i < 512; i++)
        free(input_array[i]);
    free(input_array);

    for (int i = 0; i < 512; i++)
        free(spectrum_array[i]);
    free(spectrum_array);

    for (int i = 0; i < 512; i++)
        free(phaseangle_array[i]);
    free(phaseangle_array);

    //-----------------------------------------------------opencv----------------------------------------------

    //    Mat img = imread(file.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);    //以灰度圖像讀入
    //    if( img.empty() )
    //    {
    //        printf("Cannot read image file");
    //        return;
    //    }

    //    int M = getOptimalDFTSize( img.rows );             // 獲得最佳DFT尺寸，爲2的次方
    //    int N = getOptimalDFTSize( img.cols );             //同上
    //    Mat padded;
    //    copyMakeBorder(img, padded, 0, M - img.rows, 0, N - img.cols, BORDER_CONSTANT, Scalar::all(0));   // opencv中的邊界擴展函數，提供多種方式擴展

    //    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};  // Mat 數組，第一個爲擴展後的圖像，一個爲空圖像，
    //    Mat complexImg;
    //    merge(planes, 2, complexImg);                                             // 合併成一個Mat

    //    dft(complexImg, complexImg);                                              // FFT變換， dft需要一個2通道的Mat

    //    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    //    split(complexImg, planes);                                       //分離通道， planes[0] 爲實數部分，planes[1]爲虛數部分
    //    magnitude(planes[0], planes[1], planes[0]);                      // 求模
    //    Mat mag = planes[0];
    //    mag += Scalar::all(1);
    //    log(mag, mag);                                                   // 模的對數

    //    // crop the spectrum, if it has an odd number of rows or columns
    //    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));             //保證偶數的邊長

    //    int cx = mag.cols/2;
    //    int cy = mag.rows/2;

    //    // rearrange the quadrants of Fourier image                      //對傅立葉變換的圖像進行重排，4個區塊，從左到右，從上到下 分別爲q0, q1, q2, q3
    //    // so that the origin is at the image center                     //  對調q0和q3, q1和q2
    //    Mat tmp;
    //    Mat q0(mag, Rect(0, 0, cx, cy));
    //    Mat q1(mag, Rect(cx, 0, cx, cy));
    //    Mat q2(mag, Rect(0, cy, cx, cy));
    //    Mat q3(mag, Rect(cx, cy, cx, cy));

    //    q0.copyTo(tmp);
    //    q3.copyTo(q0);
    //    tmp.copyTo(q3);

    //    q1.copyTo(tmp);
    //    q2.copyTo(q1);
    //    tmp.copyTo(q2);

    //    normalize(mag, mag, 0, 1, CV_MINMAX);                           // 規範化值到 0~1 顯示圖片的需要
    //    qDebug()<< mag.type(); //CV_32FC1


    //    //               QImage spectrum = QImage((unsigned char*)(mag.data), mag.cols, mag.rows, mag.cols*mag.channels(), QImage::Format_Indexed8);

    //    //               mag.convertTo(mag, CV_8UC1, 0.04); // Convert to 8 Bit greyscale
    //    QImage spectrum = QImage((unsigned char*)(mag.data), mag.cols, mag.rows, mag.step, QImage::Format_Grayscale8);
    //    spectrum = spectrum.rgbSwapped();

    //////    qDebug()<< img2.at<float>(50, 50);


    //    QPixmap Image_Picmap = QPixmap::fromImage(spectrum);
    //    ui->spectrum->setPixmap(Image_Picmap.scaled(ui->input->width(),ui->input->height(),Qt::KeepAspectRatio));


    //    //               QPixmap spectrum = QPixmap::fromImage(QImage((unsigned char*)(mag.data), mag.cols, mag.rows, QImage::Format_Indexed8));
    //    //               ui->spectrum->setPixmap(spectrum.scaled(ui->spectrum->width(),ui->spectrum->height(),Qt::KeepAspectRatio));

    //    //                cvtColor(mag, mag, CV_BGR2RGB);
    //    //                ui->spectrum->setPixmap(QPixmap::fromImage(QImage(mag.data, mag.cols, mag.rows, mag.step, QImage::Format_RGB888)));

    //    imshow("spectrum magnitude", mag);
    //    //               waitKey();
    //    //               return ;


}



//FFT
int rev_bits(unsigned int index, int size)
{
    int rev = 0;
    for (; size > 1; size >>= 1)
    {
        rev = (rev << 1) | (index & 1);
        index >>= 1;
    }
    return rev;
}

Complex* fft1(Complex *data, unsigned int size, int log2n, bool inverse)
{
    double angle, wtmp, wpr, wpi, wr, wi;
    int n = 1, n2;
    double pi2 = 3.14159 * 2.0;
    double scale = 1.0 / size;
    Complex tc;

    for (int k = 0; k < log2n; ++k)
    {
        n2 = n;
        n <<= 1;
        angle = (inverse) ? pi2 / n : -pi2 / n;
        wtmp = sin(0.5 * angle);
        wpr = -2.0 * wtmp * wtmp;
        wpi = sin(angle);
        wr = 1.0;
        wi = 0.0;

        for (int m = 0; m < n2; ++m) {
            for (unsigned int i = m; i < size; i += n) {
                int j = i + n2;
                tc.real = wr * data[j].real - wi * data[j].imag;
                tc.imag = wr * data[j].imag + wi * data[j].real;
                data[j] = data[i] - tc;
                data[i] += tc;
            }
            wr = (wtmp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtmp * wpi + wi;
        }
    }

    if (inverse) {
        for (int i = 0; i < n; i++) {
            data[i] *= scale;
        }
    }
    return data;
}

Complex** fft2(Complex **data, int r, int c, bool inverse)
{
    Complex *row = new Complex[r];
    Complex *column = new Complex[c];
    int log2w = log2(r);
    int log2h = log2(c);

    // Perform FFT on each row
    for (int y = 0; y < c; ++y) {
        for (int x = 0; x < r; ++x) {
            int rb = rev_bits(x, r);
            row[rb] = data[x][y];
        }
        row = fft1(row, r, log2w, inverse);
        for (int x = 0; x < r; ++x) {
            data[x][y] = row[x];
        }
    }


    // Perform FFT on each column
    for (int x = 0; x < r; ++x) {
        for (int y = 0; y < c; ++y) {
            int rb = rev_bits(y, c);
            column[rb] = data[x][y];
        }
        column = fft1(column, c, log2h, inverse);
        for (int y = 0; y < c; ++y) {
            data[x][y] = column[y];
        }
    }
    return data;
}

void MainWindow::FastFourierTransform(double ** InputImg, double ** spectrum, double ** phaseangle, int h, int w)
{
    int M = h;
    int N = w;

    //動態宣告二維度陣列

    FreqReal = new double * [w];      //列數
    for (int i = 0; i < w ; i++)
    {   FreqReal[i] = new double [h]; //行數
    }

    FreqImag = new double * [w];      //列數
    for (int i = 0; i < w ; i++)
    {   FreqImag[i] = new double [h]; //行數
    }


    double** pFreq = new double*[M];
    for (int newcnt = 0; newcnt < M; newcnt++)
    {
        pFreq[newcnt] = new double[N]; // 傅立葉頻率陣列
    }
    for (int forzero_i = 0; forzero_i < M; forzero_i++)
    {
        for (int forzero_j = 0; forzero_j < N; forzero_j++)
        {
            pFreq[forzero_i][forzero_j] = 0.0f;
        }
    }

    Complex **  input_seq = new Complex * [h];
    for (int j = 0; j < h; j++) {
        input_seq[j] = new Complex[h];
    }
    //陣列宣告結束

    //輸入待轉換的實部、虛部
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; ++j) {
            input_seq[i][j] = Complex(InputImg[i][j] * pow(-1, i + j), 0);
        }
    }

    //進行2D FFT，正轉換為false，反轉換為true
    fft2(input_seq, h, w, false);

    //將轉換結果丟進陣列儲存
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            // 將計算好的傅立葉實數與虛數部分作結合
            pFreq[i][j] = sqrt(pow(input_seq[i][j].real, 2) + pow(input_seq[i][j].imag, 2));
            // 結合後之頻率域丟入影像陣列中顯示
            FreqReal[i][j] = input_seq[i][j].real;
            FreqImag[i][j] = input_seq[i][j].imag;
            spectrum[i][j] = pFreq[i][j];
        }
    }

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            complex <double> c1 (input_seq[i][j].real, input_seq[i][j].imag);
            phaseangle[i][j] = arg(c1);
        }
    }

    f_max = 0 ;
    f_min = 10000;

    for(int j = 0; j < M ; j++)
    {
        for(int i = 0 ; i < N ; i++)
        {
            if(spectrum[i][j] > f_max){
                f_max = spectrum[i][j];
            }
            if(spectrum[i][j] < f_min){
                f_min = spectrum[i][j];
            }
        }
    }

    f_max = log( 1 + abs(f_max));
    f_min = log( 1 + abs(f_min));


    //-------------------------------------------
    for (int delcnt = 0; delcnt < M; delcnt++)
    {
        delete[] pFreq[delcnt];
    }

    delete[] pFreq;
}



void MainWindow::on_reversetrans_clicked()
{
    rev = true;

    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    Complex **  output_seq = new Complex * [512];
    for (int j = 0; j < 512; j++) {
        output_seq[j] = new Complex[512];
    }

    for (int j = 0; j < 512; j++) {
        for (int i = 0; i < 512; i++) {
            output_seq[i][j] = Complex(FreqReal[i][j], FreqImag[i][j]);
        }
    }

    fft2(output_seq, 512, 512, true);

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            output_seq[i][j].real *= pow(-1, i + j);

            if(output_seq[i][j].real > 255)
                output_seq[i][j].real = 255;
            if(output_seq[i][j].real < 0)
                output_seq[i][j].real = 0;

            outputimage->setPixel(i, j, qRgb(output_seq[i][j].real,output_seq[i][j].real,output_seq[i][j].real));
        }
    }

    QPixmap output_map = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(output_map.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    for (int i = 0; i < 512; i++)
    {
        delete[] output_seq[i];
    }
    delete[] output_seq;

}


void MainWindow::on_subtraction_clicked()
{
    if (rev != true){
        QMessageBox::warning(this, "warning", "Please do the reverse transform first");
        return;
    }

    QImage Subtraction(512,512,QImage::Format_RGB32);

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb input_rgb = propimage.pixel(i,j);
            int input_value = qGray(input_rgb);
            QRgb output_rgb = outputimage->pixel(i,j);
            int output_value = qGray(output_rgb);

            int delta = output_value - input_value;
            if(delta != 0)
                delta = 255;

            //            Subtraction.setPixel(i, j, qRgb((output_value - input_value),(output_value - input_value),(output_value - input_value)));
            Subtraction.setPixel(i, j, qRgb(delta, delta, delta));
        }
    }

    QPixmap output_map = QPixmap::fromImage(Subtraction);
    ui->output->setPixmap(output_map.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    double cutoff_freq = (double) value;
    ui->label_6->setText(QString::number(value));

    if (filter_index == 0)
    {
        QMessageBox::warning(this, "warning", "Please select an Filter");
        return;
    }

    if (filter_index == 1)
    {
        Complex **  output_seq = new Complex * [512];
        for (int j = 0; j < 512; j++) {
            output_seq[j] = new Complex[512];
        }

        Complex **  mask = new Complex * [512];
        for (int j = 0; j < 512; j++) {
            mask[j] = new Complex[512];
        }

        for(int j = 0; j < 512 ; j++)
        {
            for(int i = 0; i < 512 ; i++)
            {
                output_seq[i][j] = Complex(FreqReal[i][j], FreqImag[i][j]);

                if(H_L_pass == 0)
                {
                    if( (double) sqrt(pow(i-256,2)+pow(j-256,2)) <= cutoff_freq )
                        mask[i][j] = 1;
                    else{
                        mask[i][j] = 0;
                    }
                }

                if(H_L_pass == 1)
                {
                    if( (double) sqrt(pow(i-256,2)+pow(j-256,2)) <= cutoff_freq )
                        mask[i][j] = 0;
                    else{
                        mask[i][j] = 1;
                    }
                }

                output_seq[i][j] *= mask[i][j];
            }
        }

        MainWindow::MainWindow::show_processed_spec_pa(output_seq);

        fft2(output_seq, 512, 512, true);

        for(int j = 0; j < 512 ; j++)
        {
            for(int i = 0; i < 512 ; i++)
            {
                output_seq[i][j].real *= pow(-1, i + j);

                if(output_seq[i][j].real > 255)
                    output_seq[i][j].real = 255;
                if(output_seq[i][j].real < 0)
                    output_seq[i][j].real = 0;

                outputimage->setPixel(i, j, qRgb(output_seq[i][j].real,output_seq[i][j].real,output_seq[i][j].real));
            }
        }

        QPixmap output_map = QPixmap::fromImage(*outputimage);
        ui->output->setPixmap(output_map.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        for (int i = 0; i < 512; i++)
        {delete[] output_seq[i];}
        delete[] output_seq;

        for (int i = 0; i < 512; i++)
        {delete[] mask[i];}
        delete[] mask;
    }


    if (filter_index == 2)
    {
        Complex **  output_seq = new Complex * [512];
        for (int j = 0; j < 512; j++) {
            output_seq[j] = new Complex[512];
        }

        Complex **  mask = new Complex * [512];
        for (int j = 0; j < 512; j++) {
            mask[j] = new Complex[512];
        }

        for(int j = 0; j < 512 ; j++)
        {
            for(int i = 0; i < 512 ; i++)
            {
                output_seq[i][j] = Complex(FreqReal[i][j], FreqImag[i][j]);

                if(H_L_pass == 0)
                {
                    mask[i][j] = 1/(1+pow((sqrt(pow(i-256,2)+pow(j-256,2))/cutoff_freq), 2*n));
                }

                if(H_L_pass == 1)
                {
                    mask[i][j] = 1 - 1/(1+pow((sqrt(pow(i-256,2)+pow(j-256,2))/cutoff_freq), 2*n));
                }

                output_seq[i][j] *= mask[i][j];
            }
        }

        MainWindow::MainWindow::show_processed_spec_pa(output_seq);

        fft2(output_seq, 512, 512, true);

        for(int j = 0; j < 512 ; j++)
        {
            for(int i = 0; i < 512 ; i++)
            {
                output_seq[i][j].real *= pow(-1, i + j);

                if(output_seq[i][j].real > 255)
                    output_seq[i][j].real = 255;
                if(output_seq[i][j].real < 0)
                    output_seq[i][j].real = 0;

                outputimage->setPixel(i, j, qRgb(output_seq[i][j].real,output_seq[i][j].real,output_seq[i][j].real));
            }
        }

        QPixmap output_map = QPixmap::fromImage(*outputimage);
        ui->output->setPixmap(output_map.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        for (int i = 0; i < 512; i++)
        {delete[] output_seq[i];}
        delete[] output_seq;

        for (int i = 0; i < 512; i++)
        {delete[] mask[i];}
        delete[] mask;
    }


    if (filter_index == 3)
    {
        Complex **  output_seq = new Complex * [512];
        for (int j = 0; j < 512; j++) {
            output_seq[j] = new Complex[512];
        }

        Complex **  mask = new Complex * [512];
        for (int j = 0; j < 512; j++) {
            mask[j] = new Complex[512];
        }

        for(int j = 0; j < 512 ; j++)
        {
            for(int i = 0; i < 512 ; i++)
            {
                output_seq[i][j] = Complex(FreqReal[i][j], FreqImag[i][j]);

                if(H_L_pass == 0)
                {
                    mask[i][j] = exp(-(pow(i-256,2)+pow(j-256,2))/(2*pow(cutoff_freq,2)));
                }

                if(H_L_pass == 1)
                {
                    mask[i][j] = 1 - exp(-(pow(i-256,2)+pow(j-256,2))/(2*pow(cutoff_freq,2)));
                }

                output_seq[i][j] *= mask[i][j];
            }
        }

        MainWindow::MainWindow::show_processed_spec_pa(output_seq);

        fft2(output_seq, 512, 512, true);

        for(int j = 0; j < 512 ; j++)
        {
            for(int i = 0; i < 512 ; i++)
            {
                output_seq[i][j].real *= pow(-1, i + j);

                if(output_seq[i][j].real > 255)
                    output_seq[i][j].real = 255;
                if(output_seq[i][j].real < 0)
                    output_seq[i][j].real = 0;

                outputimage->setPixel(i, j, qRgb(output_seq[i][j].real,output_seq[i][j].real,output_seq[i][j].real));
            }
        }

        QPixmap output_map = QPixmap::fromImage(*outputimage);
        ui->output->setPixmap(output_map.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

        for (int i = 0; i < 512; i++)
        {delete[] output_seq[i];}
        delete[] output_seq;

        for (int i = 0; i < 512; i++)
        {delete[] mask[i];}
        delete[] mask;
    }
}


void MainWindow::show_processed_spec_pa(Complex ** output)
{
    double **spectrum_array = (double**)malloc((512) * sizeof(double*));
    for (int i = 0; i < 512; i++){
        spectrum_array[i] = (double*)malloc((512) * sizeof(double));}

    double **phaseangle_array = (double**)malloc((512) * sizeof(double*));
    for (int i = 0; i < 512; i++){
        phaseangle_array[i] = (double*)malloc((512) * sizeof(double));}

    for (int j = 0; j < 512; j++)
    {
        for (int i = 0; i < 512; i++)
        {
            spectrum_array[i][j] = sqrt(pow(output[i][j].real, 2) + pow(output[i][j].imag, 2));
            complex <double> c1 (output[i][j].real, output[i][j].imag);
            phaseangle_array[i][j] = arg(c1);
        }
    }

    f_max = 0 ;
    f_min = 10000;

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0 ; i < 512 ; i++)
        {
            if(spectrum_array[i][j] > f_max){
                f_max = spectrum_array[i][j];
            }
            if(spectrum_array[i][j] < f_min){
                f_min = spectrum_array[i][j];
            }
        }
    }

    f_max = log( 1 + abs(f_max));
    f_min = log( 1 + abs(f_min));

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            spectrum_array[i][j] = 255*(log(1+abs(spectrum_array[i][j])))/(f_max - f_min);
            if(spectrum_array[i][j]>255)
                spectrum_array[i][j] = 255;
            SpectrumImage->setPixel(i, j, qRgb(spectrum_array[i][j],spectrum_array[i][j],spectrum_array[i][j]));
            PhaseAngleImage->setPixel(i, j, qRgb(phaseangle_array[i][j],phaseangle_array[i][j],phaseangle_array[i][j]));
        }
    }

    QPixmap spectrum_map = QPixmap::fromImage(*SpectrumImage);
    ui->spectrum->setPixmap(spectrum_map.scaled(ui->spectrum->width(),ui->spectrum->height(),Qt::KeepAspectRatio));

    QPixmap phaseangle_map = QPixmap::fromImage(*PhaseAngleImage);
    ui->phaseangle->setPixmap(phaseangle_map.scaled(ui->phaseangle->width(),ui->phaseangle->height(),Qt::KeepAspectRatio));

    for (int i = 0; i < 512; i++)
        free(spectrum_array[i]);
    free(spectrum_array);

    for (int i = 0; i < 512; i++)
        free(phaseangle_array[i]);
    free(phaseangle_array);

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    H_L_pass = index;
}


void MainWindow::on_filters_currentIndexChanged(int index)
{
    filter_index = index;
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    n = arg1;
}




void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    if (propimage.isNull()){
        QMessageBox::warning(this, "warning", "Please select an input image");
        return;
    }

    double cutoff_freq = (double) value;
    ui->label_9->setText(QString::number(value));

    z_array = (double**)malloc((512) * sizeof(double*));
    for (int i = 0; i < 512; i++){
        z_array[i] = (double*)malloc((512) * sizeof(double));}

    Complex **  mask = new Complex * [512];
    for (int j = 0; j < 512; j++) {
        mask[j] = new Complex[512];
    }

//    int epsilon = 1;
    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            QRgb inputimagergb = propimage.pixel(i,j);
            z_array[i][j] = (double) qGray(inputimagergb);
//            z_array[i][j] = log(z_array[i][j] + epsilon);
        }
    }
//   qDebug() << z_array[100][100];

//   double** pFreq = new double * [512];
//   for (int newcnt = 0; newcnt < 512; newcnt++)
//   {
//       pFreq[newcnt] = new double[512]; // 傅立葉頻率陣列
//   }
//   for (int forzero_i = 0; forzero_i < 512; forzero_i++)
//   {
//       for (int forzero_j = 0; forzero_j < 512; forzero_j++)
//       {
//           pFreq[forzero_i][forzero_j] = 0.0f;
//       }
//   }

   Complex **  output_seq = new Complex * [512];
   for (int j = 0; j < 512; j++) {
       output_seq[j] = new Complex[512];
   }
    //輸入待轉換的實部、虛部
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; ++j) {
            output_seq[i][j] = Complex(z_array[i][j] * pow(-1, i + j), 0);
        }
    }

//    qDebug() << output_seq[100][100].real;
//    qDebug() << output_seq[100][200].real;
//    qDebug() << output_seq[300][100].real;


    //進行2D FFT，正轉換為false，反轉換為true
    fft2(output_seq, 512, 512, false);

//    qDebug() << (double)output_seq[100][100].real;


//    for (int i = 0; i < 512; i++)
//    {
//        for (int j = 0; j < 512; j++)
//        {
//            // 將計算好的傅立葉實數與虛數部分作結合
//            pFreq[i][j] = sqrt(pow(output_seq[i][j].real, 2) + pow(output_seq[i][j].imag, 2));
//            // 結合後之頻率域丟入影像陣列中顯示
//            fuckYou[i][j] = pFreq[i][j];
//        }
//    }

//    qDebug() << fuckYou[100][100];

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
            //            output_seq[i][j] = Complex(input_seq[i][j].real, input_seq[i][j].imag);
            mask[i][j] = (gamma_H - gamma_L)*(1 - exp(-5*(pow(i-256,2)+pow(j-256,2))/pow(cutoff_freq,2))) + gamma_L;
            output_seq[i][j] *= mask[i][j];
        }
    }

    MainWindow::MainWindow::show_processed_spec_pa(output_seq);

    fft2(output_seq, 512, 512, true);

    for(int j = 0; j < 512 ; j++)
    {
        for(int i = 0; i < 512 ; i++)
        {
//            output_seq[i][j].real = exp(output_seq[i][j].real)/* -1*/ ;
            output_seq[i][j].real = output_seq[i][j].real/* -1*/ ;
            output_seq[i][j].real *= pow(-1, i + j);

            if(output_seq[i][j].real > 255)
                output_seq[i][j].real = 255;
            if(output_seq[i][j].real < 0)
                output_seq[i][j].real = 0;

            outputimage->setPixel(i, j, qRgb(output_seq[i][j].real,output_seq[i][j].real,output_seq[i][j].real));
        }
    }

        qDebug() << output_seq[70][30].real;

    QPixmap output_map = QPixmap::fromImage(*outputimage);
    ui->output->setPixmap(output_map.scaled(ui->output->width(),ui->output->height(),Qt::KeepAspectRatio));

    for (int i = 0; i < 512; i++)
    {delete[] output_seq[i];}
    delete[] output_seq;

    for (int i = 0; i < 512; i++)
    {delete[] mask[i];}
    delete[] mask;

    for (int i = 0; i < 512; i++)
        free(z_array[i]);
    free(z_array);

    //    for (int i = 0; i < 512; i++)
    //    {delete[] input_seq[i];}
    //    delete[] input_seq;


}



void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    gamma_H = arg1;
}


void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    gamma_L = arg1;
}

