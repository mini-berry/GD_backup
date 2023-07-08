#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include "user.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int treasure_num = 0;
Mat src;
vector<vector<int>> map(21, vector<int>(21, 0));
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
        cvtColor(src, src, COLOR_BGR2GRAY);
        threshold(src, src, 100, 255, THRESH_BINARY);
        erode(src, src, getStructuringElement(MORPH_RECT, Size(3, 3)));
    }
    return src;
}

int main(int argc, char **argv)
{
    src = capture();

    user::perspective_fix(src);

    resize(src, src, Size(1000, 1000));
    int margin = 115;
    Rect rect(margin, margin, 1000 - 2 * margin, 1000 - 2 * margin);
    src = src(rect);

    // 生成地图，地图内所有点均为i行j列，但涉及x,y坐标的要反过来
    // 清空所有变量
    user::generate_map(map);
    Mat src_without_treasure;
    vector<array<int, 2>> treasure_pos;
    treasure_num = user::find_treasure(src, src_without_treasure, treasure_pos);
    // imshow("result", src_without_treasure);
    waitKey(0);
    cout << treasure_pos.size() << endl;

    array<int, 2> speical_point;
    vector<array<int, 2>> treasure_pos_filtered;
    for (int i = 0; i < treasure_pos.size(); i++)
    {
        if (treasure_pos[i][1] < 8)
        {
            treasure_pos_filtered.push_back(treasure_pos[i]);
        }
    }
    if (treasure_pos_filtered.size() == 3)
    {
        for (int i = 0; i < treasure_pos.size(); i++)
        {
            if (treasure_pos[i][1] > 8 & treasure_pos[i][1] < 10)
                speical_point = {treasure_pos[i][0], treasure_pos[i][1]};
        }
    }
    if (treasure_pos_filtered.size() != 3)
    {
        for (int i = 0; i < treasure_pos.size(); i++)
        {
            if (treasure_pos[i][1] == 9 & treasure_pos[i][0] > 10)
            {
                treasure_pos_filtered.push_back(treasure_pos[i]);
            }
            if (treasure_pos[i][1] == 9 & treasure_pos[i][0] < 10)
            {
                speical_point = {treasure_pos[i][0], treasure_pos[i][1]};
            }
        }
    }
    // user::block_scan(src_without_treasure, map);
    // extend点直达即可
    vector<vector<array<int, 2>>> treasure_pos_extended = user::treasure_extend(treasure_pos_filtered, speical_point);
    vector<int> min_order = user::sort(map, treasure_pos_filtered, {19, 0});
    cout << "最短路径为： ";
    for (int i = 0; i < min_order.size(); i++)
        cout << min_order[i] << " ";
    cout << endl;

    // 计算路径时先封锁特别恶心的点和sp点
    int exp_1 = map[5][9];
    int exp_2 = map[15][11];
    int sp = map[speical_point[0]][speical_point[1]];
    map[5][9] = -1;
    map[15][11] = -1;
    map[speical_point[0]][speical_point[1]] = -1;
    vector<vector<array<int, 2>>> path_0 = user::path(map, treasure_pos_filtered, min_order, {19, 0});
    // 恢复特别恶心的点和sp点
    map[5][9] = exp_1;
    map[15][11] = exp_2;
    map[speical_point[0]][speical_point[1]] = sp;

    // 存储三条路径
    vector<vector<int>> turn_0;
    // 存储第一阶段的点
    for (int i = 0; i < path_0.size(); i++)
    {
        turn_0.push_back(user::path_solver(path_0[i], map));
    }

    vector<vector<vector<int>>> turn_1_series;
    for (int i = 0; i < treasure_pos_extended.size(); i++)
    {
        vector<vector<int>> map_z = map;
        vector<vector<int>> turn_1;
        vector<array<int, 2>> turn_1_pass;
        int pass_59 = 0;
        int pass_1511 = 0;
        for (int j = 0; j < treasure_pos_extended[i].size(); j++)
        {
            if (treasure_pos_extended[i][j] == array<int, 2>({5, 9}))
                pass_59 = 1;
            if (treasure_pos_extended[i][j] == array<int, 2>({15, 11}))
                pass_1511 = 1;
        }
        if (pass_59 == 0)
        {
            map_z[5][9] = -1;
        }
        if (pass_1511 == 0)
        {
            map_z[15][11] = -1;
        }

        turn_1_pass.insert(turn_1_pass.end(), treasure_pos_extended[i].begin(), treasure_pos_extended[i].end());
        vector<int> min_order_ex = user::sort_ex(map_z, turn_1_pass, treasure_pos_filtered[min_order.back() - 1], {1, 20});

        vector<vector<array<int, 2>>> path_1 = user::path_ex(map_z, treasure_pos_extended[i], min_order_ex, treasure_pos_filtered[min_order.back() - 1], {1, 20});
        vector<vector<int>> path_1_turn;
        if (i == 3)
        {
            show_map(map, path_1[0]);
            show_map(map, path_1[1]);
        }
        for (vector<array<int, 2>> i : path_1)
        {
            path_1_turn.push_back(user::path_solver_ex(i, map_z));
        }
        turn_1_series.push_back(path_1_turn);
    }
    // show_map(map, path_0[1]);
    cout << "A";
    for (int i = 0; i < turn_0.size(); i++)
    {
        for (int j = 0; j < turn_0[i].size(); j++)
        {
            cout << turn_0[i][j];
        }
        if (i != turn_0.size() - 1)
            cout << ",";
    }
    cout << ";" << endl;
    char letter = 'B';
    for (int i = 0; i < turn_1_series.size(); i++)
    {
        cout << letter;
        for (int j = 0; j < turn_1_series[i].size(); j++)
        {
            for (int k = 0; k < turn_1_series[i][j].size(); k++)
            {
                cout << turn_1_series[i][j][k];
            }

            if (j == turn_1_series[i].size() - 1)
            {
                cout << ";" << endl;
            }
            else
            {
                cout << ",";
            }
        }
        letter++;
    }
}