// testclassifier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "libclassifier.h"

using namespace cv;
using namespace std;

string dir = "";// = "H:\\open-dnn-master\\bin\\x64\\Release\\";
string model = "deploy.prototxt";
string trained = "snapshot_iter_51100.caffemodel";
string mean_fn = "mean.binaryproto";
string label = "labels.txt";
string img_ = "H:\\Samples\\Remotes\\Real\\aircon-tornado-legend-real.png";
string model_file = dir + model;
string trained_file = dir + trained;
string mean_file = dir + mean_fn;
string label_file = dir + label;
string file = img_;

int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("H:\\Samples\\Remotes\\Real\\lg-nb4543-real.png");
	InitializeClassifier(model_file, trained_file, mean_file, label_file);
	vector<string> result = recognizeImage(img.data,img.cols,img.rows);
	imshow(result[0].c_str(),img);
	waitKey(10000);
	return 0;
}

