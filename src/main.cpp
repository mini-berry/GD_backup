#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include "user.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int treasure_num = 0;
Mat src;
vector<vector<int>> map(21, vector<int>(21, 0));
Mat src_without_treasure;
vector<array<int, 2>> treasure_pos;

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

Mat capture()
{

    Mat src;
    while (src.empty())
    {
        src = imread("/home/lane/gd/bin/map2.png");
    }
    return src;
}
void try_to_generate()
{
    do
    {
        src = capture();
        imshow("capture", src);
        waitKey(1);
        while (user::perspective_fix(src) != 0)
        {
            src = capture();
        }
        resize(src, src, Size(1000, 1000));
        int margin = 115;
        Rect rect(margin, margin, 1000 - 2 * margin, 1000 - 2 * margin);
        src = src(rect);

        // 生成地图，地图内所有点均为i行j列，但涉及x,y坐标的要反过来
        // 清空所有变量
        user::generate_map(map);
        treasure_pos.clear();
        src_without_treasure.release();
        treasure_num = user::find_treasure(src, src_without_treasure, treasure_pos);
    } while (!(treasure_num == 8));
}

int main(int argc, char **argv)
{
    do
    {
        try_to_generate();
    } while (0);
    // 按A确认，按B重新拍摄
    array<int, 2> speical_point;
    vector<array<int, 2>> treasure_pos_filtered;
    for (int i = 0; i < treasure_pos.size(); i++)
    {
        if (treasure_pos[i][1] < 10)
        {
            if (treasure_pos[i][0] < 10 && treasure_pos[i][1] < 9)
                treasure_pos_filtered.push_back(treasure_pos[i]);
            else
            {
                if (treasure_pos[i][0] > 10 && treasure_pos[i][1] < 10)
                    treasure_pos_filtered.push_back(treasure_pos[i]);
                else
                {
                    speical_point = treasure_pos[i];
                }
            }
        }
    }
    user::block_scan(src_without_treasure, map);
    // extend点直达即可
    vector<vector<array<int, 2>>> treasure_pos_extended = user::treasure_extend(treasure_pos_filtered, speical_point);
    vector<int> min_order = user::sort(map, treasure_pos_filtered, {19, 0});
    cout << "最短路径为： ";
    for (int i = 0; i < min_order.size(); i++)
        cout << min_order[i] << " ";
    cout << endl;
    vector<vector<array<int, 2>>> path_0 = user::path(map, treasure_pos_filtered, min_order, {19, 0});
    vector<vector<vector<array<int, 2>>>> path_1 = user::path_ex(map, treasure_pos_filtered, min_order, treasure_pos_extended);
    show_map(map, path_1[0][0]);
}