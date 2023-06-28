#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include "user.hpp"
#include <iostream>

using namespace std;
using namespace cv;
void show_map(vector<vector<int>> &map, vector<array<int, 2>> point_series)
{
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int flag = 0;
            for (int k = 0; k < point_series.size(); k++)
            {
                if (i == point_series[k][0] & j == point_series[k][1])
                {
                    cout << "□ ";
                    flag = 1;
                }
            }

            if (flag == 0)
            {
                switch (map[i][j])
                {
                case 0:
                    cout << "  ";
                    break;
                case -1:
                    cout << "■ ";
                    break;
                default:
                    cout << "  ";
                    break;
                }
            }
        }
        cout << endl;
    }
}

int main(int argc, char **argv)
{
    Mat src = imread("/home/lane/gd/bin/map2.png");
    resize(src, src, Size(1000, 1000));

    while (user::perspective_fix(src) != 0)
    {
        cout << "getting new img" << endl;
        waitKey();
        src = imread("/home/lane/gd/bin/map2.png");
        resize(src, src, Size(1000, 1000));
    }

    int margin = 115;
    Rect rect(margin, margin, 1000 - 2 * margin, 1000 - 2 * margin);
    src = src(rect);

    // 地图内所有点均为i行j列，但涉及x,y坐标的要反过来。
    vector<vector<int>> map(21, vector<int>(21, 0));
    user::generate_map(map);

    Mat src_without_treasure;
    vector<array<int, 2>> treasure_pos;
    user::find_treasure(src, src_without_treasure, treasure_pos);

    user::block_scan(src_without_treasure, map);
    vector<vector<array<int, 2>>> sequence = user::point_order(map, treasure_pos);
    // imshow("result", src);
    // waitKey();
}