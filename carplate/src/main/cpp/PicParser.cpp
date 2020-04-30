//
// Created by yons on 21.4.20.
//

#include "PicParser.h"
#include "recgnize/CarPlateRecgnize.h"
#include "recgnize/ParsePicParam.h"

PlateInPicMsgBean* PicParser::parsePic(ParsePicParam *param) {

    PlateInPicMsgBean* plateInPicMsgBean = new PlateInPicMsgBean();

    try {
        CarPlateRecgnize p(param->hog_model_file_path,
                           param->hog_ann_zh_file_path,
                           param->hog_ann_file_path);

        Mat src = imread( String(param->pic_file_path));

        plateInPicMsgBean =  p.plateRecgnize(src);

        plateInPicMsgBean->picWidth = src.rows;
        plateInPicMsgBean->picHeight = src.cols;
        plateInPicMsgBean->picFilePath = param->pic_file_path;




        return plateInPicMsgBean;


//    CarPlateRecgnize p("/Users/yons/Downloads/CarTrain/resource/HOG_SVM_DATA2.xml",
//                       "/Users/yons/Downloads/CarTrain/resource/HOG_ANN_ZH_DATA2.xml",
//                       "/Users/yons/Downloads/CarTrain/resource/HOG_ANN_DATA2.xml");
//    Mat src = imread("/Users/yons/Downloads/chepai1.jpg");
//        pthread_mutex_unlock(&mutex);

    }catch(...) {
//        pthread_mutex_unlock(&mutex);


    }

    return plateInPicMsgBean;

}

PicParser::PicParser() {
}

PicParser::~PicParser() {
}
