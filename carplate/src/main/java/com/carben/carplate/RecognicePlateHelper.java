package com.carben.carplate;

public class RecognicePlateHelper {


    static {
        System.loadLibrary("car-plate-recognice");
    }

//    public native void getString();

    public PlateParam getRealPlateMsg( String hogPlateShapeModel,
                                       String hogAnnZhModel,
                                       String hogAnnModel,
                                       String picFilePath){

        PlateParam plateMsg = getPlateMsg(hogPlateShapeModel, hogAnnZhModel, hogAnnModel, picFilePath);

        float angle = plateMsg.getAngle();

        if(angle >= -45f && angle <= 45f){
            plateMsg.setAngle(angle);
        }else if(angle < -45f){
            plateMsg.setAngle(-(-90 - angle));
        }else if(angle > 45f){
            plateMsg.setAngle(-(90 - angle));
        }

        return plateMsg;

    }

    public native PlateParam getPlateMsg(
            String hogPlateShapeModel,
            String hogAnnZhModel,
            String hogAnnModel,
            String picFilePath);
}
