#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef POSSIBLE_PLATE_H
#define POSSIBLE_PLATE_H

#ifndef POSSIBLE_CHAR_H
#define POSSIBLE_CHAR_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

#include <string>
#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PossiblePlate {
public:
    // member variables ///////////////////////////////////////////////////////////////////////////
    cv::Mat imgPlate;
    cv::Mat imgGrayscale;
    cv::Mat imgThresh;

    cv::RotatedRect rrLocationOfPlateInScene;

    std::string strChars;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    static bool sortDescendingByNumberOfChars(const PossiblePlate &ppLeft, const PossiblePlate &ppRight) {
        return(ppLeft.strChars.length() > ppRight.strChars.length());
    }

};
#endif		// end #ifndef POSSIBLE_PLATE_H


class PossibleChar {
public:
    // member variables ///////////////////////////////////////////////////////////////////////////
    std::vector<cv::Point> contour;

    cv::Rect boundingRect;

    int intCenterX;
    int intCenterY;

    double dblDiagonalSize;
    double dblAspectRatio;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    static bool sortCharsLeftToRight(const PossibleChar &pcLeft, const PossibleChar & pcRight) {
        return(pcLeft.intCenterX < pcRight.intCenterX);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const PossibleChar& otherPossibleChar) const {
        if (this->contour == otherPossibleChar.contour) return true;
        else return false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const PossibleChar& otherPossibleChar) const {
        if (this->contour != otherPossibleChar.contour) return true;
        else return false;
    }

    // function prototypes ////////////////////////////////////////////////////////////////////////
    PossibleChar(std::vector<cv::Point> _contour);

};

#endif  // POSSIBLE_CHAR_H

//#define SHOW_STEPS            // un-comment or comment this line to show steps or not

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // global variables (Preprocess)///////////////////////////////////////////////////////////////////////////////
    const cv::Size GAUSSIAN_SMOOTH_FILTER_SIZE = cv::Size(5, 5);
    const int ADAPTIVE_THRESH_BLOCK_SIZE = 19;
    const int ADAPTIVE_THRESH_WEIGHT = 9;

    // global constants (Main)///////////////////////////////////////////////////////////////////////////////
    const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
    const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
    const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
    const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 255.0, 0.0);
    const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

    // global constants (DetectChars)///////////////////////////////////////////////////////////////////////////////
    // constants for checkIfPossibleChar, this checks one possible char only (does not compare to another char)
    const int MIN_PIXEL_WIDTH = 2;
    const int MIN_PIXEL_HEIGHT = 8;

    const double MIN_ASPECT_RATIO = 0.25;
    const double MAX_ASPECT_RATIO = 1.0;

    const int MIN_PIXEL_AREA = 80;

    // constants for comparing two chars
    const double MIN_DIAG_SIZE_MULTIPLE_AWAY = 0.3;
    const double MAX_DIAG_SIZE_MULTIPLE_AWAY = 5.0;

    const double MAX_CHANGE_IN_AREA = 0.5;

    const double MAX_CHANGE_IN_WIDTH = 0.8;
    const double MAX_CHANGE_IN_HEIGHT = 0.2;

    const double MAX_ANGLE_BETWEEN_CHARS = 12.0;

    // other constants
    const int MIN_NUMBER_OF_MATCHING_CHARS = 3;

    const int RESIZED_CHAR_IMAGE_WIDTH = 20;
    const int RESIZED_CHAR_IMAGE_HEIGHT = 30;

    const int MIN_CONTOUR_AREA = 100;

    // external global variables //////////////////////////////////////////////////////////////////////
    /*extern */ const bool blnShowSteps = 0;
//    /*extern */ cv::Ptr<cv::ml::KNearest>  kNearest;

    // global constants (DetectPlate)///////////////////////////////////////////////////////////////////////////////
    const double PLATE_WIDTH_PADDING_FACTOR = 1.3;
    const double PLATE_HEIGHT_PADDING_FACTOR = 1.5;


private slots:
    void on_pushButton_clicked();

    // function prototypes (Preprocess)////////////////////////////////////////////////////////////////////////////

    void preprocess(cv::Mat &imgOriginal, cv::Mat &imgGrayscale, cv::Mat &imgThresh);

    cv::Mat extractValue(cv::Mat &imgOriginal);

    cv::Mat maximizeContrast(cv::Mat &imgGrayscale);

    // function prototypes (Main)////////////////////////////////////////////////////////////////////////////
//    int main(void);

    void drawRedRectangleAroundPlate(cv::Mat &imgOriginalScene, PossiblePlate &licPlate);

    void writeLicensePlateCharsOnImage(cv::Mat &imgOriginalScene, PossiblePlate &licPlate);

    // function prototypes (DetectChars)////////////////////////////////////////////////////////////////////////////

    bool loadKNNDataAndTrainKNN(void);

    std::vector<PossiblePlate> detectCharsInPlates(std::vector<PossiblePlate> &vectorOfPossiblePlates);

    std::vector<PossibleChar> findPossibleCharsInPlate(cv::Mat &imgGrayscale, cv::Mat &imgThresh);

    bool checkIfPossibleChar(PossibleChar &possibleChar);

    std::vector<std::vector<PossibleChar> > findVectorOfVectorsOfMatchingChars(const std::vector<PossibleChar> &vectorOfPossibleChars);

    std::vector<PossibleChar> findVectorOfMatchingChars(const PossibleChar &possibleChar, const std::vector<PossibleChar> &vectorOfChars);

    double distanceBetweenChars(const PossibleChar &firstChar, const PossibleChar &secondChar);

    double angleBetweenChars(const PossibleChar &firstChar, const PossibleChar &secondChar);

    std::vector<PossibleChar> removeInnerOverlappingChars(std::vector<PossibleChar> &vectorOfMatchingChars);

    std::string recognizeCharsInPlate(cv::Mat &imgThresh, std::vector<PossibleChar> &vectorOfMatchingChars);

    // function prototypes (DetectPlate)////////////////////////////////////////////////////////////////////////////
    std::vector<PossiblePlate> detectPlatesInScene(cv::Mat &imgOriginalScene);

    std::vector<PossibleChar> findPossibleCharsInScene(cv::Mat &imgThresh);

    PossiblePlate extractPlate(cv::Mat &imgOriginal, std::vector<PossibleChar> &vectorOfMatchingChars);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H








