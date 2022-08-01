//
// Written by yuantao chen
//

#ifndef MAP_H
#define MAP_H

#include <opencv2/core/core.hpp>
#include <list>
#include "MapPoint.h"
#include "Frame.h"

using namespace std;
class MyMap {
public:
    typedef shared_ptr<MyMap> Ptr;
    deque<MapPoint::Ptr> mapPoints;
    vector<Frame::Ptr> Frames;
    MyMap() {}

    void addPoint(MapPoint::Ptr mapPoint) {
        mapPoints.push_back(mapPoint);
    }

    void getDescriptors(cv::Mat &mapDescriptors) {
        cv::Mat descriptorsMap;
        for (MapPoint::Ptr &point : mapPoints) {
            descriptorsMap.push_back(point->pointDescriptor);
        }
        mapDescriptors = descriptorsMap;
    }

    vector<cv::KeyPoint> getKeyPoints() {
        vector<cv::KeyPoint> keypoints;
        for (auto point : mapPoints) {
            cv::KeyPoint keypoint;
            keypoint.pt = point.get()->points[point.get()->points.size() - 1];
            keypoints.push_back(keypoint);
        }
        return keypoints;
    }

    void addFrame(Frame::Ptr frame) {
        Frames.push_back(frame);
    }

    vector<Frame::Ptr> getFrame() {
        return Frames;
    }

    deque<MapPoint::Ptr> getmapPoints() {
        return mapPoints;
    }
};


#endif //MAP_H
#pragma once
