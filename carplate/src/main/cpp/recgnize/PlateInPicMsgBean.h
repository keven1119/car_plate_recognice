//
//  PlateInPicMsgBean.h
//  carben_car_recgnize
//
//  Created by yons on 20.4.20.
//  Copyright © 2020 yons. All rights reserved.
//

#ifndef PlateInPicMsgBean_h
#define PlateInPicMsgBean_h

class PlateInPicMsgBean{
public:
    char* plate;
    float offsetWidth;
    float offsetHeight;
    float offsetCenterX;
    float offsetCenterY;
    float angle;
    float picWidth;
    float picHeight;
    char* picFilePath;
    
};


#endif /* PlateInPicMsgBean_h */
