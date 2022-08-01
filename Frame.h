//
// Written by yuantao chen
//

#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>

using namespace std;
class Frame {
public:
    typedef shared_ptr<Frame> Ptr;
    int frame_index;
    vector<cv::KeyPoint> keypoints;
    cv::Mat Descriptors;
    Frame();
    Frame(vector<cv::KeyPoint> keypoint, cv::Mat descriptor, int frame_index) :
        keypoints(keypoint),
        Descriptors(descriptor),
        frame_index(frame_index) {}

    void test() {
        cout << "keypoints: " << keypoints.size() << endl;
    }
};

#endif //FRAME_H
