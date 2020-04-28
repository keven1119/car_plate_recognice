//
// Created by yons on 21.4.20.
//

#include "PicParser.h"
#include "recgnize/CarPlateRecgnize.h"
#include "recgnize/ParsePicParam.h"

void PicParser::parsePic(ParsePicParam *param) {

    try {

        CarPlateRecgnize p(param->hog_model_file_path,
                           param->hog_ann_zh_file_path,
                           param->hog_ann_file_path);

        Mat src = imread( String(param->pic_file_path));

        PlateInPicMsgBean* plate =  p.plateRecgnize(src);

        javaCallHelper->parsePlateSuc(plate);

//    CarPlateRecgnize p("/Users/yons/Downloads/CarTrain/resource/HOG_SVM_DATA2.xml",
//                       "/Users/yons/Downloads/CarTrain/resource/HOG_ANN_ZH_DATA2.xml",
//                       "/Users/yons/Downloads/CarTrain/resource/HOG_ANN_DATA2.xml");
//    Mat src = imread("/Users/yons/Downloads/chepai1.jpg");

        delete plate;
//        pthread_mutex_unlock(&mutex);

    }catch(...) {
//        pthread_mutex_unlock(&mutex);
        javaCallHelper->parsePlateFail();

    }

}

PicParser::PicParser(JavaCallHelper* callHelper) {


    this->javaCallHelper = callHelper;
}

PicParser::~PicParser() {
    delete (javaCallHelper);
}
