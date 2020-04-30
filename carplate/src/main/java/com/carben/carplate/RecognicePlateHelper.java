package com.carben.carplate;

public class RecognicePlateHelper {


    static {
        System.loadLibrary("car-plate-recognice");
    }

//    public native void getString();


    public native PlateParam getPlateMsg(
            String hogPlateShapeModel,
            String hogAnnZhModel,
            String hogAnnModel,
            String picFilePath);
}
