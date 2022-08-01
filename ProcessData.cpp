#include "ProcessData.h"

vector<Point3d> triangulation(
    const vector<KeyPoint>& keypoint_1,
    const vector<KeyPoint>& keypoint_2,
    const std::vector<DMatch>& matches,
    const Mat& T1, const Mat& T2) {
    vector<Point3d> points;
    // Mat T1 = (Mat_<float>(3, 4) <<
    //   1, 0, 0, 0,
    //   0, 1, 0, 0,
    //   0, 0, 1, 0);
    // Mat T2 = (Mat_<float>(3, 4) <<
    //   R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), t.at<double>(0, 0),
    //   R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2), t.at<double>(1, 0),
    //   R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2), t.at<double>(2, 0)
    // );
    Mat K = (Mat_<double>(3, 3) << 744.375, 0, 426, 0, 744.375, 240, 0, 0, 1);
    vector<Point2f> pts_1, pts_2;
    for (DMatch m : matches) {
        pts_1.push_back(pixel2cam(keypoint_1[m.queryIdx].pt, K));
        pts_2.push_back(pixel2cam(keypoint_2[m.trainIdx].pt, K));
    }

    Mat pts_4d;
    cv::triangulatePoints(T1, T2, pts_1, pts_2, pts_4d);

    for (int i = 0; i < pts_4d.cols; i++) {
        Mat x = pts_4d.col(i);
        x /= x.at<float>(3, 0); 
        Point3d p(
            x.at<float>(0, 0),
            x.at<float>(1, 0),
            x.at<float>(2, 0)
        );
        points.push_back(p);
    }
    return points;
}

Point2f pixel2cam(const Point2d& p, const Mat& K) {
    return Point2f
    (
        (p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
        (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1)
    );
}

void get_pose(Mat& T1, Mat& T2, int first, int second, double GT[][7]) {
    Eigen::Quaterniond q_pose_0;
    Eigen::Quaterniond q_pose_1;
    q_pose_0.x() = GT[first][4];
    q_pose_0.y() = GT[first][5];
    q_pose_0.z() = GT[first][6];
    q_pose_0.w() = GT[first][3];
    q_pose_1.x() = GT[second][4];
    q_pose_1.y() = GT[second][5];
    q_pose_1.z() = GT[second][6];
    q_pose_1.w() = GT[second][3];
    Eigen::Matrix3d R_pose_0;
    Eigen::Matrix3d R_pose_1;
    R_pose_0 = q_pose_0.normalized().toRotationMatrix();
    R_pose_1 = q_pose_1.normalized().toRotationMatrix();
    // cout << "RPOSE:  " << R_pose_0(0,0) << "  " << R_pose_0(0,1)  << "   " << R_pose_0(0,2) << "   " << GT[first][0] <<endl;
    T1 = (cv::Mat_<float>(3, 4) <<
        R_pose_0(0, 0), R_pose_0(0, 1), R_pose_0(0, 2), GT[first][0],
        R_pose_0(1, 0), R_pose_0(1, 1), R_pose_0(1, 2), GT[first][1],
        R_pose_0(2, 0), R_pose_0(2, 1), R_pose_0(2, 2), GT[first][2]
        );
    // test
    // cout << "T1:  " << T1 << endl;
    T2 = (cv::Mat_<float>(3, 4) <<
        R_pose_1(0, 0), R_pose_1(0, 1), R_pose_1(0, 2), GT[second][0],
        R_pose_1(1, 0), R_pose_1(1, 1), R_pose_1(1, 2), GT[second][1],
        R_pose_1(2, 0), R_pose_1(2, 1), R_pose_1(2, 2), GT[second][2]
        );
}

void extractFeatures(const Mat& img,
    std::vector<KeyPoint>& keypoints,
    Mat& descriptors) {
    Ptr<FeatureDetector> detector = SiftFeatureDetector::create(0, 3, 0.04, 10);
    Ptr<DescriptorExtractor> descriptor = SiftDescriptorExtractor::create();
    // Ptr<FeatureDetector> detector = ORB::create();
    // Ptr<DescriptorExtractor> descriptor = ORB::create();
    detector->detect(img, keypoints);
    descriptor->compute(img, keypoints, descriptors);
}

void filterMatches(vector<DMatch>& matches,
    Mat descriptors_1,
    Mat descriptors_2,
    vector<KeyPoint> keypoints_1,
    vector<KeyPoint> keypoints_2) {
    double min_dist = 10000, max_dist = 0;
    vector<DMatch> goodMatches;
    for (int i = 0; i < descriptors_1.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    vector<Point2f> matchPoints1, matchPoints2;
    for (auto match : matches) {
        matchPoints1.push_back(keypoints_1[match.queryIdx].pt);
        matchPoints2.push_back(keypoints_2[match.trainIdx].pt);
    }
    Mat inlierMask;
    findEssentialMat(matchPoints1, matchPoints2,
        744.375,
        Point2d(426., 240.),
        RANSAC, 0.999, 1.0, inlierMask);

    // inlierMask.at<uint8_t>(i, 0)
    
    for (int i = 0; i < descriptors_1.rows; i++) {
        int isInLier = inlierMask.at<uint8_t>(i, 0);
        if ((matches[i].distance <= 4 * min_dist) && isInLier) {
            goodMatches.push_back(matches[i]);
        }
    }
    matches = goodMatches;
}








