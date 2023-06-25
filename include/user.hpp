#ifndef __USER_HPP__
#define __USER_HPP__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

namespace user
{
    int perspective_fix(Mat &src);
    void generate_map(int (*map)[21]);
    void find_treasure(Mat &src, int (*map)[21], Mat &src_without_treasure);
    void block_scan(Mat &src_without_treasure, int (*map)[21]);
}
#endif