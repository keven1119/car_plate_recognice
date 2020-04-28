#ifndef CarPlateRecgnize_H
#define CarPlateRecgnize_H

#include "CarSobelPlateLocation.h"
#include "CarColorPlateLocation.h"
#include "PlateInPicMsgBean.h"
#include <string>
#include "../include/opencv2/ml.hpp"

using namespace std;
using namespace cv;
using namespace ml;



class CarPlateRecgnize {
public:
    CarPlateRecgnize(const char* svm_model, const char* ann_ch_path, const char* ann_path);
    ~CarPlateRecgnize();

    /*
    *      ∂±≥µ≈∆ ∑µªÿΩ·π˚∏¯µ˜”√’ﬂ
    *        1°¢∂®Œª
    *        2°¢ ∂±
    */
    PlateInPicMsgBean* plateRecgnize(Mat src);
private:
    void getHogFeatures(HOGDescriptor* svmHog,Mat src,Mat& out);

    void clearFixPoint(Mat& src);
    
    int verityCharSize(Mat src);

    int getCityIndex(vector<Rect> src);
    void getChineseRect(Rect city,Rect& chineseRect);

    void predict(vector<Mat> plateChar,string& result);

    CarSobelPlateLocation *plateLocation = 0;
    CarColorPlateLocation *plateColorLocation = 0;
    Ptr<SVM> svm;
    HOGDescriptor *svmHog = 0;


    HOGDescriptor *annHog = 0;
    Ptr<ANN_MLP> annCh;
    Ptr<ANN_MLP> ann;
    static string ZHCHARS[];
    static char CHARS[];

};


#endif
