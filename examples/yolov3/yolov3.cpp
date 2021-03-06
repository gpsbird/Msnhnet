﻿#include <iostream>
#include "Msnhnet/net/MsnhNetBuilder.h"
#include "Msnhnet/io/MsnhIO.h"
#include "Msnhnet/config/MsnhnetCfg.h"
#include "Msnhnet/utils/MsnhOpencvUtil.h"

int main(int argc, char** argv) 
{
    if(argc != 2)
    {
        std::cout<<"\nYou need to give models dir path.\neg: yolov3 /your/models/dir/path/ \n\nModels folder must be like this:\nmodels\n  |-Lenet5\n    |-Lenet5.msnhnet\n    |-Lenet5.msnhbin";
        getchar();
        return 0;
    }

    std::string msnhnetPath = std::string(argv[1]) + "/yolov3/yolov3.msnhnet";
    std::string msnhbinPath = std::string(argv[1]) + "/yolov3/yolov3.msnhbin";
    std::string labelsPath  = "../labels/coco.names";
    std::string imgPath = "../images/dog.jpg";
    try
    {
        Msnhnet::NetBuilder  msnhNet;
        msnhNet.buildNetFromMsnhNet(msnhnetPath);
        std::cout<<msnhNet.getLayerDetail();
        msnhNet.loadWeightsFromMsnhBin(msnhbinPath);
        std::vector<std::string> labels ;
        Msnhnet::IO::readVectorStr(labels, labelsPath.data(), "\n");
        Msnhnet::Point2I inSize = msnhNet.getInputSize();
        std::vector<float> img = Msnhnet::OpencvUtil::getPaddingZeroF32C3(imgPath, cv::Size(inSize.x,inSize.y));
        cv::Mat org = cv::imread(imgPath);
        std::vector<std::vector<Msnhnet::Yolov3Box>> result = msnhNet.runYolov3(img);
        Msnhnet::OpencvUtil::drawYolov3Box(org,labels,result,inSize);
        std::cout<<msnhNet.getTimeDetail()<<std::endl<<std::flush;
        cv::imshow("test",org);
        cv::waitKey();
    }
    catch (Msnhnet::Exception ex)
    {
        std::cout<<ex.what()<<std::endl;
    }
    return 0;
}
