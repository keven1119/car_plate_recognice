package com.carben.carplate;

public interface OnPlateMsgListener {

    void onParsePlateFail();

    void onParsePlateSuc(String plateNum, float offsetCenterX, float offsetCenterY, float offsetWidth, float offsetHeight, float angle);
}
