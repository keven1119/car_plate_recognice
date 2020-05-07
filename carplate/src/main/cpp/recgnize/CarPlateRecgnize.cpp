


#include "CarPlateRecgnize.h"


string CarPlateRecgnize::ZHCHARS[] = {"川","鄂","赣","甘","贵","桂","黑","沪","冀","津","京","吉","辽","鲁","蒙","闽","宁","青","琼","陕","苏","晋","皖","湘","新","豫","渝","粤","云","藏","浙"   };
char CarPlateRecgnize::CHARS[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z' };

CarPlateRecgnize::CarPlateRecgnize(const char* svm_model,const char* ann_ch_path,const char* ann_path) {
    plateLocation = new CarSobelPlateLocation();
    plateColorLocation = new CarColorPlateLocation();

    svm = SVM::load(svm_model);
    //参数1的宽-参数2的宽 结果与参数3的余数为0  高也一样
    svmHog = new HOGDescriptor(Size(128,64),Size(16,16),Size(8,8),Size(8,8),3);


    //创建特征提取对象
    annHog = new HOGDescriptor(Size(32, 32), Size(16, 16), Size(8, 8), Size(8, 8), 3);
    //识别汉字
    annCh = ANN_MLP::load(ann_ch_path);
    //识别模型
    ann = ANN_MLP::load(ann_path);
}


CarPlateRecgnize::~CarPlateRecgnize() {
    // Õ∑≈
    if (!plateLocation) {
        delete plateLocation;
        plateLocation = 0;
    }
    svm->clear();
    svm.release();
    if (svmHog) {
        delete svmHog;
        svmHog = 0;
    }
    if (annHog) {
        delete annHog;
        annHog = 0;
    }
    annCh->clear();
    annCh.release();
    ann->clear();
    ann.release();
}

/*
*     识别车牌 返回结果给调用者
*        1、定位
*        2、识别
*/
PlateInPicMsgBean* CarPlateRecgnize::plateRecgnize(Mat src) {

    PlateInPicMsgBean* plate_total_msg = new PlateInPicMsgBean();


    vector< PlateBean > sobel_plates;
    //sobel定位
    plateLocation->location(src, sobel_plates);

    //颜色定位
    vector< PlateBean > color_plates;
    plateColorLocation->location(src, color_plates);

    vector<PlateBean> plates;
    //把sobel_plates的内容 全部加入plates向量
    plates.insert(plates.end(),sobel_plates.begin(), sobel_plates.end());
    plates.insert(plates.end(), color_plates.begin(), color_plates.end());
    int index = -1;
    float minScore = FLT_MAX; //floatµƒ◊Ó¥Û÷µ
    //使用 svm 进行 评测
    for (int i = 0;i< plates.size();++i){
        Mat plate = plates[i].plateMat;


//        imshow("name",plate);
//        waitKey();



        //抽取车牌特征 HOG
        Mat gray;
        cvtColor(plate, gray,COLOR_BGR2GRAY);
        //二值化 必须是以单通道进行
        Mat shold;
        threshold(gray, shold, 0, 255, THRESH_OTSU + THRESH_BINARY);
        //提取特征
        Mat features;
        getHogFeatures(svmHog,shold, features);
        //把特征置为一行
        Mat samples = features.reshape(1, 1);

        //原始模式
        // svm: 直接告诉你这个数据是属于什么类型.
        // RAW_OUTPUT：让svm 给出一个评分
        /*char name[100];
        sprintf(name,"候选车牌%d",i);
        imshow(name,plate);*/

        float score = svm->predict(samples,noArray(), StatModel::Flags::RAW_OUTPUT);
        //printf("评分：%f\n",score);
        if (score < minScore) {
            minScore = score;
            index = i;
        }
        gray.release();
        shold.release();
        features.release();
        samples.release();
    }

//    Mat dst;
//    int plate_from_type = 0;


    if (index >= 0) {

        recognicePic(&(plates[index]),NULL,plate_total_msg);

        char* plateNum = plate_total_msg->plate;

        if(plateNum && *plateNum != '\0'){

        } else{
            recognicePic(&(plates[index]), plateColorLocation, plate_total_msg);
        }

//        PlateBean target_plate = plates[index];
//        plate_from_type = target_plate.type;
//
//        dst = target_plate.plateMat.clone();
//        plate_total_msg->offsetCenterX = target_plate.offsetCenterX;
//        plate_total_msg->offsetCenterY = target_plate.offsetCenterY;
//        plate_total_msg->angle = target_plate.angle;
//        plate_total_msg->offsetWidth = target_plate.offsetCenterWidth;
//        plate_total_msg->offsetHeight = target_plate.offsetCenterHeight;



    }else{
        return plate_total_msg;
    }
    // 释放
    for (PlateBean p : plates) {
        p.release();
    }

    return plate_total_msg;
}


void CarPlateRecgnize::recognicePic(PlateBean* target_plate, CarColorPlateLocation* carColorPlateLocation,PlateInPicMsgBean* plateInPicMsgBean){

    Mat dst;

    if (target_plate) {

        dst = target_plate->plateMat.clone();
        plateInPicMsgBean->offsetCenterX = target_plate->offsetCenterX;
        plateInPicMsgBean->offsetCenterY = target_plate->offsetCenterY;
        plateInPicMsgBean->angle = target_plate->angle;
        plateInPicMsgBean->offsetWidth = target_plate->offsetCenterWidth;
        plateInPicMsgBean->offsetHeight = target_plate->offsetCenterHeight;

    }else{
        return;
    }

    //    //imshow("车牌", dst);
    //    Mat plate_gray;
    //    cvtColor(dst, plate_gray,COLOR_BGR2GRAY);

    Mat plate_gray;

    if(carColorPlateLocation) {
        // 通过颜色识别出的图形，使用原来颜色的二值器进行二值化
        //将牌照背景和文字二值化
        Mat plate_threshold;

        ColorHsv *normalHsv = new ColorHsv();
        normalHsv->hsv_h = 255;
        normalHsv->hsv_s = 255;
        normalHsv->hsv_v = 255;

        ColorHsv *targetHsv = new ColorHsv();
        targetHsv->hsv_h = 0;
        targetHsv->hsv_s = 0;
        targetHsv->hsv_v = 0;
        carColorPlateLocation->thresholdCarPlate(dst, plate_threshold, normalHsv, targetHsv);

        cvtColor(plate_threshold, plate_threshold, COLOR_HSV2RGB);
        cvtColor(plate_threshold, plate_gray, COLOR_RGB2GRAY);


        plate_threshold.release();
        delete normalHsv;
        delete targetHsv;

    } else {
        cvtColor(dst,plate_gray, COLOR_BGR2GRAY);
    }


    Mat plate_shold;
    threshold(plate_gray, plate_shold,0,255, THRESH_OTSU + THRESH_BINARY);


    //去掉 车牌在车子上的固定点
    //因为固定点所在的行 它的黑到白、白到黑的改变次数是最小的
    clearFixPoint(plate_shold);

//    imshow("macth",dst);
//    waitKey();
//
//    imshow("clear fix plate_shold",plate_shold);
//    waitKey();


    //字符分割
    vector< vector<Point> > contours;
    //查找轮廓 提取最外层的轮廓  将结果变成点序列放入 集合
    findContours(plate_shold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    vector<Rect> charVec;
    for (vector<Point> point : contours) {
        Rect rect = boundingRect(point);
        //获得候选字符的图片
        Mat p  = plate_shold(rect);

//        imshow("char rect", p);
//        waitKey();

        // 进行初步的筛选
        if (verityCharSize(p))
        {
            charVec.push_back(rect);
        }
    }
    //集合中仍然会存在 非字符矩形
    //对集合中的矩形按照x进行一下排序，保证它们是从左到右的顺序
    sort(charVec.begin(), charVec.end(), [] (const Rect& r1, const Rect& r2){
        return r1.x < r2.x;
    });

    //汉字比较特殊
    // 如何拿汉字的矩形：获取城市字符的轮廓所在集合的下标 比如湘A ，那么A就是城市字符 代表长沙    int cityIndex =
    int cityIndex = getCityIndex(charVec);


    vector<Mat> plateChar;
    // ∂±
    String plate_str;

    //通过城市的下标 判断获取汉字轮廓
    Rect chineseRect;
    if(charVec.size()!=0 && cityIndex < charVec.size()){
        getChineseRect(charVec[cityIndex], chineseRect);
        plateChar.push_back(plate_shold(chineseRect));

        //包含了所有待识别的字符图片
        int cout = 0;
        for (size_t i = cityIndex; i < charVec.size();   i++)
        {
            plateChar.push_back(plate_shold(charVec[i]));
        }
        predict(plateChar,plate_str);

    } else{
        plateInPicMsgBean->offsetCenterX = 0.0;
        plateInPicMsgBean->offsetCenterY = 0.0;
        plateInPicMsgBean->offsetWidth = 0.0;
        plateInPicMsgBean->offsetHeight = 0.0;
    }


    plateInPicMsgBean->plate = const_cast<char *>(plate_str.c_str());

    plate_gray.release();
    plate_shold.release();
    for (int i = 0; i < contours.size(); i++) {
        contours[i].clear();
    }

    contours.clear();

    charVec.clear();

    plateChar.clear();



}


//Ã·»°Ãÿ’˜
void CarPlateRecgnize::getHogFeatures(HOGDescriptor* hog, Mat src, Mat& out) {
    //重新定义大小 缩放 提取特征的时候数据需要为  ：CV_32S 有符号的32位数据
    Mat trainImg = Mat(hog->winSize, CV_32S);
    resize(src, trainImg,hog->winSize);
    //计算特征 获得float集合
    vector<float> d;
    hog->compute(trainImg,d, Size(8, 8));

    Mat features(d);
    //特征矩阵
    features.copyTo(out);
    features.release();
    trainImg.release();
}


void CarPlateRecgnize::clearFixPoint(Mat& src) {
    //最大改变次数是10
    int maxChange = 10;
    //一个集合统计每一行的跳变次数
    vector<int> c;
    for (size_t i = 0; i < src.rows; i++)
    {
        //记录这一行的改变次数
        int change = 0;
        for (size_t j = 0; j < src.cols - 1; j++)
        {
            //获得像素值
            char p = src.at<char>(i, j);
            //当前的像素点与下一个像素点值是否相同
            if (p != src.at<char>(i, j + 1)) {
                change++;
            }
        }
        c.push_back(change);
    }
    for (size_t i = 0; i < c.size(); i++)
    {
        //取出每一行的改变次数
        int change = c[i];
        //如果小与max ，则可能就是干扰点所在的行
        if (change <= maxChange) {
            //把这一行都抹黑
            for (size_t j = 0; j < src.cols; j++)
            {
                src.at<char>(i, j) = 0;
            }
        }
    }
}

int CarPlateRecgnize::verityCharSize(Mat src) {
    //最理想情况 车牌字符的标准宽高比
    float aspect = 45.0f / 90;
    // 当前获得矩形的真实宽高比
    float realAspect = (float)src.cols / (float)src.rows;
    //最小的字符高
    float minHeight = 10.0f;
    //最大的字符高
    float maxHeight = 35.0f;
    //1、判断高符合范围  2、宽、高比符合范围
    //最大宽、高比 最小宽高比
    float error = 0.7f;
    float maxAspect = aspect + aspect * error;
    float minAspect = aspect - aspect * error;

    if (realAspect  >= minAspect && realAspect <= maxAspect && src.rows >= minHeight && src.rows <= maxHeight)
    {
        return 1;
    }
    return 0;
}


int CarPlateRecgnize::getCityIndex(vector<Rect> src) {
    int cityIndex = 0;
    //循环集合
    for (size_t i = 0; i < src.size(); i++)
    {
        Rect rect = src[i];
        //获得矩形
        //把车牌区域划分为7个字符
        //如果当前获得的矩形 它的中心点 比 1/7 大，比2/7小，那么就是城市的轮廓
        int midX = rect.x + rect.width / 2;
        if (midX < 136 / 7 * 2 && midX > 136 / 7) {
            cityIndex = i;
            break;
        }
    }

    return cityIndex;
}

void CarPlateRecgnize::getChineseRect(Rect city, Rect& chineseRect) {
    //把宽度稍微扩大一点
    float width = city.width * 1.15f;
    //城市轮廓的x坐标
    int x = city.x;

    //x ：当前汉字后面城市轮廓的x坐标
    //减去城市的宽
    int newX = x - width;
    chineseRect.x = newX >= 0 ? newX : 0;
    chineseRect.y = city.y;
    chineseRect.width = width;
    chineseRect.height = city.height;
}


void CarPlateRecgnize::predict(vector<Mat> vec,String& result) {
    for (size_t i = 0; i < vec.size(); i++)
    {
        //提取图片特征去进行识别
        Mat src = vec[i];
        Mat features;
        //提取hog特征
        getHogFeatures(annHog,src,features);
        Mat response;
        Point maxLoc;
        Point minLoc;
        //特征置为一行
        Mat samples = features.reshape(1, 1);
        if (i) {
            //识别字母与数字
            ann->predict(samples,response);
            //获取最大可信度 匹配度最高的属于31种中的哪一个。
            minMaxLoc(response, 0, 0, &minLoc, &maxLoc);
            //跟你训练时候有关
            int index = maxLoc.x;
            result += CHARS[index];
        } else {
            //识别汉字
            annCh->predict(samples, response);
            //获取最大可信度 匹配度最高的属于31种中的哪一个。
            minMaxLoc(response,0,0,&minLoc, &maxLoc);
            //跟你训练时候有关
            int index = maxLoc.x;
            //识别出来的汉字 拼到string当中去
            result += ZHCHARS[index];
        }
        cout << "匹配度:" << minLoc.x << endl;
    }
}
