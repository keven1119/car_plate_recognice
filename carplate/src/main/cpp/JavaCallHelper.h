///**
// * @author Lance
// * @date 2018/8/6
// */
//
//#ifndef DNPLAYER_JAVACALLHELPER_H
//#define DNPLAYER_JAVACALLHELPER_H
//
//
//#include <jni.h>
//#include "recgnize/PlateInPicMsgBean.h"
//
//
//
//class JavaCallHelper {
//
//
//public:
//    JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject &_jobj);
//
//    ~JavaCallHelper();
//
//    void parsePlateFail();
//
//    void parsePlateSuc(PlateInPicMsgBean* plateInPicMsgBean);
//
//public:
//    JavaVM *javaVM;
//    jobject jobj;
//    jmethodID jmid_parse_pic_fail;
//    jmethodID jmid_parse_pic_suc;
//    JNIEnv *env;
//};
//
//
//#endif //DNPLAYER_JAVACALLHELPER_H
