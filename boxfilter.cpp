#include "boxfilter.h"


std::vector<Box> BoxFilter::boxes() const
{
    return _boxes;
}

void BoxFilter::setBoxes(const std::vector<Box> &boxes)
{
    _boxes = boxes;
}

cv::Mat BoxFilter::perform(std::vector<cv::Mat> in_d){

    assert(in_d.size() > 0);
    assert(in_d[0].type() == CV_64F);

    int R = in_d[0].rows;
    int C = in_d[0].cols;
    int Z = in_d.size();

    cv::Mat out = cv::Mat::zeros(R, C, CV_64F);

    for (int b = 0; b < _boxes.size(); b++){

        int l_start = 0;    // near layer = 0
        int l_end = Z - 1;  // far layer = last

        // Near layers

        cv::Mat ntl(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
        cv::Mat ntr(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
        cv::Mat nbl(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
        cv::Mat nbr(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));

        cv::Range rs[2];

        if (l_start > 0){

            rs[0] = cv::Range(_boxes[b].tl().y, R - _shape[0] + _boxes[b].tl().y);
            rs[1] = cv::Range(_boxes[b].tl().x, C - _shape[1] + _boxes[b].tl().x);

            ntl = cv::Mat(in_d[l_start-1], rs); // ntl: Near-Top-Left layer, -1 because near layer of box filtering process

            rs[0] = cv::Range(_boxes[b].tl().y, R - _shape[0] + _boxes[b].tl().y);
            rs[1] = cv::Range(_boxes[b].br().x, C - _shape[1] + _boxes[b].br().x);

            ntr = cv::Mat(in_d[l_start-1], rs); // ntr: Near-Top-Right layer

            rs[0] = cv::Range(_boxes[b].br().y, R - _shape[0] + _boxes[b].br().y);
            rs[1] = cv::Range(_boxes[b].tl().x, C - _shape[1] + _boxes[b].tl().x);

            nbl = cv::Mat(in_d[l_start-1], rs); // nbl: Near-Bottom-Left layer

            rs[0] = cv::Range(_boxes[b].br().y, R - _shape[0] + _boxes[b].br().y);
            rs[1] = cv::Range(_boxes[b].br().x, C - _shape[1] + _boxes[b].br().x);

            nbr = cv::Mat(in_d[l_start-1], rs);   // nbr: Near-Bottom-Right layer
        }
        else{
            ntl = cv::Mat(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
            ntr = cv::Mat(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
            nbl = cv::Mat(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
            nbr = cv::Mat(R - _shape[0], C - _shape[1], CV_64F, cv::Scalar::all(0.0));
        }

        // Far layers

        rs[0] = cv::Range(_boxes[b].tl().y, R - _shape[0] + _boxes[b].tl().y);
        rs[1] = cv::Range(_boxes[b].tl().x, C - _shape[1] + _boxes[b].tl().x);

        cv::Mat ftl = cv::Mat(in_d[l_end], rs);   // ftl: Far-Top-Left layer

        rs[0] = cv::Range(_boxes[b].tl().y, R - _shape[0] + _boxes[b].tl().y);
        rs[1] = cv::Range(_boxes[b].br().x, C - _shape[1] + _boxes[b].br().x);

        cv::Mat ftr = cv::Mat(in_d[l_end], rs);   // ftr: Far-Top-Right layer

        rs[0] = cv::Range(_boxes[b].br().y, R - _shape[0] + _boxes[b].br().y);
        rs[1] = cv::Range(_boxes[b].tl().x, C - _shape[1] + _boxes[b].tl().x);
        rs[2] = cv::Range(l_end, l_end + 1);

        cv::Mat fbl = cv::Mat(in_d[l_end], rs);    // fbl: Far-Bottom-Left layer

        rs[0] = cv::Range(_boxes[b].br().y, R - _shape[0] + _boxes[b].br().y);
        rs[1] = cv::Range(_boxes[b].br().x, C - _shape[1] + _boxes[b].br().x);
        rs[2] = cv::Range(l_end, l_end + 1);

        cv::Mat fbr = cv::Mat(in_d[l_end], rs);    // fbr: Far-Bottom-Right layer

        // apply zero padding
        int pad_left = ceil((double) _shape[1] / 2.0);
        int pad_top = ceil((double) _shape[0] / 2.0);
        int pad_right = _shape[1]/2;
        int pad_bottom = _shape[0]/2;

        cv::Mat ntl_p(R, C, CV_64F);    // ntl: Near-Top-Left layer
        cv::copyMakeBorder(ntl, ntl_p, pad_top + 1, pad_bottom - 1,
                       pad_left + 1, pad_right - 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat ntr_p(R, C, CV_64F);    // ntr: Near-Top-Right layer
        cv::copyMakeBorder(ntr, ntr_p, pad_top + 1, pad_bottom - 1,
                       pad_left, pad_right, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat nbl_p(R, C, CV_64F);    // nbl: Near-Bottom-Left layer
        cv::copyMakeBorder(nbl, nbl_p, pad_top, pad_bottom,
                       pad_left + 1, pad_right - 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat nbr_p(R, C, CV_64F);    // nbr: Near-Bottom-Right layer
        cv::copyMakeBorder(nbr, nbr_p, pad_top, pad_bottom,
                       pad_left, pad_right, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat ftl_p(R, C, CV_64F);    // ftl: Far-Top-Left layer
        cv::copyMakeBorder(ftl, ftl_p, pad_top + 1, pad_bottom - 1,
                       pad_left + 1, pad_right - 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat ftr_p(R, C, CV_64F);    // ftr: Far-Top-Right layer
        cv::copyMakeBorder(ftr, ftr_p, pad_top + 1, pad_bottom - 1,
                       pad_left, pad_right, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat fbl_p(R, C, CV_64F);    // fbl: Far-Bottom-Left layer
        cv::copyMakeBorder(fbl, fbl_p, pad_top, pad_bottom,
                       pad_left + 1, pad_right - 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat fbr_p(R, C, CV_64F);    // fbr: Far-Bottom-Right layer
        cv::copyMakeBorder(fbr, fbr_p, pad_top, pad_bottom,
                       pad_left, pad_right, cv::BORDER_CONSTANT, cv::Scalar::all(0));


        // obtain filtered image with a weighted sum of layers
        // equation for box filtering
        out += _boxes[b].v() * (fbr_p - nbr_p - ftr_p - fbl_p + ftl_p + nbl_p + ntr_p - ntl_p);

    }

//    cv::Range ran[2];
//    ran[0] = cv::Range(_shape[0], out.rows - shape[0]);
//    ran[1] = cv::Range(_shape[1], out.cols - shape[1]);
    return out(cv::Range(_shape[0], out.rows - _shape[0]), cv::Range(_shape[1], out.cols - _shape[1]));
}
std::vector<int> BoxFilter::shape() const
{
    return _shape;
}

void BoxFilter::setShape(const std::vector<int> &shape)
{
    _shape = shape;
}


cv::Point3f Box::tl() const
{
    return _tl;
}

void Box::setTl(const cv::Point3f &tl)
{
    _tl = tl;
}
cv::Point3f Box::br() const
{
    return _br;
}

void Box::setBr(const cv::Point3f &br)
{
    _br = br;
}
float Box::v() const
{
    return _v;
}

void Box::setV(float v)
{
    _v = v;
}


