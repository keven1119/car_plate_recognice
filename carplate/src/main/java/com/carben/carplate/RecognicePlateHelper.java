package com.carben.carplate;

public class RecognicePlateHelper {


    static {
        System.loadLibrary("car-plate-recognice");
    }


    public native void getPlateMsg(
            String hogPlateShapeModel,
            String hogAnnZhModel,
            String hogAnnModel,
            String picFilePath,
            OnPlateMsgListener onPlateMsgListener);
}
