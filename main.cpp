//#include <QCoreApplication>
#include <opencv2/opencv.hpp>

#include "summedvolumetable.h"
#include "boxfilter.h"
#include "boxfilterbank.h"

#include <time.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <dirent.h>
#include <fstream>

bool GRAY = false;

using namespace std;

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error opening " << dir << endl;
        return -1;
    }

    int fcount = 0;
    while ((dirp = readdir(dp)) != NULL) {
        if( strcmp((dirp->d_name),".")!=0 && strcmp((dirp->d_name),"..")!=0 )
            files.push_back(string(dirp->d_name));
        fcount++;
    }
    closedir(dp);
    return 0;
}

int main(int argc, char *argv[])
{

    string out_path = "/Users/lucabaroffio/Desktop/CNN_data/";

    string img_path = "/Users/lucabaroffio/Documents/CNN/pubfig83-baseline-master/pubfig83-aligned/";
    vector<string> db_obj;

    if( getdir( img_path, db_obj )!=0 ){
        cout << "Error reading object files" << endl;
    }

    sort( db_obj.begin(), db_obj.end() );

    int n_objects = db_obj.size();

    vector<string> db_files;
    vector<string> obj_files;

    vector<string> out_labels;
    vector<string> out_filenames;

    cv::Mat out_feat;

    for( int i=0; i<n_objects; i++ ){
        obj_files.clear();

        string temp = img_path + db_obj[i] + "/";

        if( getdir( temp, obj_files )!=0 ){
            cout << "Error reading object files" << endl;
        }

        // sort( obj_files.begin(), obj_files.end() );

        for(unsigned int j=0; j<obj_files.size(); j++){

            string filename = img_path + db_obj[i] + "/" + obj_files[j];
            cout << filename << endl;

            out_labels.push_back(db_obj[i]);
            out_filenames.push_back(filename);

            // CNN

            cv::Mat img_gray = cv::imread(filename);

            if (!img_gray.rows > 0)
                continue;

//            cv::Mat img_gray = cv::imread("/Users/lucabaroffio/Downloads/pubfig83-aligned/Claudia Schiffer/19.jpg");
            //    cv::Mat img_gray = cv::imread("/Users/lucabaroffio/Desktop/lena.bmp");
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

            clock_t begin = clock();

            std::vector<cv::Mat> s = svt.compute(im_v);

            clock_t end = clock();

            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//            std::cout << "Summed volume table - elapsed time: " << elapsed_secs << " s" << std::endl;

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

            BoxFilterBank fb(32, filtsize, cv::Range(1, 5), cv::Range(1, 5), cv::Range(1, 1), cv::Range(15, 20), s.size(), std::time(NULL));

            begin = clock();

            out = fb.perform(s);

            end = clock();

            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//            std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;


            img_gray.convertTo(img_gray, CV_8U);
            imshow("ORIGINAL", img_gray);

            for (int i = 0; i < out.size(); i++){
                cv::Mat f_im = out[i];

                cv::resize(out[i], out[i], cv::Size(out[i].rows/2, out[i].cols/2));

                cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
                f_im.convertTo(f_im, CV_8U);
                cv::threshold(f_im, f_im, 100, 255, CV_THRESH_TOZERO);
                //        cv::threshold(f_im, f_im, 255, 255, CV_THRESH_TRUNC);

//                imshow("FILTERED", f_im);
//                cv::waitKey(100);
            }

            filtsize.clear();
            filtsize.push_back(3);
            filtsize.push_back(3);

            BoxFilterBank fb2(256, filtsize, cv::Range(1, 3), cv::Range(1, 3), cv::Range(4, 16), cv::Range(2, 5), out.size(), std::time(NULL));

            begin = clock();

            out = svt.compute(out);

            end = clock();

            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//            std::cout << "Summed volume table - elapsed time: " << elapsed_secs << " s" << std::endl;



            begin = clock();

            out = fb2.perform(out);

            end = clock();

            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//            std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;

            img_gray.convertTo(img_gray, CV_8U);
            imshow("ORIGINAL", img_gray);

            for (int i = 0; i < out.size(); i++){
                cv::Mat f_im = out[i];

                cv::resize(out[i], out[i], cv::Size(out[i].rows/2, out[i].cols/2));

                cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
                f_im.convertTo(f_im, CV_8U);
                cv::threshold(f_im, f_im, 100, 255, CV_THRESH_TOZERO);
                //        cv::threshold(f_im, f_im, 200, 255, CV_THRESH_TRUNC);

//                imshow("FILTERED", f_im);
//                //        cv::resize(out[i], out[i], cv::Size(out[i].rows/2, out[i].cols/2));
//                cv::waitKey(100);
            }


            filtsize.clear();
            filtsize.push_back(3);
            filtsize.push_back(3);

            BoxFilterBank fb3(128, filtsize, cv::Range(1, 3), cv::Range(1, 3), cv::Range(64, 128), cv::Range(2, 5), out.size(), std::time(NULL));

            out = svt.compute(out);

            begin = clock();

            out = fb3.perform(out);

            end = clock();

            elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//            std::cout << "elapsed time: " << elapsed_secs << " s" << std::endl;

            img_gray.convertTo(img_gray, CV_8U);
//            imshow("ORIGINAL", img_gray);

            cv::Mat temp;

            for (int i = 0; i < out.size(); i++){
                cv::Mat f_im = out[i];

                cv::normalize(f_im, f_im, 0, 255, cv::NORM_MINMAX, CV_64F);
                f_im.convertTo(f_im, CV_8U);
                cv::threshold(f_im, f_im, 100, 255, CV_THRESH_TOZERO);
                //        cv::threshold(f_im, f_im, 200, 255, CV_THRESH_TRUNC);

//                imshow("FILTERED", f_im);
//                //        cv::resize(out[i], out[i], cv::Size(out[i].rows/2, out[i].cols/2));
//                cv::waitKey(100);

                temp.push_back(f_im);
            }

            temp = temp.reshape(0, 1);
            out_feat.push_back(temp);


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

    }

    stringstream ss;
    ss << out_path << "DB.xml";

    cv::FileStorage fs;
    fs.open(ss.str(), cv::FileStorage::WRITE);

    cv::write(fs, "features", out_feat);
    cv::write(fs, "labels", out_labels);
    cv::write(fs, "filenames", out_filenames);

}
