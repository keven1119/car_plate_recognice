/**
 * @author Lance
 * @date 2018/8/6
 */

#include "JavaCallHelper.h"

JavaCallHelper::JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject &_jobj) : javaVM(_javaVM),
                                                                                env(_env) {
    jobj = env->NewGlobalRef(_jobj);
    jclass jclazz = env->GetObjectClass(jobj);

    jmid_parse_pic_fail = env->GetMethodID(jclazz, "onParsePlateFail", "()V");
    jmid_parse_pic_suc = env->GetMethodID(jclazz, "onParsePlateSuc", "(Ljava/lang/String;FFFFF)V");
}

JavaCallHelper::~JavaCallHelper() {
    env->DeleteGlobalRef(jobj);
}

void JavaCallHelper::parsePlateFail() {

//    JNIEnv *jniEnv;
//    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
//        return;
//    }
    env->CallVoidMethod(jobj, jmid_parse_pic_fail);
//    javaVM->DetachCurrentThread();


}

void JavaCallHelper::parsePlateSuc(PlateInPicMsgBean* picMsgBean) {

//    JNIEnv *jniEnv;
//    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
//        return;
//    }
    env->CallVoidMethod(jobj, jmid_parse_pic_suc,
            env->NewStringUTF(picMsgBean->plate),
            picMsgBean->offsetCenterX,
            picMsgBean->offsetCenterY,
            picMsgBean->offsetWidth,
            picMsgBean->offsetHeight,
            picMsgBean->angle);
//    javaVM->DetachCurrentThread();

}
