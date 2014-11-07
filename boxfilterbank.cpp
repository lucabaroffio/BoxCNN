#include "boxfilterbank.h"



BoxFilterBank::BoxFilterBank(int nFilt, std::vector<int> filtsize, cv::Range boxShapeRows, cv::Range boxShapeCols, cv::Range boxShapeDim, cv::Range nBox, int nDim, int rngSeed)
{
    cv::RNG rng(rngSeed);

    for (int f = 0; f < nFilt; f++){

        int n = rng.uniform(nBox.start, nBox.end);

        std::vector<Box> boxes;

        double sum_v = 0.0;

        int centerDim = ((double) f / (double) nFilt) * nDim;

        for (int b = 0; b < n; b++){
            int size_r = rng.uniform(boxShapeRows.start, std::min(boxShapeRows.end, filtsize[0]));
            int start_r = rng.uniform(0, filtsize[0] - size_r);
            int end_r = start_r + size_r;

            int size_c = rng.uniform(boxShapeCols.start, std::min(boxShapeCols.end, filtsize[1]));
            int start_c = rng.uniform(0, filtsize[1] - size_c);
            int end_c = start_c + size_c;

            int size_d = rng.uniform(boxShapeDim.start, std::min(boxShapeDim.end, nDim));
            int start_d = std::max(0, centerDim - size_d/2);
            int end_d = std::min(nDim, centerDim + size_d/2);

            double v = rng.uniform(-1.0, 1.0);
            sum_v += v*size_r*size_c*(end_d - start_d);

            boxes.push_back(Box(cv::Point3f(start_c, start_r, start_d), cv::Point3f(end_c, end_r, end_d), v));
        }

//        for (int b = 0; b < n; b++){
//            boxes[b].setV(boxes[b].v()/sum_v);
//        }

        _fb.push_back(BoxFilter(boxes, filtsize));
    }
}

std::vector<cv::Mat> BoxFilterBank::perform(std::vector<cv::Mat> in)
{
    std::vector<cv::Mat> out;
    for (int filt = 0; filt < _fb.size(); filt++){
        out.push_back(_fb[filt].perform(in));
    }
    return out;
}
