#include "boxfilterparameters.h"

BoxFilterParameters::BoxFilterParameters(int nBoxes, std::vector<int> filterShape, int seed, double minOut, double maxOut):
    _nBoxes(nBoxes),
    _filterShape(filterShape),
    _seed(seed),
    _minOut(minOut),
    _maxOut(maxOut)
{
}

void BoxFilterParameters::save(cv::FileStorage &f)
{

}
