#ifndef SUMMEDVOLUMETABLE_H
#define SUMMEDVOLUMETABLE_H

#include <opencv2/opencv.hpp>

class SummedVolumeTable
{
public:
    SummedVolumeTable();
    std::vector<cv::Mat> compute(std::vector<cv::Mat> im);

};

#endif // SUMMEDVOLUMETABLE_H
