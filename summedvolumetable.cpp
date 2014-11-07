#include "summedvolumetable.h"

SummedVolumeTable::SummedVolumeTable()
{
}

std::vector<cv::Mat> SummedVolumeTable::compute(std::vector<cv::Mat> im)
{

    assert(im.size() > 0);
    assert(im[0].type() == CV_64F);

    int R = im[0].rows;
    int C = im[0].cols;
    int P = im.size();

    // initialization
    std::vector<cv::Mat> out;
    for (int z = 0; z < P; z++){
        out.push_back(cv::Mat::zeros(R, C, CV_64F));
    }

    // case i, j, k = 0
    int i = 0, j = 0, k = 0;

    out[k].at<double>(i, j) = im[k].at<double>(i, j);

    // case i, j = 0
    i = 0, j = 0;

    for (int k = 1; k < P; k++){
        double v = 0;
        v += im[k].at<double>(i, j);
        v += out[k-1].at<double>(i, j);
        out[k].at<double>(i, j) = v;
    }

    // case i, k = 0
    i = 0, k = 0;

    for (int j = 1; j < C; j++){
        double v = 0;
        v += im[k].at<double>(i, j);
        v += out[k].at<double>(i, j-1);
        out[k].at<double>(i, j) = v;
    }

    // case j, k = 0
    j = 0, k = 0;

    for (int i = 1; i < R; i++){
        double v = 0;
        v += im[k].at<double>(i, j);
        v += out[k].at<double>(i-1, j);
        out[k].at<double>(i, j) = v;
    }

    // case i = 0
    i = 0;

    for (int j = 1; j < C; j++){
        for (int k = 1; k < P; k++){

            double v = 0;
            v += im[k].at<double>(i, j);
            v += out[k-1].at<double>(i, j);
            v += out[k].at<double>(i, j-1);
            v -= out[k-1].at<double>(i, j-1);
            out[k].at<double>(i, j) = v;

        }
    }

    // case j = 0
    j = 0;

    for (int i = 1; i < R; i++){
        for (int k = 1; k < P; k++){

            double v = 0;
            v += im[k].at<double>(i, j);
            v += out[k-1].at<double>(i, j);
            v += out[k].at<double>(i-1, j);
            v -= out[k-1].at<double>(i-1, j);
            out[k].at<double>(i, j) = v;

        }
    }

    // case k = 0
    k = 0;

    for (int i = 1; i < R; i++){
        for (int j = 1; j < C; j++){

            double v = 0;
            v += im[k].at<double>(i, j);
            v += out[k].at<double>(i-1, j);
            v += out[k].at<double>(i, j-1);
            v -= out[k].at<double>(i-1, j-1);
            out[k].at<double>(i, j) = v;

        }
    }

    // case i, j, k > 0

    for (int i = 1; i < R; i++){
        for (int j = 1; j < C; j++){
            for (int k = 1; k < P; k++){

                double v = 0;
                v += im[k].at<double>(i, j);
                v += out[k-1].at<double>(i, j);
                v += out[k].at<double>(i-1, j);
                v -= out[k-1].at<double>(i-1, j);
                v += out[k].at<double>(i, j-1);
                v -= out[k-1].at<double>(i, j-1);
                v -= out[k].at<double>(i-1, j-1);
                v += out[k-1].at<double>(i-1, j-1);
                out[k].at<double>(i, j) = v;

            }
        }
    }

    return out;

}
