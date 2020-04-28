

#ifndef CarSobelPlateLocation_H
#define CarSobelPlateLocation_H
#include "CarPlateLocation.h"




class CarSobelPlateLocation: public CarPlateLocation {
public:
    CarSobelPlateLocation();
    ~CarSobelPlateLocation();

    // 1°¢“™∂®ŒªµƒÕº∆¨ 2°¢“˝”√¿‡–Õ ◊˜Œ™∂®ŒªΩ·π˚
    void location(Mat src,vector<PlateBean>& dst);


};


#endif
