#include "CarColorPlateLocation.h"
#include "CarPlateRecgnize.h"


CarColorPlateLocation::CarColorPlateLocation(){

}

CarColorPlateLocation::~CarColorPlateLocation() {

}
void CarColorPlateLocation::thresholdCarPlate(Mat& src, Mat& dis, ColorHsv* normalHsv, ColorHsv* targetHsv){
    cvtColor(src,dis,COLOR_BGR2HSV);

    //3Õ®µ¿
    int chanles = dis.channels();
    //∏ﬂ
    int h = dis.rows;
    //øÌ ˝æ›≥§∂»
    int w = dis.cols * 3;
    //≈–∂œ ˝æ› «∑ÒŒ™“ª––¥Ê¥¢µƒ
    //ƒ⁄¥Ê◊„πªµƒª∞ matµƒ ˝æ› «“ªøÈ¡¨–¯µƒƒ⁄¥ÊΩ¯––¥Ê¥¢
    if (dis.isContinuous()) {
        w *= h;
        h = 1;
    }
    for (size_t i = 0; i < h; i++){
        //µ⁄i––µƒ ˝æ› “ª–– hsvµƒ ˝æ› uchar = java byte
        uchar* p = dis.ptr<uchar>(i);
        //¥¶¿Ì3∏ˆ ˝æ› À˘“‘≤Ω≥§Œ™3
        for (size_t j = 0; j < w; j+=3){
            int h = int(p[j]);
            int s = int(p[j+1]);
            int v = int(p[j + 2]);
            // «∑ÒŒ™¿∂…´œÒÀÿµ„µƒ±Íº«
            bool isColorPlate = false;
            //¿∂…´
            if (h >= 100 && h <= 124 && s >= 125 && s <= 255 && v >= 100  && v <= 255) {
                //蓝色
                isColorPlate = true;
            }else if (h >= 11 && h <= 34 && s >= 100 && s <= 255 && v >= 100 && v <= 255) {
                //黄色
                isColorPlate = true;
            }
            //∞—¿∂…´œÒÀÿ Õπœ‘≥ˆ¿¥ £¨∆‰À˚µƒ«¯”Ú»´±‰≥…∫⁄…´

            if (isColorPlate) {
                p[j] = targetHsv->hsv_h;
                p[j+1] = targetHsv->hsv_s;
                p[j + 2] = targetHsv->hsv_v;
            }else {
                //hsv ƒ£–Õ h£∫0 ∫Ï…´ ¡¡∂»∫Õ±•∫Õ∂»∂º «0 £¨“≤æÕ±‰≥…¡À∫⁄…´
                p[j] = normalHsv->hsv_h;
                p[j + 1] = normalHsv->hsv_s;
                p[j + 2] = normalHsv->hsv_v;
            }
        }
    }

}


void CarColorPlateLocation::location(Mat src, vector<PlateBean>& dst) {
    //1°¢◊™≥…HSV
    Mat hsv;

    ColorHsv* normalHsv = new ColorHsv();
    normalHsv->hsv_h = 0;
    normalHsv->hsv_s = 0;
    normalHsv->hsv_v = 0;

    ColorHsv* targetHsv = new ColorHsv();
    targetHsv->hsv_h = 0;
    targetHsv->hsv_s = 0;
    targetHsv->hsv_v = 255;


    thresholdCarPlate(src, hsv,normalHsv, targetHsv);

    delete normalHsv;
    delete targetHsv;
    //∞—¡¡∂» ˝æ›≥È≥ˆ¿¥
    //∞—h°¢s°¢v∑÷¿Î≥ˆ¿¥
    vector<Mat> hsv_split;
    split(hsv, hsv_split);
    
    //∂˛÷µªØ
    Mat shold;
    threshold(hsv_split[2], shold, 0, 255, THRESH_OTSU + THRESH_BINARY);

    Mat close;
    Mat element = getStructuringElement(MORPH_RECT, Size(17, 3));
    morphologyEx(shold, close, MORPH_CLOSE, element);
    
    //6°¢≤È’“¬÷¿™
    //ªÒµ√≥ı≤Ω…∏—°≥µ≈∆¬÷¿™================================================================
    //¬÷¿™ºÏ≤‚
    vector< vector<Point> > contours;
    //≤È’“¬÷¿™ Ã·»°◊ÓÕ‚≤„µƒ¬÷¿™  Ω´Ω·π˚±‰≥…µ„–Ú¡–∑≈»Î ºØ∫œ
    findContours(close, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    //±È¿˙
    vector<RotatedRect> vec_color_roi;
    for (vector<Point> point : contours) {
        RotatedRect rotatedRect = minAreaRect(point);
        //rectangle(src, rotatedRect.boundingRect(), Scalar(255, 0, 255));
        //Ω¯––≥ı≤Ωµƒ…∏—° ∞—ÕÍ»´≤ª∑˚∫œµƒ¬÷¿™∏¯≈≈≥˝µÙ ( ±»»Á£∫1x1£¨5x1000 )
        if (verifySizes(rotatedRect)) {
            vec_color_roi.push_back(rotatedRect);
        }
    }
    
    tortuosity(src, vec_color_roi, dst);
    /*for (Mat s : dst) {
        imshow("∫Ú—°2", s);
        waitKey();
    }*/
}


