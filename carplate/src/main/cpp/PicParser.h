//
// Created by yons on 21.4.20.
//

#ifndef CARBEN_CHEPAI_RECGNIZE_PICPARSER_H
#define CARBEN_CHEPAI_RECGNIZE_PICPARSER_H

#include "recgnize/PlateBean.h"
#include "recgnize/ParsePicParam.h"
#include "JavaCallHelper.h"
#include "include/opencv2/core/mat.hpp"
#include <pthread.h>
#include <PlateInPicMsgBean.h>


using namespace cv;
using namespace std;


class PicParser{

public:
//    JavaCallHelper* javaCallHelper=0;

    PicParser();
    ~PicParser();

    void parsePic(ParsePicParam* parsePicParam, vector<PlateInPicMsgBean*>& plateInPicList);


};

#endif //CARBEN_CHEPAI_RECGNIZE_PICPARSER_H
