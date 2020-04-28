//
// Created by yons on 20.4.20.
//

#ifndef CARBEN_CHEPAI_RECGNIZE_PARSEPICPARAM_H
#define CARBEN_CHEPAI_RECGNIZE_PARSEPICPARAM_H

#include <jni.h>
class ParsePicParam{

public:
    char *hog_model_file_path;
    char *hog_ann_zh_file_path=0;
    char *hog_ann_file_path =0;
    char *pic_file_path = 0;
    jobject listenr=0;
    JNIEnv *env = 0;

    void release(){
        if(hog_model_file_path){
            delete(hog_model_file_path);
            hog_model_file_path = 0;
        }
        if(hog_ann_zh_file_path){
            delete(hog_ann_zh_file_path);
            hog_ann_zh_file_path = 0;
        }
        if(hog_ann_file_path){
            delete(hog_ann_file_path);
            hog_ann_file_path = 0;
        }
        if(pic_file_path){
            delete(pic_file_path);
            pic_file_path = 0;
        }if(listenr && env){
            env->DeleteGlobalRef(listenr);
            listenr = 0;
            env = 0;
        }

    }
};



#endif //CARBEN_CHEPAI_RECGNIZE_PARSEPICPARAM_H
