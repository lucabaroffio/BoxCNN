#ifndef BOXFILTERPARAMETERS_H
#define BOXFILTERPARAMETERS_H

#include "operationparameters.h"
#include <limits>

class BoxFilterParameters : public OperationParameters
{
public:
    BoxFilterParameters(int nBoxes, std::vector<int> filterShape, int seed = 0, double minOut = -INFINITY, double maxOut = INFINITY);
    void save(cv::FileStorage &f);

private:
    int _nBoxes;
    std::vector<int> _filterShape;
    int _seed;
    double _minOut;
    double _maxOut;
};

#endif // BOXFILTERPARAMETERS_H
