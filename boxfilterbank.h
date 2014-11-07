#ifndef BOXFILTERBANK_H
#define BOXFILTERBANK_H

#include "boxfilter.h"


class BoxFilterBank
{
public:
    BoxFilterBank(std::vector<BoxFilter> fb):
        _fb(fb)
    {
    }

    BoxFilterBank(int nFilt, std::vector<int> filtsize, cv::Range boxShapeRows, cv::Range boxShapeCols, cv::Range boxShapeDim, cv::Range nBox, int nDim, int rngSeed = 0);

    std::vector<cv::Mat> perform(std::vector<cv::Mat> in);

private:

    std::vector<BoxFilter> _fb;
};

#endif // BOXFILTERBANK_H
