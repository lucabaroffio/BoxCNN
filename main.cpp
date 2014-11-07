//#include <QCoreApplication>
#include <opencv2/opencv.hpp>

#include "summedvolumetable.h"
#include "boxfilter.h"
#include "boxfilterbank.h"

#include <time.h>

bool GRAY = false;

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

//    return a.exec();

//    cv::Mat img_gray = cv::imread("/Users/lucabaroffio/Downloads/pubfig83-aligned/Ben Affleck/5.jpg");
    cv::Mat img_gray = cv::imread("/Users/lucabaroffio/Desktop/lena.bmp");
//    cv::resize(img_gray, img_gray, cv::Size(300, 300));

    cv::Range ran[2];

//    ran[0] = cv::Range(200, 210);
//    ran[1] = cv::Range(200, 210);
//    img_gray = cv::Mat(img_gray, ran);

//    std::cout << img_gray << std::endl;
    img_gray.convertTo(img_gray, CV_64F);

    std::vector<cv::Mat> im_v;

    std::vector<cv::Mat> channels(3);
    // split img:
    cv::split(img_gray, channels);

    if (!GRAY){
        for (int i = 0; i < 3; i++){
            im_v.push_back(channels[i]);
        }
    }
    else{
        cv::cvtColor(img_gray, img_gray, CV_RGB2GRAY);
        im_v.push_back(img_gray);
    }



    int sz[3];
    sz[0] = 3;
    sz[1] = 3;
    sz[2] = 128;
    cv::Mat a(3, sz, CV_64F, cv::Scalar::all(1));

//    for (int i = 0; i < 3; i++){
//         for (int j = 0; j < 3; j++){
//              for (int k = 0; k < 3; k++){
//                    std::cout << a.at<double>(i, j, k) << std::endl;
//              }
//         }
//    }


    SummedVolumeTable svt;
    std::vector<cv::Mat> s = svt.compute(im_v);

//    std::cout << s.size() << std::endl;

//    std::cout << im_v[0] << std::endl << std::endl;

//    for (int i = 0; i < s.size(); i++){
//        std::cout << s[i] << std::endl << std::endl << std::endl;
//    }


//    std::cout << lay << std::endl;

//    for (int j = 0; j < 3; j++){
//        for (int k = 0; k < 3; k++){
//            std::cout << lay.at<double>(j, k, 0) << std::endl;
//        }
//    }




//    std::vector<int> shape(3);
//    shape[0] = 15;
//    shape[1] = 15;
//    shape[2] = 1;

//    std::vector<Box> bs;

//    int nbox = 5;
//    int bshape = 3;

//    cv::RNG rng(std::time(NULL));

//    double sum_v = 0.0;

//    for (int b = 0; b < nbox; b++){

//        int r = rng.uniform((int) 0, (int) shape[0] - bshape - 1);
//        int c = rng.uniform((int) 0, (int) shape[0] - bshape - 1);
//        double v = rng.uniform(-2.0, 2.0);
//        sum_v += v*bshape*bshape;
//        bs.push_back(Box(cv::Point3f(r, c, 0), cv::Point3f(r + bshape, c + bshape, 0), v));
//    }

//    for (int b = 0; b < nbox; b++){

//        bs[b].setV(bs[b].v()/sum_v);
//        std::cout << bs[b].v() << std::endl;
//    }

//    Box b1(cv::Point3f(0, 0, 0), cv::Point3f(5, 5, 0), +1.0/(6.0*6.0));
//    bs.push_back(b1);

//    Box b2(cv::Point3f(2, 2, 0), cv::Point3f(3, 3, 0), -2.0/(6.0*6.0));
//    bs.push_back(b2);

//    BoxFilter bf(bs, shape);
//    cv::Mat f_im = bf.perform(s);

//    std::cout << f_im << std::endl;

    std::vector<cv::Mat> out;
    std::vector<int> filtsize;
    filtsize.push_back(5);
    filtsize.push_back(5);

    BoxFilterBank fb(64, filtsize, cv::Range(2, 4), cv::Range(2, 4), cv::Range(1, 2), cv::Range(5, 10), s.size(), std::time(NULL));

    clock_t begin = clock();

    out = fb.perform(s);

    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;


    img_gray.convertTo(img_gray, CV_8U);
    imshow("ORIGINAL", img_gray);

    for (int i = 0; i < out.size(); i++){
        cv::Mat f_im = out[i];

        cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
        f_im.convertTo(f_im, CV_8U);
        cv::threshold(f_im, f_im, 100, 255, CV_THRESH_TOZERO);
        cv::threshold(f_im, f_im, 200, 255, CV_THRESH_TRUNC);
        imshow("FILTERED", f_im);
        cv::waitKey(100);
    }

    BoxFilterBank fb2(256, filtsize, cv::Range(2, 4), cv::Range(2, 4), cv::Range(50, 100), cv::Range(5, 10), out.size(), std::time(NULL));

    begin = clock();

    out = fb2.perform(out);

    end = clock();

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;

    img_gray.convertTo(img_gray, CV_8U);
    imshow("ORIGINAL", img_gray);

    for (int i = 0; i < out.size(); i++){
        cv::Mat f_im = out[i];

        cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
        f_im.convertTo(f_im, CV_8U);
        cv::threshold(f_im, f_im, 100, 255, CV_THRESH_TOZERO);
        cv::threshold(f_im, f_im, 200, 255, CV_THRESH_TRUNC);

        imshow("FILTERED", f_im);
//        cv::resize(out[i], out[i], cv::Size(out[i].rows/2, out[i].cols/2));
        cv::waitKey(100);
    }


//    BoxFilterBank fb3(128, filtsize, cv::Range(2, 4), cv::Range(2, 4), cv::Range(10, 20), cv::Range(5, 10), out.size(), std::time(NULL));

//    begin = clock();

//    out = fb3.perform(out);

//    end = clock();

//    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//    std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;

//    img_gray.convertTo(img_gray, CV_8U);
//    imshow("ORIGINAL", img_gray);

//    for (int i = 0; i < out.size(); i++){
//        cv::Mat f_im = out[i];

//        cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
//        f_im.convertTo(f_im, CV_8U);
//        cv::threshold(f_im, f_im, 120, 255, CV_THRESH_TOZERO);

//        imshow("FILTERED", f_im);
//        cv::waitKey(100);
//    }

//    BoxFilterBank fb4(64, filtsize, cv::Range(2, 4), cv::Range(2, 4), cv::Range(10, 20), cv::Range(5, 10), out.size(), std::time(NULL));

//    begin = clock();

//    out = fb4.perform(out);

//    end = clock();

//    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//    std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;

//    img_gray.convertTo(img_gray, CV_8U);
//    imshow("ORIGINAL", img_gray);

//    for (int i = 0; i < out.size(); i++){
//        cv::Mat f_im = out[i];

//        cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
//        f_im.convertTo(f_im, CV_8U);
//        cv::threshold(f_im, f_im, 120, 255, CV_THRESH_TOZERO);

//        imshow("FILTERED", f_im);
//        cv::waitKey(100);
//    }


}
