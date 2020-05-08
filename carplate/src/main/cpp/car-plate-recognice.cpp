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



int checkUtfString(const char* bytes)
{
    const char* origBytes = bytes;
    if (bytes == NULL) {
        return -1;
    }
    while (*bytes != '\0') {
        unsigned char utf8 = *(bytes++);
        // Switch on the high four bits.
        switch (utf8 >> 4) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07: {
                // Bit pattern 0xxx. No need for any extra bytes.
                break;
            }
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0f: {
                /*printf("****JNI WARNING: illegal start byte 0x%x\n", utf8);*/
                return -1;
            }
            case 0x0e: {
                // Bit pattern 1110, so there are two additional bytes.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    /*printf("****JNI WARNING: illegal continuation byte 0x%x\n", utf8);*/
                    return -1;
                }
                // Fall through to take care of the final byte.
            }
            case 0x0c:
            case 0x0d: {
                // Bit pattern 110x, so there is one additional byte.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    /*printf("****JNI WARNING: illegal continuation byte 0x%x\n", utf8);*/
                    return -1;
                }
                break;
            }
        }
    }
    return 0;
}


void correctUtfBytes(char* bytes) {
    char three = 0;
    while (*bytes != '\0') {
        unsigned char utf8 = *(bytes++);
        three = 0;
        // Switch on the high four bits.
        switch (utf8 >> 4) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                // Bit pattern 0xxx. No need for any extra bytes.
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0f:
                /*
                 * Bit pattern 10xx or 1111, which are illegal start bytes.
                 * Note: 1111 is valid for normal UTF-8, but not the
                 * modified UTF-8 used here.
                 */
                *(bytes-1) = '?';
                break;
            case 0x0e:
                // Bit pattern 1110, so there are two additional bytes.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    --bytes;
                    *(bytes-1) = '?';
                    break;
                }
                three = 1;
                // Fall through to take care of the final byte.
            case 0x0c:
            case 0x0d:
                // Bit pattern 110x, so there is one additional byte.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    --bytes;
                    if(three)--bytes;
                    *(bytes-1)='?';
                }
                break;
        }
    }
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

    char* plateNumStr =(*plateInPicMsgBean).plate;
    if(!checkUtfString(plateNumStr)){
        correctUtfBytes(plateNumStr);
    }
    env->SetObjectField(plateMsg_, plateNum, env->NewStringUTF((const char*)plateNumStr));
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

