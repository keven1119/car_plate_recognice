#ifndef CarColorPlateLocation_H
#define CarColorPlateLocation_H
#include "CarPlateLocation.h"
#include "ColorHsv.h"


class CarColorPlateLocation :public CarPlateLocation {
public:
    CarColorPlateLocation();
    ~CarColorPlateLocation();

    // 1°¢“™∂®ŒªµƒÕº∆¨ 2°¢“˝”√¿‡–Õ ◊˜Œ™∂®ŒªΩ·π˚
    void location(Mat src, vector<PlateBean>& dst);

    void thresholdCarPlate(Mat& src, Mat& dis, ColorHsv* normalHsv, ColorHsv* targetHsv );


private:
};
#endif // CarColorPlateLocation_H

