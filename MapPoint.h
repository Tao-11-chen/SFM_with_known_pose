//
// Written by yuantao chen
//

#ifndef MAPPOINT_H
#define MAPPOINT_H

#include <opencv2/core/core.hpp>

using namespace std;
class MapPoint {
public:
    typedef shared_ptr<MapPoint> Ptr;
    cv::Mat pointDescriptor;        // Descriptor for matching
    cv::Point3d pos;       // Position in world
    vector<int> observationIndex;
    vector<cv::Point2f> points;
    MapPoint(const cv::Point3d pos, const cv::Mat descriptor) :
        pos(pos),
        pointDescriptor(descriptor) {}

    void addIndex(const int index, const cv::Point2f point) {
        observationIndex.push_back(index);
        points.push_back(point);
    }

};

#endif //MAPPOINT_H
#pragma once
