#include <jni.h>
#include <string>
#include <pthread.h>
#include "recgnize/CarPlateRecgnize.h"
#include "recgnize/ParsePicParam.h"
#include "JavaCallHelper.h"
#include "PicParser.h"


JavaVM *javaVm = 0;
pthread_mutex_t mutex;

int JNI_OnLoad(JavaVM *vm, void *r) {
    javaVm = vm;
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_carben_carben_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT void JNICALL
Java_com_carben_carplate_RecognicePlateHelper_getPlateMsg(JNIEnv *env, jobject thiz,
                                                jstring hog_plate_shape_model,
                                                jstring hog_ann_zh_model, jstring hog_ann_model,
                                                jstring pic_file,
                                                jobject on_plate_msg_listener) {


    ParsePicParam* parsePicParam = new ParsePicParam();

    char* hog_model_file_path = const_cast<char *>(env->GetStringUTFChars(hog_plate_shape_model,0));
    char* hog_ann_zh_file_path = const_cast<char *>(env->GetStringUTFChars(hog_ann_zh_model, 0));
    char* hog_ann_file_path = const_cast<char *>(env->GetStringUTFChars(hog_ann_model, 0));
    char* pic_file_path = const_cast<char *>(env->GetStringUTFChars(pic_file, 0));

    parsePicParam->hog_model_file_path = new char[strlen(hog_model_file_path) +1];
    strcpy(parsePicParam->hog_model_file_path, hog_model_file_path);

    parsePicParam->hog_ann_zh_file_path = new char[strlen(hog_ann_zh_file_path)+1];
    strcpy(parsePicParam->hog_ann_zh_file_path, hog_ann_zh_file_path);

    parsePicParam->hog_ann_file_path = new char[strlen(hog_ann_file_path)+1];
    strcpy(parsePicParam->hog_ann_file_path, hog_ann_file_path);

    parsePicParam->pic_file_path = new char[strlen(pic_file_path)+1];
    strcpy(parsePicParam->pic_file_path, pic_file_path);

    parsePicParam->listenr = on_plate_msg_listener;
    parsePicParam->env = env;

//    pthread_mutex_init(&mutex, NULL);
//
//    pthread_t pid;


    JavaCallHelper* javaCallHelper = new JavaCallHelper(javaVm, env, on_plate_msg_listener);
    PicParser* parser = new PicParser(javaCallHelper);

    parser->parsePic(parsePicParam);


    env->ReleaseStringUTFChars(hog_plate_shape_model, hog_model_file_path);
    env->ReleaseStringUTFChars(hog_ann_zh_model, hog_ann_zh_file_path);
    env->ReleaseStringUTFChars(hog_ann_model, hog_ann_file_path);
    env->ReleaseStringUTFChars(pic_file, pic_file_path);


}