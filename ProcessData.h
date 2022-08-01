//
// Written by yuantao chen
//
#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "MapPoint.h"
#include "Map.h"
#include "Frame.h"
#include <opencv2/core/eigen.hpp>

using namespace std;
using namespace cv;


void extractFeatures(const Mat& img,
	std::vector<KeyPoint>& keypoints,
	Mat& descriptors
);

void filterMatches(vector<DMatch>& matches,
	Mat descriptors_1,
	Mat descriptors_2,
	vector<KeyPoint> keypoints_1,
	vector<KeyPoint> keypoints_2
);

vector<Point3d> triangulation(
	const vector<KeyPoint>& keypoint_1,
	const vector<KeyPoint>& keypoint_2,
	const std::vector<DMatch>& matches,
	const Mat& T1, const Mat& T2
);

void get_pose(Mat& T1, Mat& T2, int first, int second, double GT[][7]);

Point2f pixel2cam(const Point2d& p, const Mat& K);

#endif 
