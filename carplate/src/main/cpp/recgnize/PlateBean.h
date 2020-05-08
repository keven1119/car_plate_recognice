//
//  PlateBean.h
//  car_recognize_demo
//
//  Created by yons on 20.4.20.
//  Copyright © 2020 yons. All rights reserved.
//

#ifndef PlateBean_h
#define PlateBean_h
#include "../include/opencv2/opencv.hpp"

using namespace cv;

class PlateBean{
public:
    Mat plateMat;
    float offsetCenterX;
    float offsetCenterY;
    float offsetWidth;
    float offsetHeight;
    int type = 0; //1:通过颜色获取  ，2：通过sobel获取的

    float angle;
    
    void release(){
    
        this->plateMat.release();
        this->plateMat = NULL;
    
    }
};


#endif /* PlateBean_h */
