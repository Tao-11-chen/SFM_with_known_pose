#include "ProcessData.h"

using namespace cv;
using namespace std;

// const int num_data = 2;
// string path = "/home/chen/data/Example_Data/";
// const int num_data = 2;
// string path = "/home/chen/data/seq-02/";
const int num_data = 100;
string path = "/home/chen/Downloads/rgbd_dataset_freiburg1_plant/rgb/";
ofstream OutFile("out.txt");
ofstream OutPly("map.ply");
ifstream fin;
double GT[num_data][7];
double obs[num_data][7];
string image_paths[num_data];


int main(int argc, char** argv) {
    // Cambridge10 read data
    // for (int i = 0; i < num_data; i++) {
    //     string txt_path;
    //     string image_path;
    //     if ((i + 1) < 10) {
    //         txt_path = path + "frame0000" + to_string(i + 1) + ".pose.txt";
    //         image_path = path + "frame0000" + to_string(i + 1) + ".png";
    //     }
    //     else if ((i + 1) < 100) {
    //         txt_path = path + "frame000" + to_string(i + 1) + ".pose.txt";
    //         image_path = path + "frame000" + to_string(i + 1) + ".png";
    //     }
    //     else if ((i + 1) < 1000) {
    //         txt_path = path + "frame00" + to_string(i + 1) + ".pose.txt";
    //         image_path = path + "frame00" + to_string(i + 1) + ".png";
    //     }
    //     else {
    //         txt_path = path + "frame0" + to_string(i + 1) + ".pose.txt";
    //         image_path = path + "frame0" + to_string(i + 1) + ".png";
    //     }
    //     image_paths[i] = image_path;
    //     fin.open(txt_path, ios::in);
    //     char buf[1021] = { 0 };
    //     int txt_i = 0;
    //     while (fin.getline(buf, sizeof(buf))) {
    //         const char* d = " ,";
    //         char* p = strtok(buf, d);
    //         while (p) {
    //             if (txt_i < 7)
    //                 GT[i][txt_i] = atof(p);
    //             else
    //                 obs[i][txt_i - 7] = atof(p);
    //             p = strtok(NULL, " ,");
    //             txt_i++;
    //         }
    //     }
    //     fin.close();
    //     fin.clear(ios::goodbit);
    // }



    //7scenes
      // for (int i = 0;i < num_data;i++){
      //   string image_path;
      //   if ( i < 10){
      //     image_path = path + "frame-00000" + to_string(i) + ".color.png";
      //   }else if ( i < 100){
      //     image_path = path + "frame-0000" + to_string(i) + ".color.png";
      //   }else if ( i < 1000){
      //     image_path = path + "frame-000" + to_string(i) + ".color.png";
      //   }else{
      //     image_path = path + "frame-00" + to_string(i) + ".color.png";
      //   }
      //   image_paths[i] = image_path;
      // }
      // fin.open("/home/chen/data/office-seq2.csv");
      // string line;
      // int index = 0;
      // while (getline(fin,line) && index < num_data){ 
      //   vector<float> data_line;
      //   string number;
      //   istringstream readstr(line); 
      //   for(int j = 0;j < 8;j++){ 
      //       getline(readstr,number,',');
      //       // cout << atof(number.c_str()) << endl;
      //       data_line.push_back(atof(number.c_str())); 
      //   }
      //   for (int a = 0; a < 7; a++){
      //     GT[index][a] = data_line[a];
      //   }
      //   index++;
      // }
      // fin.close();
      // fin.clear(ios::goodbit);
      // fin.open("/home/chen/data/office-seq2-res.csv");
      // index = 0;
      // while (getline(fin,line) && index < num_data){ 
      //   vector<float> data_line;
      //   string number;
      //   istringstream readstr(line); 
      //   for(int j = 0;j < 8;j++){ 
      //       getline(readstr,number,',');
      //       data_line.push_back(atof(number.c_str())); 
      //   }
      //   for (int a = 0; a < 7; a++){
      //     obs[index][a] = data_line[a];
      //   }
      //   index++;
      // }
      // fin.close();
      // fin.clear(ios::goodbit);
    


    for (int i = 0; i < num_data; i++) {
        string image_path;
        if ((i + 1) < 10) {
            image_path = path + to_string(i + 1) + ".png";
        }
        else if ((i + 1) < 100) {
            image_path = path + to_string(i + 1) + ".png";
        }
        else if ((i + 1) < 1000) {
            image_path = path + to_string(i + 1) + ".png";
        }
        else {
            image_path = path + to_string(i + 1) + ".png";
        }
        image_paths[i] = image_path;
    }
    string txt_path = "/home/chen/Downloads/rgbd_dataset_freiburg1_plant/groundtruth.txt";
    fin.open(txt_path, ios::in);
    char buf[1021] = { 0 };
    int lines = 0;
    while (fin.getline(buf, sizeof(buf))) {
      char* p = strtok(buf, " ");
      int txt_i = 0;
      while (p) {
        if (txt_i > 0 && lines < num_data){
          GT[lines][txt_i - 1] = atof(p);
          obs[lines][txt_i - 1] = atof(p);
        }
        p = strtok(NULL, " ");
        txt_i++;
      }
      lines++;
    }
    fin.close();
    fin.clear(ios::goodbit);

    cout << "Data reading finished" << endl;

    // use two frames to init map
    Mat img_1 = imread(image_paths[0], cv::IMREAD_COLOR);
    Mat img_2 = imread(image_paths[1], cv::IMREAD_COLOR);
    // extract features
    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat Descriptor_1, Descriptor_2;
    extractFeatures(img_1, keypoints_1, Descriptor_1);
    extractFeatures(img_2, keypoints_2, Descriptor_2);
    //test features
      // cv::Mat outimg1;
      // drawKeypoints(img_1, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
      // imshow("ORB features", outimg1);
      // waitKey(0);

    // match img 0 and 1
    vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
    matcher->match(Descriptor_1, Descriptor_2, matches);
    filterMatches(matches, Descriptor_1, Descriptor_2, keypoints_1, keypoints_2);
    cout << "find:" << matches.size() << " pairs after filter while init using frame 1 and 2 " << endl;

// test matching
    // Mat img_match;
    // drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_match);
    // imshow("all matches", img_match);
    // waitKey(0);

    // trangulation using R,t
    Mat T1, T2;
    get_pose(T1, T2, 0, 1, GT);
    vector<Point3d> points = triangulation(keypoints_1, keypoints_2, matches, T1, T2);

    // test trangulation
    // test(matches, keypoints_1, keypoints_2, R, t, points);

    // init map
    MyMap::Ptr map(new MyMap());
    for (int i = 0; i < points.size(); ++i) {
        MapPoint::Ptr newpoint(new MapPoint(points[i], Descriptor_1.row(matches[i].queryIdx))); //! Descriptor_2.row(matches[i].trainIdx) same exists
        newpoint->addIndex(0, keypoints_1[matches[i].queryIdx].pt);  // push camera index and 2dpoints
        newpoint->addIndex(1, keypoints_2[matches[i].trainIdx].pt);
        map->addPoint(newpoint);
    }
    Frame::Ptr newFrame1(new Frame(keypoints_1, Descriptor_1, 0));
    Frame::Ptr newFrame2(new Frame(keypoints_2, Descriptor_2, 1));
    // test
    // newFrame1->test();
    map->addFrame(newFrame1);
    map->addFrame(newFrame2);
    cout << "map init finished" << endl;


    // update frames
    for (int i = 2; i < num_data; i++) {
        cout << "total " << map.get()->Frames.size() << " frames in map" << endl;
        cout << "total " << map.get()->mapPoints.size() << " points in map" << endl;
        cout << "loading new image: " << i << endl;
        // load new img and extract features
        Mat img = imread(image_paths[i], cv::IMREAD_COLOR);
        vector<KeyPoint> keypoints;
        Mat Descriptors;
        extractFeatures(img, keypoints, Descriptors);
        // match map and new frame
        Mat MapDescriptors;
        map->getDescriptors(MapDescriptors);
        vector<DMatch> map_matches;
        matcher->match(MapDescriptors, Descriptors, map_matches);
        filterMatches(map_matches, MapDescriptors, Descriptors, map->getKeyPoints(), keypoints);
        cout << "find:" << map_matches.size() << " pairs between frame " << i << " and map" << endl;
        // write down in map or not 
        vector<int> isInMap;
        for (int j = 0; j < map_matches.size(); j++) {
            map->mapPoints[map_matches[j].queryIdx]->addIndex(i, keypoints[map_matches[j].trainIdx].pt);
            // use mapDescriptors's index to push camera index and 2dpoints
            map->mapPoints[map_matches[j].queryIdx]->pointDescriptor = Descriptors.row(map_matches[j].trainIdx);
            // update descriptors
            isInMap.push_back(map_matches[j].trainIdx);  // save frameDescriptor's index
        }
        // cout << isInMap.size() << "points in map: "  << endl;

    // match between map's previous frames one by one
        for (auto frame : map->getFrame()) {
            // get matrix of two camers
            Mat T1, T2;
            get_pose(T1, T2, frame->frame_index, i, GT);
            // much every previous frames in map
            vector<DMatch> frame_matches;
            matcher->match(frame->Descriptors, Descriptors, frame_matches);
            filterMatches(frame_matches, frame->Descriptors, Descriptors, frame->keypoints, keypoints);
            vector<Point3d> new_points = triangulation(frame->keypoints, keypoints, frame_matches, T1, T2);
            // test trangulation
            // test(frame_matches, frame->keypoints, keypoints, R, t, new_points);
            // cout << "find:" << frame_matches.size() << "pairs between frame " << i << " and previous frame" << endl;
            int count_add = 0;

            // add new points to map
            for (int match_index = 0; match_index < frame_matches.size(); match_index++) {
                // filter the point which has already matched with map
                vector<int>::iterator find_same = find(isInMap.begin(), isInMap.end(), frame_matches[match_index].trainIdx);
                if (find_same == isInMap.end()) {
                    MapPoint::Ptr newpoint(new MapPoint(new_points[match_index], Descriptors.row(frame_matches[match_index].trainIdx)));
                    newpoint->addIndex(i, keypoints[frame_matches[match_index].trainIdx].pt);  // add camera index and 2dpoints
                    map->addPoint(newpoint);
                    count_add++;
                }
            }
            cout << "add new " << count_add << "points to map" << endl;
        }
        // add new frame to map    
        Frame::Ptr newFrame(new Frame(keypoints, Descriptors, i));
        map->addFrame(newFrame);

    }

    cout << "finish" << endl;

    // output to ply 
    deque<MapPoint::Ptr> finalPoints = map->mapPoints;
    deque<MapPoint::Ptr>::iterator i;
    OutPly << "ply" << endl;
    OutPly << "format ascii 1.0" << endl;
    OutPly << "element vertex " << finalPoints.size() << endl;
    OutPly << "property float x" << endl;
    OutPly << "property float y" << endl;
    OutPly << "property float z" << endl;
    OutPly << "property uchar red" << endl;
    OutPly << "property uchar green" << endl;
    OutPly << "property uchar blue" << endl;
    OutPly << "end_header" << endl;
    int num_points2d = 0;
    int num_points3d = finalPoints.size();
    for (i = finalPoints.begin(); i != finalPoints.end(); i++) {
        OutPly << (*i).get()->pos.x << " " << (*i).get()->pos.y << " " << (*i).get()->pos.z << " " << "255 255 255" << endl;
        for (int j : (*i).get()->observationIndex) {
            num_points2d++;
        }
    }

    // output file for BA
    int point_index = 0;
    OutFile << num_data << " " << num_points3d << " " << num_points2d << endl;
    for (i = finalPoints.begin(); i != finalPoints.end(); i++) {
        int index_observation = 0;
        for (int j : (*i).get()->observationIndex) {
            OutFile << j << " " << point_index << "     " << (*i).get()->points[index_observation].x << " " << (*i).get()->points[index_observation].y << endl;
            index_observation++;
        }
        point_index++;
    }

    for (int i = 0; i < num_data; i++) {
        Eigen::Quaterniond q_pose;
        q_pose.x() = obs[i][4];
        q_pose.y() = obs[i][5];
        q_pose.z() = obs[i][6];
        q_pose.w() = obs[i][3];
        cv::Mat Rotation_matrix;
        cv::Mat Rotation_vector;
        cv::eigen2cv(q_pose.normalized().toRotationMatrix(), Rotation_matrix);
        cv::Rodrigues(Rotation_matrix, Rotation_vector);
        OutFile << Rotation_vector.at<double>(0, 0) << endl;
        OutFile << Rotation_vector.at<double>(1, 0) << endl;
        OutFile << Rotation_vector.at<double>(2, 0) << endl;
        OutFile << obs[i][0] << endl;
        OutFile << obs[i][1] << endl;
        OutFile << obs[i][2] << endl;
        OutFile << "744.375" << endl;
        OutFile << "426" << endl;
        OutFile << "240" << endl;
    }
    deque<MapPoint::Ptr>::iterator j;
    for (j = finalPoints.begin(); j != finalPoints.end(); j++) {
        OutFile << (*j).get()->pos.x << endl;
        OutFile << (*j).get()->pos.y << endl;
        OutFile << (*j).get()->pos.z << endl;
    }
    OutFile.close();
    OutPly.close();
    return 0;
}
