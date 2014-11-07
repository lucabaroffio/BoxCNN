#ifndef OPERATIONPARAMETERS_H
#define OPERATIONPARAMETERS_H

#include <opencv2/opencv.hpp>

class OperationParameters
{
public:
    virtual void save(cv::FileStorage &f) = 0;
};

#endif // OPERATIONPARAMETERS_H
