#ifndef BOXFILTER_H
#define BOXFILTER_H

#include <opencv2/opencv.hpp>
#include <cnn_operation.h>

class Box
{
public:

    Box(cv::Point3f tl, cv::Point3f br, float v):
        _tl(tl),
        _br(br),
        _v(v)
    {
    }

    cv::Point3f tl() const;
    void setTl(const cv::Point3f &tl);

    cv::Point3f br() const;
    void setBr(const cv::Point3f &br);

    float v() const;
    void setV(float v);

private:

    cv::Point3f _tl;
    cv::Point3f _br;
    float _v;
};


class BoxFilter
{
public:
    BoxFilter(std::vector<Box> boxes, std::vector<int> shape):
        _boxes(boxes),
        _shape(shape)
    {
    }

    std::vector<Box> boxes() const;
    void setBoxes(const std::vector<Box> &boxes);

    std::vector<int> shape() const;
    void setShape(const std::vector<int> &shape);

    cv::Mat perform(std::vector<cv::Mat>);

private:
    std::vector<Box> _boxes;
    std::vector<int> _shape;
};

#endif // BOXFILTER_H
