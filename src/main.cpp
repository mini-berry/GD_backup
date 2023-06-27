#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "user.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat src = imread("/home/lane/gd/bin/map.png");
    resize(src, src, Size(1000, 1000));

    while (user::perspective_fix(src) != 0)
    {
        cout << "getting new img" << endl;
        waitKey();
        src = imread("/home/lane/gd/bin/map.png");
        resize(src, src, Size(1000, 1000));
    }

    int margin = 115;
    Rect rect(margin, margin, 1000 - 2 * margin, 1000 - 2 * margin);
    src = src(rect);

    vector<vector<int>> map(21, vector<int>(21, 0));
    user::generate_map(map);

    Mat src_without_treasure;
    vector<array<int, 2>> treasure_pos;
    user::find_treasure(src, src_without_treasure, treasure_pos);

    user::block_scan(src_without_treasure, map);

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int flag = 0;
            for (int k = 0; k < treasure_pos.size(); k++)
            {
                if (j == treasure_pos[k][0] & i == treasure_pos[k][1])
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
                case 1:
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
    vector<array<int, 2>> sequence = user::point_order(map, treasure_pos);

    // imshow("result", src);
    // waitKey();
}