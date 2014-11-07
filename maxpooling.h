#ifndef MAXPOOLING_H
#define MAXPOOLING_H

#include <opencv2/opencv.hpp>

class MaxPooling
{
public:
    MaxPooling(int rSize, int cSize, int lSize, int stride):
        _rSize(rSize),
        _cSize(cSize),
        _lSize(lSize),
        _stride(stride)
    {
    }

    std::vector<cv::Mat> perform(std::vector<cv::Mat> in);

private:

    int _rSize;
    int _cSize;
    int _lSize;
    int _stride;
};

#endif // MAXPOOLING_H
