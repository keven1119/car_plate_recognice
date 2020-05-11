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


jstring chartoJstring(JNIEnv* env, const char* pat)
{
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}



jobject createJavaPlate(JNIEnv *env, PlateInPicMsgBean* plateInPicMsgBean){
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

    env->SetObjectField(plateMsg_, plateNum, chartoJstring(env, plateInPicMsgBean->plate));
    env->SetObjectField(plateMsg_, filePath, env->NewStringUTF((*plateInPicMsgBean).picFilePath));
    env->SetFloatField(plateMsg_, offsetCenterX, (*plateInPicMsgBean).offsetCenterX);
    env->SetFloatField(plateMsg_, offsetCenterY, (*plateInPicMsgBean).offsetCenterY);
    env->SetFloatField(plateMsg_, offsetWidth, (*plateInPicMsgBean).offsetWidth);
    env->SetFloatField(plateMsg_, offsetHeight, (*plateInPicMsgBean).offsetHeight);
    env->SetFloatField(plateMsg_, angle, (*plateInPicMsgBean).angle);
    env->SetFloatField(plateMsg_, picWidth, (*plateInPicMsgBean).picWidth);
    env->SetFloatField(plateMsg_, picHeight, (*plateInPicMsgBean).picHeight);


    delete plateInPicMsgBean;

    return plateMsg_;
}

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

//获取ArrayList类引用
    jclass list_jcs = env->FindClass("java/util/ArrayList");

    //获取ArrayList构造函数id
    jmethodID list_init = env->GetMethodID(list_jcs, "<init>", "()V");
    //创建一个ArrayList对象
    jobject list_obj = env->NewObject(list_jcs, list_init);
    //获取ArrayList对象的add()的methodID
    jmethodID list_add = env->GetMethodID(list_jcs, "add",
                                          "(Ljava/lang/Object;)Z");


    PicParser* parser = new PicParser();
    vector<PlateInPicMsgBean*> plateInPicList;
    parser->parsePic(parsePicParam, plateInPicList);

    for (int i = 0; i < plateInPicList.size(); i++) {

        PlateInPicMsgBean* plateInPic = plateInPicList[i];

        jobject plate_obj = createJavaPlate(env, plateInPic);

        env->CallBooleanMethod(list_obj, list_add, plate_obj);
    }

    plateInPicList.clear();


    env->ReleaseStringUTFChars(hog_plate_shape_model, hog_model_file_path);
    env->ReleaseStringUTFChars(hog_ann_zh_model, hog_ann_zh_file_path);
    env->ReleaseStringUTFChars(hog_ann_model, hog_ann_file_path);
    env->ReleaseStringUTFChars(pic_file, pic_file_path);

    return list_obj;
}

