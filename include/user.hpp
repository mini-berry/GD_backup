#ifndef __USER_HPP__
#define __USER_HPP__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <array>
#include "BFS.hpp"

using namespace cv;
using namespace std;

namespace user
{
    int perspective_fix(Mat &src);
    void generate_map(vector<vector<int>> &map);
    void find_treasure(Mat &src, Mat &src_without_treasure, vector<array<int, 2>> &treasure_pos);
    void block_scan(Mat &src_without_treasure, vector<vector<int>> &map);
    vector<array<int, 2>> point_order(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos);
}
#endif