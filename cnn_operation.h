#ifndef CNN_OPERATION_H
#define CNN_OPERATION_H

#include <opencv2/opencv.hpp>

class CNN_Operation
{
public:
    CNN_Operation();
    virtual std::vector<cv::Mat> perform(std::vector<cv::Mat> in) const = 0;
};

#endif // CNN_OPERATION_H
