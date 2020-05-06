#include <jni.h>
#include <string>
#include <pthread.h>
#include "recgnize/CarPlateRecgnize.h"
#include "recgnize/ParsePicParam.h"
#include "JavaCallHelper.h"
#include "PicParser.h"


//JavaVM *javaVm = 0;
//pthread_mutex_t mutex;

//int JNI_OnLoad(JavaVM *vm, void *r) {
//    javaVm = vm;
//    return JNI_VERSION_1_6;
//}

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_carben_carben_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_carben_carplate_RecognicePlateHelper_getPlateMsg(JNIEnv *env, jobject thiz,
                                                          jstring hog_plate_shape_model,
                                                          jstring hog_ann_zh_model,
                                                          jstring hog_ann_model,
                                                          jstring pic_file) {
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

//    pthread_mutex_init(&mutex, NULL);
//
//    pthread_t pid;


    PicParser* parser = new PicParser();

    PlateInPicMsgBean* plateInPicMsgBean = parser->parsePic(parsePicParam);



    char* clazzName = "com/carben/carplate/PlateParam";
    // 找到对象的Java类
    jclass plateClazz = env->FindClass(clazzName);

    // 获取类的构造函数，记住这里是调用无参的构造函数
    jmethodID id = env->GetMethodID(plateClazz, "<init>", "()V");
    // 创建一个新的对象
    jobject plateMsg_ = env->NewObject(plateClazz, id);

    jfieldID plateNum = env->GetFieldID(plateClazz, "plateNum", "Ljava/lang/String;");
    jfieldID filePath = env->GetFieldID(plateClazz, "picFilePath", "Ljava/lang/String;");
    jfieldID offsetCenterX = env->GetFieldID(plateClazz, "offsetCenterX", "F");
    jfieldID offsetCenterY = env->GetFieldID(plateClazz, "offsetCenterY", "F");
    jfieldID offsetWidth = env->GetFieldID(plateClazz, "offsetWidth", "F");
    jfieldID offsetHeight = env->GetFieldID(plateClazz, "offsetHeight", "F");
    jfieldID angle = env->GetFieldID(plateClazz, "angle", "F");
    jfieldID picWidth = env->GetFieldID(plateClazz, "picWidth", "F");
    jfieldID picHeight = env->GetFieldID(plateClazz, "picHeight", "F");

    env->SetObjectField(plateMsg_, plateNum, env->NewStringUTF((*plateInPicMsgBean).plate));
    env->SetObjectField(plateMsg_, filePath, env->NewStringUTF((*plateInPicMsgBean).picFilePath));
    env->SetFloatField(plateMsg_, offsetCenterX, (*plateInPicMsgBean).offsetCenterX);
    env->SetFloatField(plateMsg_, offsetCenterY, (*plateInPicMsgBean).offsetCenterY);
    env->SetFloatField(plateMsg_, offsetWidth, (*plateInPicMsgBean).offsetWidth);
    env->SetFloatField(plateMsg_, offsetHeight, (*plateInPicMsgBean).offsetHeight);
    env->SetFloatField(plateMsg_, angle, (*plateInPicMsgBean).angle);
    env->SetFloatField(plateMsg_, picWidth, (*plateInPicMsgBean).picWidth);
    env->SetFloatField(plateMsg_, picHeight, (*plateInPicMsgBean).picHeight);


    delete plateInPicMsgBean;


    env->ReleaseStringUTFChars(hog_plate_shape_model, hog_model_file_path);
    env->ReleaseStringUTFChars(hog_ann_zh_model, hog_ann_zh_file_path);
    env->ReleaseStringUTFChars(hog_ann_model, hog_ann_file_path);
    env->ReleaseStringUTFChars(pic_file, pic_file_path);

    return plateMsg_;
}