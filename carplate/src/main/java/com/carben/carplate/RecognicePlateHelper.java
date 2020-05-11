package com.carben.carplate;

import android.text.TextUtils;

import java.util.Iterator;
import java.util.List;

public class RecognicePlateHelper {


    static {
        System.loadLibrary("car-plate-recognice");
    }

//    public native void getString();

    public List<PlateParam> getRealPlateMsgList( String hogPlateShapeModel,
                                       String hogAnnZhModel,
                                       String hogAnnModel,
                                       String picFilePath){

        List<PlateParam> plateMsgList = getPlateMsg(hogPlateShapeModel, hogAnnZhModel, hogAnnModel, picFilePath);

        Iterator<PlateParam> iterator = plateMsgList.iterator();

        while (iterator.hasNext()){
            PlateParam next = iterator.next();
            if(TextUtils.isEmpty(next.plateNum)){
                iterator.remove();
            }else if(next.getOffsetHeight()>0 && next.getOffsetWidth()/ next.getOffsetHeight() > 5){
                iterator.remove();
            }else {
                correctPlateAngle(next);
            }
        }

        return plateMsgList;

    }

    private PlateParam correctPlateAngle(PlateParam plateMsg){
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

    public native List<PlateParam> getPlateMsg(
            String hogPlateShapeModel,
            String hogAnnZhModel,
            String hogAnnModel,
            String picFilePath);
}
