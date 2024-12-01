#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QFileDialog>
#include "QMessageBox"
#include <algorithm>
#include "QDebug"
//#include "qfiledialog.h"


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


//---------------------------Preprocess----------------------------//
void MainWindow::preprocess(cv::Mat &imgOriginal, cv::Mat &imgGrayscale, cv::Mat &imgThresh) {
    imgGrayscale = extractValue(imgOriginal);                           // extract value channel only from original image to get imgGrayscale

    cv::Mat imgMaxContrastGrayscale = maximizeContrast(imgGrayscale);       // maximize contrast with top hat and black hat

    cv::Mat imgBlurred;

    cv::GaussianBlur(imgMaxContrastGrayscale, imgBlurred, GAUSSIAN_SMOOTH_FILTER_SIZE, 0);          // gaussian blur

                                                                                                    // call adaptive threshold to get imgThresh
    cv::adaptiveThreshold(imgBlurred, imgThresh, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE, ADAPTIVE_THRESH_WEIGHT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat MainWindow::extractValue(cv::Mat &imgOriginal) {
    cv::Mat imgHSV;
    std::vector<cv::Mat> vectorOfHSVImages;
    cv::Mat imgValue;

    cv::cvtColor(imgOriginal, imgHSV, CV_BGR2HSV);

    cv::split(imgHSV, vectorOfHSVImages);

    imgValue = vectorOfHSVImages[2];

    return(imgValue);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat MainWindow::maximizeContrast(cv::Mat &imgGrayscale) {
    cv::Mat imgTopHat;
    cv::Mat imgBlackHat;
    cv::Mat imgGrayscalePlusTopHat;
    cv::Mat imgGrayscalePlusTopHatMinusBlackHat;

    cv::Mat structuringElement = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));

    cv::morphologyEx(imgGrayscale, imgTopHat, CV_MOP_TOPHAT, structuringElement);       //保留影像小亮點
    cv::morphologyEx(imgGrayscale, imgBlackHat, CV_MOP_BLACKHAT, structuringElement);

    imgGrayscalePlusTopHat = imgGrayscale + imgTopHat;
    imgGrayscalePlusTopHatMinusBlackHat = imgGrayscalePlusTopHat - imgBlackHat;         //原影像+小亮點+小暗點(將輪廓強化)

    return(imgGrayscalePlusTopHatMinusBlackHat);
}

//---------------------------PossibleChar----------------------------//
PossibleChar::PossibleChar(std::vector<cv::Point> _contour) {
    contour = _contour;

    boundingRect = cv::boundingRect(contour);

    intCenterX = (boundingRect.x + boundingRect.x + boundingRect.width) / 2;
    intCenterY = (boundingRect.y + boundingRect.y + boundingRect.height) / 2;

    dblDiagonalSize = sqrt(pow(boundingRect.width, 2) + pow(boundingRect.height, 2));

    dblAspectRatio = (float)boundingRect.width / (float)boundingRect.height;
}


//---------------------------Main----------------------------//
//int MainWindow::main(void) {

//    bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN();           // attempt KNN training

//    if (blnKNNTrainingSuccessful == false) {                            // if KNN training was not successful
//                                                                        // show error message
//        std::cout << std::endl << std::endl << "error: error: KNN traning was not successful" << std::endl << std::endl;
//        return(0);                                                      // and exit program
//    }

//    cv::Mat imgOriginalScene;           // input image

//    QString file = QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/HW6", tr("Images File (*.bmp *.jpg)"));     //打開檔案並讀作字串
//    std::string image_from_file = file.toStdString();
//    imgOriginalScene = cv::imread(image_from_file, cv::IMREAD_COLOR);        // open image

//    if (imgOriginalScene.empty()) {                             // if unable to open image
//        std::cout << "error: image not read from file\n\n";     // show error message on command line
//        _getch();                                               // may have to modify this line if not using Windows
//        return(0);                                              // and exit program
//    }

//    std::vector<PossiblePlate> vectorOfPossiblePlates = detectPlatesInScene(imgOriginalScene);          // detect plates

//    vectorOfPossiblePlates = detectCharsInPlates(vectorOfPossiblePlates);                               // detect chars in plates

//    cv::imshow("imgOriginalScene", imgOriginalScene);           // show scene image

//    if (vectorOfPossiblePlates.empty()) {                                               // if no plates were found
//        std::cout << std::endl << "no license plates were detected" << std::endl;       // inform user no plates were found
//    }
//    else {                                                                            // else
//                                                                                      // if we get in here vector of possible plates has at leat one plate

//                                                                                      // sort the vector of possible plates in DESCENDING order (most number of chars to least number of chars)
//        std::sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);

//        // suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
//        PossiblePlate licPlate = vectorOfPossiblePlates.front();

//        cv::imshow("imgPlate", licPlate.imgPlate);            // show crop of plate and threshold of plate
//        cv::imshow("imgThresh", licPlate.imgThresh);

//        if (licPlate.strChars.length() == 0) {                                                      // if no chars were found in the plate
//            std::cout << std::endl << "no characters were detected" << std::endl << std::endl;      // show message
//            return(0);                                                                              // and exit program
//        }

//        drawRedRectangleAroundPlate(imgOriginalScene, licPlate);                // draw red rectangle around plate

//        std::cout << std::endl << "license plate read from image = " << licPlate.strChars << std::endl;     // write license plate text to std out
//        std::cout << std::endl << "-----------------------------------------" << std::endl;

//        writeLicensePlateCharsOnImage(imgOriginalScene, licPlate);              // write license plate text on the image

//        cv::imshow("imgOriginalScene", imgOriginalScene);                       // re-show scene image

//        cv::imwrite("imgOriginalScene.png", imgOriginalScene);                  // write image out to file
//    }

//    cv::waitKey(0);                 // hold windows open until user presses a key

//    return(0);
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::drawRedRectangleAroundPlate(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
    cv::Point2f p2fRectPoints[4];

    licPlate.rrLocationOfPlateInScene.points(p2fRectPoints);            // get 4 vertices of rotated rect

    for (int i = 0; i < 4; i++) {                                       // draw 4 red lines
        cv::line(imgOriginalScene, p2fRectPoints[i], p2fRectPoints[(i + 1) % 4], SCALAR_RED, 2);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::writeLicensePlateCharsOnImage(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
    cv::Point ptCenterOfTextArea;                   // this will be the center of the area the text will be written to
    cv::Point ptLowerLeftTextOrigin;                // this will be the bottom left of the area that the text will be written to

    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;                              // choose a plain jane font
    double dblFontScale = (double)licPlate.imgPlate.rows / 30.0;            // base font scale on height of plate area
    int intFontThickness = (int)std::round(dblFontScale * 1.5);             // base font thickness on font scale
    int intBaseline = 0;

    cv::Size textSize = cv::getTextSize(licPlate.strChars, intFontFace, dblFontScale, intFontThickness, &intBaseline);      // call getTextSize

    ptCenterOfTextArea.x = (int)licPlate.rrLocationOfPlateInScene.center.x;         // the horizontal location of the text area is the same as the plate

    if (licPlate.rrLocationOfPlateInScene.center.y < (imgOriginalScene.rows * 0.75)) {      // if the license plate is in the upper 3/4 of the image
                                                                                            // write the chars in below the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) + (int)std::round((double)licPlate.imgPlate.rows * 1.6);
    }
    else {                                                                                // else if the license plate is in the lower 1/4 of the image
                                                                                          // write the chars in above the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) - (int)std::round((double)licPlate.imgPlate.rows * 1.6);
    }

    ptLowerLeftTextOrigin.x = (int)(ptCenterOfTextArea.x - (textSize.width / 2));           // calculate the lower left origin of the text area
    ptLowerLeftTextOrigin.y = (int)(ptCenterOfTextArea.y + (textSize.height / 2));          // based on the text area center, width, and height

                                                                                            // write the text on the image
    cv::putText(imgOriginalScene, licPlate.strChars, ptLowerLeftTextOrigin, intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<PossiblePlate> MainWindow::detectPlatesInScene(cv::Mat &imgOriginalScene) {
    std::vector<PossiblePlate> vectorOfPossiblePlates;			// this will be the return value

    cv::Mat imgGrayscaleScene;
    cv::Mat imgThreshScene;
    cv::Mat imgContours(imgOriginalScene.size(), CV_8UC3, SCALAR_BLACK);

    cv::RNG rng;

    cv::destroyAllWindows();

#ifdef SHOW_STEPS
    cv::imshow("0", imgOriginalScene);
#endif	// SHOW_STEPS

    preprocess(imgOriginalScene, imgGrayscaleScene, imgThreshScene);        // preprocess to get grayscale and threshold images

#ifdef SHOW_STEPS
    cv::imshow("1a", imgGrayscaleScene);
    cv::imshow("1b", imgThreshScene);
#endif	// SHOW_STEPS

    // find all possible chars in the scene,
    // this function first finds all contours, then only includes contours that could be chars (without comparison to other chars yet)
    std::vector<PossibleChar> vectorOfPossibleCharsInScene = findPossibleCharsInScene(imgThreshScene);

#ifdef SHOW_STEPS
    std::cout << "step 2 - vectorOfPossibleCharsInScene.Count = " << vectorOfPossibleCharsInScene.size() << std::endl;        // 131 with MCLRNF1 image

    imgContours = cv::Mat(imgOriginalScene.size(), CV_8UC3, SCALAR_BLACK);
    std::vector<std::vector<cv::Point> > contours;

    for (auto &possibleChar : vectorOfPossibleCharsInScene) {
        contours.push_back(possibleChar.contour);
    }
    cv::drawContours(imgContours, contours, -1, SCALAR_WHITE);
    cv::imshow("2b", imgContours);
#endif	// SHOW_STEPS

    // given a vector of all possible chars, find groups of matching chars
    // in the next steps each group of matching chars will attempt to be recognized as a plate
    std::vector<std::vector<PossibleChar> > vectorOfVectorsOfMatchingCharsInScene = findVectorOfVectorsOfMatchingChars(vectorOfPossibleCharsInScene);

#ifdef SHOW_STEPS
    std::cout << "step 3 - vectorOfVectorsOfMatchingCharsInScene.size() = " << vectorOfVectorsOfMatchingCharsInScene.size() << std::endl;        // 13 with MCLRNF1 image

    imgContours = cv::Mat(imgOriginalScene.size(), CV_8UC3, SCALAR_BLACK);

    for (auto &vectorOfMatchingChars : vectorOfVectorsOfMatchingCharsInScene) {
        int intRandomBlue = rng.uniform(0, 256);
        int intRandomGreen = rng.uniform(0, 256);
        int intRandomRed = rng.uniform(0, 256);

        std::vector<std::vector<cv::Point> > contours;

        for (auto &matchingChar : vectorOfMatchingChars) {
            contours.push_back(matchingChar.contour);
        }
        cv::drawContours(imgContours, contours, -1, cv::Scalar((double)intRandomBlue, (double)intRandomGreen, (double)intRandomRed));
    }
    cv::imshow("3", imgContours);
#endif	// SHOW_STEPS

    for (auto &vectorOfMatchingChars : vectorOfVectorsOfMatchingCharsInScene) {                     // for each group of matching chars
        PossiblePlate possiblePlate = extractPlate(imgOriginalScene, vectorOfMatchingChars);        // attempt to extract plate

        if (possiblePlate.imgPlate.empty() == false) {                                              // if plate was found
            vectorOfPossiblePlates.push_back(possiblePlate);                                        // add to vector of possible plates
        }
    }

    std::cout << std::endl << vectorOfPossiblePlates.size() << " possible plates found" << std::endl;       // 13 with MCLRNF1 image

#ifdef SHOW_STEPS
    std::cout << std::endl;
    cv::imshow("4a", imgContours);

    for (unsigned int i = 0; i < vectorOfPossiblePlates.size(); i++) {
        cv::Point2f p2fRectPoints[4];

        vectorOfPossiblePlates[i].rrLocationOfPlateInScene.points(p2fRectPoints);

        for (int j = 0; j < 4; j++) {
            cv::line(imgContours, p2fRectPoints[j], p2fRectPoints[(j + 1) % 4], SCALAR_RED, 2);
        }
        cv::imshow("4a", imgContours);

        std::cout << "possible plate " << i << ", click on any image and press a key to continue . . ." << std::endl;

        cv::imshow("4b", vectorOfPossiblePlates[i].imgPlate);
        cv::waitKey(0);
    }
    std::cout << std::endl << "plate detection complete, click on any image and press a key to begin char recognition . . ." << std::endl << std::endl;
    cv::waitKey(0);
#endif	// SHOW_STEPS

    return vectorOfPossiblePlates;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<PossibleChar> MainWindow::findPossibleCharsInScene(cv::Mat &imgThresh) {
    std::vector<PossibleChar> vectorOfPossibleChars;            // this will be the return value

    cv::Mat imgContours(imgThresh.size(), CV_8UC3, SCALAR_BLACK);
    int intCountOfPossibleChars = 0;

    cv::Mat imgThreshCopy = imgThresh.clone();

    std::vector<std::vector<cv::Point> > contours;

    cv::findContours(imgThreshCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);        // find all contours

    for (unsigned int i = 0; i < contours.size(); i++) {                // for each contour
#ifdef SHOW_STEPS
        cv::drawContours(imgContours, contours, i, SCALAR_WHITE);
#endif	// SHOW_STEPS
        PossibleChar possibleChar(contours[i]);

        if (checkIfPossibleChar(possibleChar)) {                // if contour is a possible char, note this does not compare to other chars (yet) . . .
            intCountOfPossibleChars++;                          // increment count of possible chars
            vectorOfPossibleChars.push_back(possibleChar);      // and add to vector of possible chars
        }
    }

#ifdef SHOW_STEPS
    std::cout << std::endl << "contours.size() = " << contours.size() << std::endl;                         // 2362 with MCLRNF1 image
    std::cout << "step 2 - intCountOfValidPossibleChars = " << intCountOfPossibleChars << std::endl;        // 131 with MCLRNF1 image
    cv::imshow("2a", imgContours);
#endif	// SHOW_STEPS

    return(vectorOfPossibleChars);
}

//---------------------------DetectPlate----------------------------//
PossiblePlate MainWindow::extractPlate(cv::Mat &imgOriginal, std::vector<PossibleChar> &vectorOfMatchingChars) {
    PossiblePlate possiblePlate;            // this will be the return value

                                            // sort chars from left to right based on x position
    std::sort(vectorOfMatchingChars.begin(), vectorOfMatchingChars.end(), PossibleChar::sortCharsLeftToRight);

    // calculate the center point of the plate
    double dblPlateCenterX = (double)(vectorOfMatchingChars[0].intCenterX + vectorOfMatchingChars[vectorOfMatchingChars.size() - 1].intCenterX) / 2.0;
    double dblPlateCenterY = (double)(vectorOfMatchingChars[0].intCenterY + vectorOfMatchingChars[vectorOfMatchingChars.size() - 1].intCenterY) / 2.0;
    cv::Point2d p2dPlateCenter(dblPlateCenterX, dblPlateCenterY);

    // calculate plate width and height
    int intPlateWidth = (int)((vectorOfMatchingChars[vectorOfMatchingChars.size() - 1].boundingRect.x + vectorOfMatchingChars[vectorOfMatchingChars.size() - 1].boundingRect.width - vectorOfMatchingChars[0].boundingRect.x) * PLATE_WIDTH_PADDING_FACTOR);

    double intTotalOfCharHeights = 0;

    for (auto &matchingChar : vectorOfMatchingChars) {
        intTotalOfCharHeights = intTotalOfCharHeights + matchingChar.boundingRect.height;
    }

    double dblAverageCharHeight = (double)intTotalOfCharHeights / vectorOfMatchingChars.size();

    int intPlateHeight = (int)(dblAverageCharHeight * PLATE_HEIGHT_PADDING_FACTOR);

    // calculate correction angle of plate region
    double dblOpposite = vectorOfMatchingChars[vectorOfMatchingChars.size() - 1].intCenterY - vectorOfMatchingChars[0].intCenterY;
    double dblHypotenuse = distanceBetweenChars(vectorOfMatchingChars[0], vectorOfMatchingChars[vectorOfMatchingChars.size() - 1]);
    double dblCorrectionAngleInRad = asin(dblOpposite / dblHypotenuse);
    double dblCorrectionAngleInDeg = dblCorrectionAngleInRad * (180.0 / CV_PI);

    // assign rotated rect member variable of possible plate
    possiblePlate.rrLocationOfPlateInScene = cv::RotatedRect(p2dPlateCenter, cv::Size2f((float)intPlateWidth, (float)intPlateHeight), (float)dblCorrectionAngleInDeg);

    cv::Mat rotationMatrix;             // final steps are to perform the actual rotation
    cv::Mat imgRotated;
    cv::Mat imgCropped;

    rotationMatrix = cv::getRotationMatrix2D(p2dPlateCenter, dblCorrectionAngleInDeg, 1.0);         // get the rotation matrix for our calculated correction angle

    cv::warpAffine(imgOriginal, imgRotated, rotationMatrix, imgOriginal.size());            // rotate the entire image

                                                                                            // crop out the actual plate portion of the rotated image
    cv::getRectSubPix(imgRotated, possiblePlate.rrLocationOfPlateInScene.size, possiblePlate.rrLocationOfPlateInScene.center, imgCropped);

    possiblePlate.imgPlate = imgCropped;            // copy the cropped plate image into the applicable member variable of the possible plate

    return(possiblePlate);
}

//---------------------------DetectChar----------------------------//

// global variables ///////////////////////////////////////////////////////////////////////////////
cv::Ptr<cv::ml::KNearest> kNearest = cv::ml::KNearest::create();

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MainWindow::loadKNNDataAndTrainKNN(void) {

    // read in training classifications ///////////////////////////////////////////////////

    cv::Mat matClassificationInts;              // we will read the classification numbers into this variable as though it is a vector

    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);        // open the classifications file

    if (fsClassifications.isOpened() == false) {                                                        // if the file was not opened successfully
        std::cout << "error, unable to open training classifications file, exiting program\n\n";        // show error message
        return(false);                                                                                  // and exit program
    }

    fsClassifications["classifications"] >> matClassificationInts;          // read classifications section into Mat classifications variable
    fsClassifications.release();                                            // close the classifications file

    // read in training images ////////////////////////////////////////////////////////////

    cv::Mat matTrainingImagesAsFlattenedFloats;         // we will read multiple images into this single image variable as though it is a vector

    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);              // open the training images file

    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
        return(false);                                                                          // and exit program
    }

    fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
    fsTrainingImages.release();                                                 // close the traning images file

                                                                                // train //////////////////////////////////////////////////////////////////////////////

                                                                                // finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
                                                                                // even though in reality they are multiple images / numbers
    kNearest->setDefaultK(1);

    kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<PossiblePlate> MainWindow::detectCharsInPlates(std::vector<PossiblePlate> &vectorOfPossiblePlates) {
    int intPlateCounter = 0;				// this is only for showing steps
    cv::Mat imgContours;
    std::vector<std::vector<cv::Point> > contours;
    cv::RNG rng;

    if (vectorOfPossiblePlates.empty()) {               // if vector of possible plates is empty
        return(vectorOfPossiblePlates);                 // return
    }
    // at this point we can be sure vector of possible plates has at least one plate

    for (auto &possiblePlate : vectorOfPossiblePlates) {            // for each possible plate, this is a big for loop that takes up most of the function

        preprocess(possiblePlate.imgPlate, possiblePlate.imgGrayscale, possiblePlate.imgThresh);        // preprocess to get grayscale and threshold images

#ifdef SHOW_STEPS
        cv::imshow("5a", possiblePlate.imgPlate);
        cv::imshow("5b", possiblePlate.imgGrayscale);
        cv::imshow("5c", possiblePlate.imgThresh);
#endif	// SHOW_STEPS

        // upscale size by 60% for better viewing and character recognition
        cv::resize(possiblePlate.imgThresh, possiblePlate.imgThresh, cv::Size(), 1.6, 1.6);

        // threshold again to eliminate any gray areas
        cv::threshold(possiblePlate.imgThresh, possiblePlate.imgThresh, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);

#ifdef SHOW_STEPS
        cv::imshow("5d", possiblePlate.imgThresh);
#endif	// SHOW_STEPS

        // find all possible chars in the plate,
        // this function first finds all contours, then only includes contours that could be chars (without comparison to other chars yet)
        std::vector<PossibleChar> vectorOfPossibleCharsInPlate = findPossibleCharsInPlate(possiblePlate.imgGrayscale, possiblePlate.imgThresh);

#ifdef SHOW_STEPS
        imgContours = cv::Mat(possiblePlate.imgThresh.size(), CV_8UC3, SCALAR_BLACK);
        contours.clear();

        for (auto &possibleChar : vectorOfPossibleCharsInPlate) {
            contours.push_back(possibleChar.contour);
        }

        cv::drawContours(imgContours, contours, -1, SCALAR_WHITE);

        cv::imshow("6", imgContours);
#endif	// SHOW_STEPS

        // given a vector of all possible chars, find groups of matching chars within the plate
        std::vector<std::vector<PossibleChar> > vectorOfVectorsOfMatchingCharsInPlate = findVectorOfVectorsOfMatchingChars(vectorOfPossibleCharsInPlate);

#ifdef SHOW_STEPS
        imgContours = cv::Mat(possiblePlate.imgThresh.size(), CV_8UC3, SCALAR_BLACK);

        contours.clear();

        for (auto &vectorOfMatchingChars : vectorOfVectorsOfMatchingCharsInPlate) {
            int intRandomBlue = rng.uniform(0, 256);
            int intRandomGreen = rng.uniform(0, 256);
            int intRandomRed = rng.uniform(0, 256);

            for (auto &matchingChar : vectorOfMatchingChars) {
                contours.push_back(matchingChar.contour);
            }
            cv::drawContours(imgContours, contours, -1, cv::Scalar((double)intRandomBlue, (double)intRandomGreen, (double)intRandomRed));
        }
        cv::imshow("7", imgContours);
#endif	// SHOW_STEPS

        if (vectorOfVectorsOfMatchingCharsInPlate.size() == 0) {                // if no groups of matching chars were found in the plate
#ifdef SHOW_STEPS
            std::cout << "chars found in plate number " << intPlateCounter << " = (none), click on any image and press a key to continue . . ." << std::endl;
            intPlateCounter++;
            cv::destroyWindow("8");
            cv::destroyWindow("9");
            cv::destroyWindow("10");
            cv::waitKey(0);
#endif	// SHOW_STEPS
            possiblePlate.strChars = "";            // set plate string member variable to empty string
            continue;                               // go back to top of for loop
        }

        for (auto &vectorOfMatchingChars : vectorOfVectorsOfMatchingCharsInPlate) {                                         // for each vector of matching chars in the current plate
            std::sort(vectorOfMatchingChars.begin(), vectorOfMatchingChars.end(), PossibleChar::sortCharsLeftToRight);      // sort the chars left to right
            vectorOfMatchingChars = removeInnerOverlappingChars(vectorOfMatchingChars);                                     // and eliminate any overlapping chars
        }

#ifdef SHOW_STEPS
        imgContours = cv::Mat(possiblePlate.imgThresh.size(), CV_8UC3, SCALAR_BLACK);

        for (auto &vectorOfMatchingChars : vectorOfVectorsOfMatchingCharsInPlate) {
            int intRandomBlue = rng.uniform(0, 256);
            int intRandomGreen = rng.uniform(0, 256);
            int intRandomRed = rng.uniform(0, 256);

            contours.clear();

            for (auto &matchingChar : vectorOfMatchingChars) {
                contours.push_back(matchingChar.contour);
            }
            cv::drawContours(imgContours, contours, -1, cv::Scalar((double)intRandomBlue, (double)intRandomGreen, (double)intRandomRed));
        }
        cv::imshow("8", imgContours);
#endif	// SHOW_STEPS

        // within each possible plate, suppose the longest vector of potential matching chars is the actual vector of chars
        unsigned int intLenOfLongestVectorOfChars = 0;
        unsigned int intIndexOfLongestVectorOfChars = 0;
        // loop through all the vectors of matching chars, get the index of the one with the most chars
        for (unsigned int i = 0; i < vectorOfVectorsOfMatchingCharsInPlate.size(); i++) {
            if (vectorOfVectorsOfMatchingCharsInPlate[i].size() > intLenOfLongestVectorOfChars) {
                intLenOfLongestVectorOfChars = vectorOfVectorsOfMatchingCharsInPlate[i].size();
                intIndexOfLongestVectorOfChars = i;
            }
        }
        // suppose that the longest vector of matching chars within the plate is the actual vector of chars
        std::vector<PossibleChar> longestVectorOfMatchingCharsInPlate = vectorOfVectorsOfMatchingCharsInPlate[intIndexOfLongestVectorOfChars];

#ifdef SHOW_STEPS
        imgContours = cv::Mat(possiblePlate.imgThresh.size(), CV_8UC3, SCALAR_BLACK);

        contours.clear();

        for (auto &matchingChar : longestVectorOfMatchingCharsInPlate) {
            contours.push_back(matchingChar.contour);
        }
        cv::drawContours(imgContours, contours, -1, SCALAR_WHITE);

        cv::imshow("9", imgContours);
#endif	// SHOW_STEPS

        // perform char recognition on the longest vector of matching chars in the plate
        possiblePlate.strChars = recognizeCharsInPlate(possiblePlate.imgThresh, longestVectorOfMatchingCharsInPlate);

#ifdef SHOW_STEPS
        std::cout << "chars found in plate number " << intPlateCounter << " = " << possiblePlate.strChars << ", click on any image and press a key to continue . . ." << std::endl;
        intPlateCounter++;
        cv::waitKey(0);
#endif	// SHOW_STEPS

    }   // end for each possible plate big for loop that takes up most of the function

#ifdef SHOW_STEPS
    std::cout << std::endl << "char detection complete, click on any image and press a key to continue . . ." << std::endl;
    cv::waitKey(0);
#endif	// SHOW_STEPS

    return(vectorOfPossiblePlates);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<PossibleChar> MainWindow::findPossibleCharsInPlate(cv::Mat &imgGrayscale, cv::Mat &imgThresh) {
    std::vector<PossibleChar> vectorOfPossibleChars;                            // this will be the return value

    cv::Mat imgThreshCopy;

    std::vector<std::vector<cv::Point> > contours;

    imgThreshCopy = imgThresh.clone();				// make a copy of the thresh image, this in necessary b/c findContours modifies the image

    cv::findContours(imgThreshCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);        // find all contours in plate

    for (auto &contour : contours) {                            // for each contour
        PossibleChar possibleChar(contour);

        if (checkIfPossibleChar(possibleChar)) {                // if contour is a possible char, note this does not compare to other chars (yet) . . .
            vectorOfPossibleChars.push_back(possibleChar);      // add to vector of possible chars
        }
    }

    return(vectorOfPossibleChars);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MainWindow::checkIfPossibleChar(PossibleChar &possibleChar) {
    // this function is a 'first pass' that does a rough check on a contour to see if it could be a char,
    // note that we are not (yet) comparing the char to other chars to look for a group
    if (possibleChar.boundingRect.area() > MIN_PIXEL_AREA &&
        possibleChar.boundingRect.width > MIN_PIXEL_WIDTH && possibleChar.boundingRect.height > MIN_PIXEL_HEIGHT &&
        MIN_ASPECT_RATIO < possibleChar.dblAspectRatio && possibleChar.dblAspectRatio < MAX_ASPECT_RATIO) {
        return(true);
    }
    else {
        return(false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::vector<PossibleChar> > MainWindow::findVectorOfVectorsOfMatchingChars(const std::vector<PossibleChar> &vectorOfPossibleChars) {
    // with this function, we start off with all the possible chars in one big vector
    // the purpose of this function is to re-arrange the one big vector of chars into a vector of vectors of matching chars,
    // note that chars that are not found to be in a group of matches do not need to be considered further
    std::vector<std::vector<PossibleChar> > vectorOfVectorsOfMatchingChars;             // this will be the return value

    for (auto &possibleChar : vectorOfPossibleChars) {                  // for each possible char in the one big vector of chars

                                                                        // find all chars in the big vector that match the current char
        std::vector<PossibleChar> vectorOfMatchingChars = findVectorOfMatchingChars(possibleChar, vectorOfPossibleChars);

        vectorOfMatchingChars.push_back(possibleChar);          // also add the current char to current possible vector of matching chars

                                                                // if current possible vector of matching chars is not long enough to constitute a possible plate
        if (vectorOfMatchingChars.size() < MIN_NUMBER_OF_MATCHING_CHARS) {
            continue;                       // jump back to the top of the for loop and try again with next char, note that it's not necessary
                                            // to save the vector in any way since it did not have enough chars to be a possible plate
        }
        // if we get here, the current vector passed test as a "group" or "cluster" of matching chars
        vectorOfVectorsOfMatchingChars.push_back(vectorOfMatchingChars);            // so add to our vector of vectors of matching chars

                                                                                    // remove the current vector of matching chars from the big vector so we don't use those same chars twice,
                                                                                    // make sure to make a new big vector for this since we don't want to change the original big vector
        std::vector<PossibleChar> vectorOfPossibleCharsWithCurrentMatchesRemoved;

        for (auto &possChar : vectorOfPossibleChars) {
            if (std::find(vectorOfMatchingChars.begin(), vectorOfMatchingChars.end(), possChar) == vectorOfMatchingChars.end()) {
                vectorOfPossibleCharsWithCurrentMatchesRemoved.push_back(possChar);
            }
        }
        // declare new vector of vectors of chars to get result from recursive call
        std::vector<std::vector<PossibleChar> > recursiveVectorOfVectorsOfMatchingChars;

        // recursive call
        recursiveVectorOfVectorsOfMatchingChars = findVectorOfVectorsOfMatchingChars(vectorOfPossibleCharsWithCurrentMatchesRemoved);	// recursive call !!

        for (auto &recursiveVectorOfMatchingChars : recursiveVectorOfVectorsOfMatchingChars) {      // for each vector of matching chars found by recursive call
            vectorOfVectorsOfMatchingChars.push_back(recursiveVectorOfMatchingChars);               // add to our original vector of vectors of matching chars
        }

        break;		// exit for loop
    }

    return(vectorOfVectorsOfMatchingChars);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<PossibleChar> MainWindow::findVectorOfMatchingChars(const PossibleChar &possibleChar, const std::vector<PossibleChar> &vectorOfChars) {
    // the purpose of this function is, given a possible char and a big vector of possible chars,
    // find all chars in the big vector that are a match for the single possible char, and return those matching chars as a vector
    std::vector<PossibleChar> vectorOfMatchingChars;                // this will be the return value

    for (auto &possibleMatchingChar : vectorOfChars) {              // for each char in big vector

                                                                    // if the char we attempting to find matches for is the exact same char as the char in the big vector we are currently checking
        if (possibleMatchingChar == possibleChar) {
            // then we should not include it in the vector of matches b/c that would end up double including the current char
            continue;           // so do not add to vector of matches and jump back to top of for loop
        }
        // compute stuff to see if chars are a match
        double dblDistanceBetweenChars = distanceBetweenChars(possibleChar, possibleMatchingChar);
        double dblAngleBetweenChars = angleBetweenChars(possibleChar, possibleMatchingChar);
        double dblChangeInArea = (double)abs(possibleMatchingChar.boundingRect.area() - possibleChar.boundingRect.area()) / (double)possibleChar.boundingRect.area();
        double dblChangeInWidth = (double)abs(possibleMatchingChar.boundingRect.width - possibleChar.boundingRect.width) / (double)possibleChar.boundingRect.width;
        double dblChangeInHeight = (double)abs(possibleMatchingChar.boundingRect.height - possibleChar.boundingRect.height) / (double)possibleChar.boundingRect.height;

        // check if chars match
        if (dblDistanceBetweenChars < (possibleChar.dblDiagonalSize * MAX_DIAG_SIZE_MULTIPLE_AWAY) &&
            dblAngleBetweenChars < MAX_ANGLE_BETWEEN_CHARS &&
            dblChangeInArea < MAX_CHANGE_IN_AREA &&
            dblChangeInWidth < MAX_CHANGE_IN_WIDTH &&
            dblChangeInHeight < MAX_CHANGE_IN_HEIGHT) {
            vectorOfMatchingChars.push_back(possibleMatchingChar);      // if the chars are a match, add the current char to vector of matching chars
        }
    }

    return(vectorOfMatchingChars);          // return result
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// use Pythagorean theorem to calculate distance between two chars
double MainWindow::distanceBetweenChars(const PossibleChar &firstChar, const PossibleChar &secondChar) {
    int intX = abs(firstChar.intCenterX - secondChar.intCenterX);
    int intY = abs(firstChar.intCenterY - secondChar.intCenterY);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// use basic trigonometry(SOH CAH TOA) to calculate angle between chars
double MainWindow::angleBetweenChars(const PossibleChar &firstChar, const PossibleChar &secondChar) {
    double dblAdj = abs(firstChar.intCenterX - secondChar.intCenterX);
    double dblOpp = abs(firstChar.intCenterY - secondChar.intCenterY);

    double dblAngleInRad = atan(dblOpp / dblAdj);

    double dblAngleInDeg = dblAngleInRad * (180.0 / CV_PI);

    return(dblAngleInDeg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// if we have two chars overlapping or to close to each other to possibly be separate chars, remove the inner (smaller) char,
// this is to prevent including the same char twice if two contours are found for the same char,
// for example for the letter 'O' both the inner ring and the outer ring may be found as contours, but we should only include the char once
std::vector<PossibleChar> MainWindow::removeInnerOverlappingChars(std::vector<PossibleChar> &vectorOfMatchingChars) {
    std::vector<PossibleChar> vectorOfMatchingCharsWithInnerCharRemoved(vectorOfMatchingChars);

    for (auto &currentChar : vectorOfMatchingChars) {
        for (auto &otherChar : vectorOfMatchingChars) {
            if (currentChar != otherChar) {                         // if current char and other char are not the same char . . .
                                                                    // if current char and other char have center points at almost the same location . . .
                if (distanceBetweenChars(currentChar, otherChar) < (currentChar.dblDiagonalSize * MIN_DIAG_SIZE_MULTIPLE_AWAY)) {
                    // if we get in here we have found overlapping chars
                    // next we identify which char is smaller, then if that char was not already removed on a previous pass, remove it

                    // if current char is smaller than other char
                    if (currentChar.boundingRect.area() < otherChar.boundingRect.area()) {
                        // look for char in vector with an iterator
                        std::vector<PossibleChar>::iterator currentCharIterator = std::find(vectorOfMatchingCharsWithInnerCharRemoved.begin(), vectorOfMatchingCharsWithInnerCharRemoved.end(), currentChar);
                        // if iterator did not get to end, then the char was found in the vector
                        if (currentCharIterator != vectorOfMatchingCharsWithInnerCharRemoved.end()) {
                            vectorOfMatchingCharsWithInnerCharRemoved.erase(currentCharIterator);       // so remove the char
                        }
                    }
                    else {        // else if other char is smaller than current char
                                  // look for char in vector with an iterator
                        std::vector<PossibleChar>::iterator otherCharIterator = std::find(vectorOfMatchingCharsWithInnerCharRemoved.begin(), vectorOfMatchingCharsWithInnerCharRemoved.end(), otherChar);
                        // if iterator did not get to end, then the char was found in the vector
                        if (otherCharIterator != vectorOfMatchingCharsWithInnerCharRemoved.end()) {
                            vectorOfMatchingCharsWithInnerCharRemoved.erase(otherCharIterator);         // so remove the char
                        }
                    }
                }
            }
        }
    }

    return(vectorOfMatchingCharsWithInnerCharRemoved);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// this is where we apply the actual char recognition
std::string MainWindow::recognizeCharsInPlate(cv::Mat &imgThresh, std::vector<PossibleChar> &vectorOfMatchingChars) {
    std::string strChars;               // this will be the return value, the chars in the lic plate

    cv::Mat imgThreshColor;

    // sort chars from left to right
    std::sort(vectorOfMatchingChars.begin(), vectorOfMatchingChars.end(), PossibleChar::sortCharsLeftToRight);

    cv::cvtColor(imgThresh, imgThreshColor, CV_GRAY2BGR);       // make color version of threshold image so we can draw contours in color on it

    for (auto &currentChar : vectorOfMatchingChars) {           // for each char in plate
        cv::rectangle(imgThreshColor, currentChar.boundingRect, SCALAR_GREEN, 2);       // draw green box around the char

        cv::Mat imgROItoBeCloned = imgThresh(currentChar.boundingRect);                 // get ROI image of bounding rect

        cv::Mat imgROI = imgROItoBeCloned.clone();      // clone ROI image so we don't change original when we resize

        cv::Mat imgROIResized;
        // resize image, this is necessary for char recognition
        cv::resize(imgROI, imgROIResized, cv::Size(RESIZED_CHAR_IMAGE_WIDTH, RESIZED_CHAR_IMAGE_HEIGHT));

        cv::Mat matROIFloat;

        imgROIResized.convertTo(matROIFloat, CV_32FC1);         // convert Mat to float, necessary for call to findNearest

        cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);       // flatten Matrix into one row

        cv::Mat matCurrentChar(0, 0, CV_32F);                   // declare Mat to read current char into, this is necessary b/c findNearest requires a Mat

        kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!

        float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);       // convert current char from Mat to float

        strChars = strChars + char(int(fltCurrentChar));        // append current char to full string
    }

#ifdef SHOW_STEPS
    cv::imshow("10", imgThreshColor);
#endif	// SHOW_STEPS

    return(strChars);               // return result
}

void MainWindow::on_pushButton_clicked()
{
    bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN();           // attempt KNN training

    if (blnKNNTrainingSuccessful == false) {                            // if KNN training was not successful
                                                                        // show error message
        QMessageBox::warning(this, "warning", "KNN traning was not successful");
//        std::cout << std::endl << std::endl << "error: error: KNN traning was not successful" << std::endl << std::endl;
        return;                                                      // and exit program
    }

    cv::Mat imgOriginalScene;           // input image----++

    QString file = QFileDialog::getOpenFileName(this,tr("Select file"), "C:/DIP/term project", tr("Images File (*.jpeg *.jpg *.png)"));     //打開檔案並讀作字串
    std::string image_from_file = file.toStdString();
    imgOriginalScene = cv::imread(image_from_file, cv::IMREAD_COLOR);        // open image

    if (imgOriginalScene.empty()) {                             // if unable to open image
        QMessageBox::warning(this, "warning", "Please select an input image");
//        std::cout << "error: image not read from file\n\n";     // show error message on command line
        _getch();                                               // may have to modify this line if not using Windows
        return;                                              // and exit program
    }

    std::vector<PossiblePlate> vectorOfPossiblePlates = detectPlatesInScene(imgOriginalScene);          // detect plates

    vectorOfPossiblePlates = detectCharsInPlates(vectorOfPossiblePlates);                               // detect chars in plates

//    cv::imshow("imgOriginalScene", imgOriginalScene);           // show scene image

    if (vectorOfPossiblePlates.empty()) {                                               // if no plates were found
        QMessageBox::warning(this, "warning", "No license plates were detected");
//        std::cout << std::endl << "no license plates were detected" << std::endl;       // inform user no plates were found
    }
    else {                                                                            // else
                                                                                      // if we get in here vector of possible plates has at leat one plate

                                                                                      // sort the vector of possible plates in DESCENDING order (most number of chars to least number of chars)
        std::sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);

        // suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
        PossiblePlate licPlate = vectorOfPossiblePlates.front();

//        cv::imshow("imgPlate", licPlate.imgPlate);            // show crop of plate and threshold of plate
//        cv::imshow("imgThresh", licPlate.imgThresh);

        if (licPlate.strChars.length() == 0) {                                                      // if no chars were found in the plate
            QMessageBox::warning(this, "warning", "No characters were detected");
//            std::cout << std::endl << "no characters were detected" << std::endl << std::endl;      // show message
            return;                                                                              // and exit program
        }

        drawRedRectangleAroundPlate(imgOriginalScene, licPlate);                // draw red rectangle around plate

        std::cout << std::endl << "license plate read from image = " << licPlate.strChars << std::endl;     // write license plate text to std out
        std::cout << std::endl << "-----------------------------------------" << std::endl;

        ui->label_2->setText(QString::fromStdString(licPlate.strChars));

//        writeLicensePlateCharsOnImage(imgOriginalScene, licPlate);              // write license plate text on the image

//        cv::imshow("imgOriginalScene", imgOriginalScene);                       // re-show scene image

        QImage trans_img= QImage((uchar*) imgOriginalScene.data, imgOriginalScene.cols, imgOriginalScene.rows, imgOriginalScene.step, QImage::Format_BGR888);

        QPixmap OuputImage_pixmap = QPixmap::fromImage(trans_img);
        ui->label->setPixmap(OuputImage_pixmap.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));

//        cv::imwrite("imgOriginalScene.png", imgOriginalScene);                  // write image out to file
    }

    cv::waitKey(0);                 // hold windows open until user presses a key

    return;
}

