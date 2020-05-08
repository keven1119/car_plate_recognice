//
// Created by yons on 21.4.20.
//

#include "PicParser.h"
#include "recgnize/CarPlateRecgnize.h"
#include "recgnize/ParsePicParam.h"

void PicParser::parsePic(ParsePicParam *param,vector<PlateInPicMsgBean*>& plateInPicList) {


    try {
        CarPlateRecgnize p(param->hog_model_file_path,
                           param->hog_ann_zh_file_path,
                           param->hog_ann_file_path);

        Mat src = imread( String(param->pic_file_path));


        p.plateRecgnize(src, plateInPicList);

        for (int i = 0; i < plateInPicList.size(); i++) {
            PlateInPicMsgBean* plateInPicMsgBean = plateInPicList[i];
            plateInPicMsgBean->picWidth = src.rows;
            plateInPicMsgBean->picHeight = src.cols;
            plateInPicMsgBean->picFilePath = param->pic_file_path;
        }


    }catch(...) {


    }

}

PicParser::PicParser() {
}

PicParser::~PicParser() {
}
