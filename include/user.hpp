#ifndef __USER_HPP__
#define __USER_HPP__

#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <array>
#include "BFS.hpp"

using namespace cv;
using namespace std;

namespace user
{
    int perspective_fix(Mat &src);
    void generate_map(vector<vector<int>> &map);
    int find_treasure(Mat &src, Mat &src_without_treasure, vector<array<int, 2>> &treasure_pos);
    void block_scan(Mat &src_without_treasure, vector<vector<int>> &map);
    vector<vector<array<int, 2>>> treasure_extend(vector<array<int, 2>> &treasure_pos_filtered, array<int, 2> &special_point);
    vector<int> sort(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos_filtered, array<int, 2> start);
    vector<int> sort_ex(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos_filtered, array<int, 2> start, array<int, 2> end);
    vector<vector<array<int, 2>>> path(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos_filtered, vector<int> &min_order, array<int, 2> start);
    vector<vector<array<int, 2>>> path_ex(vector<vector<int>> &map_z, vector<array<int, 2>> &treasure_pos_extended, vector<int> &min_order_ex, array<int, 2> start, array<int, 2> end);
    vector<int> path_solver(vector<array<int, 2>> &path, vector<vector<int>> &map);
    vector<int> path_solver_ex(vector<array<int, 2>> &path, vector<vector<int>> &map);
}
#endif