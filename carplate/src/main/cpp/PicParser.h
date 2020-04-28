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

using namespace cv;


class PicParser{

public:
    JavaCallHelper* javaCallHelper=0;

    PicParser(JavaCallHelper* callHelper);
    ~PicParser();

    void parsePic(ParsePicParam* parsePicParam);


};

#endif //CARBEN_CHEPAI_RECGNIZE_PICPARSER_H
