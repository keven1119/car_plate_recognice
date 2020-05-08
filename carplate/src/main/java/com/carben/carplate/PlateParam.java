package com.carben.carplate;

public  class PlateParam{
        String plateNum="";
        String picFilePath ="";
        float offsetCenterX=0f;
        float offsetCenterY=0f;
        float  offsetWidth =0f;
        float  offsetHeight=0f;
        float   angle=0f;
        float picWidth = 0f;
        float picHeight = 0f;

        public String getPlateNum() {
                return plateNum;
        }

        public String getPicFilePath() {
                return picFilePath;
        }

        public float getOffsetCenterX() {
                return offsetCenterX;
        }

        public float getOffsetCenterY() {
                return offsetCenterY;
        }

        public float getOffsetWidth() {
                return offsetWidth;
        }

        public float getOffsetHeight() {
                return offsetHeight;
        }

        public float getAngle() {
                return angle;
        }

        public float getPicWidth() {
                return picWidth;
        }

        public float getPicHeight() {
                return picHeight;
        }

        public void setAngle(float angle) {
                this.angle = angle;
        }
}