#include "user.hpp"
#include <iostream>

template <typename T>
Point2f get_center_point(vector<T> contours_i)
{
    float x = 0;
    float y = 0;
    for (int i = 0; i < contours_i.size(); i++)
    {
        x += contours_i[i].x;
        y += contours_i[i].y;
    }
    return Point2f(x / contours_i.size(), y / contours_i.size());
}

int user::perspective_fix(Mat &src)
{
    Mat src_without_treasure;
    cvtColor(src, src_without_treasure, COLOR_BGR2GRAY);
    threshold(src_without_treasure, src_without_treasure, 100, 255, THRESH_BINARY);
    morphologyEx(src_without_treasure, src_without_treasure, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3)));

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src_without_treasure, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<Point2f> track_point;
    vector<Point2f> center_point;
    for (int i = 0; i < hierarchy.size(); i++)
    {
        if (hierarchy[i][3] == 0 & hierarchy[hierarchy[hierarchy[i][2]][2]][0] == -1)
        {
            track_point.push_back(get_center_point(contours[hierarchy[hierarchy[i][2]][2]]));
        }
    }

    if (track_point.size() != 4)
    {
        cout << "found" << track_point.size() << "track points" << endl;
        return -1;
    }

    Point2f src_point[4];
    {
        Point2f center_point = get_center_point(track_point);
        for (int i = 0; i < 4; i++)
        {
            if (track_point[i].x < center_point.x)
            {
                if (track_point[i].y < center_point.y)
                {
                    src_point[3] = track_point[i];
                }
                else
                {
                    src_point[0] = track_point[i];
                }
            }
            else
            {
                if (track_point[i].y < center_point.y)
                {
                    src_point[2] = track_point[i];
                }
                else
                {
                    src_point[1] = track_point[i];
                }
            }
        }
    }
    Point2f dst_point[4] = {Point2f(0, 1000), Point2f(1000, 1000), Point2f(1000, 0), Point2f(0, 0)};
    Mat M = getPerspectiveTransform(src_point, dst_point);
    warpPerspective(src, src, M, Size(1000, 1000));
    return 0;
}
void user::generate_map(int (*map)[21])
{
    for (int i = 0; i < 21; i++)
    {
        map[0][i] = 1;
        map[i][0] = 1;
        map[20][i] = 1;
        map[i][20] = 1;
    }
    map[1][20] = 0;
    map[19][0] = 0;
}
void user::find_treasure(Mat &src, int (*map)[21], Mat &src_without_treasure)
{
    vector<array<int, 2>> treasure_pos;
    resize(src, src, Size(800, 800));

    cvtColor(src, src_without_treasure, COLOR_BGR2GRAY);
    threshold(src_without_treasure, src_without_treasure, 100, 255, THRESH_BINARY);
    morphologyEx(src_without_treasure, src_without_treasure, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3)));

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src_without_treasure, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < hierarchy.size(); i++)
    {
        if (boundingRect(contours[i]).width<47 & contourArea(contours[i]) < 900 & contourArea(contours[i])> 800 & ((float)boundingRect(contours[i]).width) / ((float)boundingRect(contours[i]).height) < 1.1 & ((float)boundingRect(contours[i]).width) / ((float)boundingRect(contours[i]).height) > 0.9)
        {
            drawContours(src, contours, i, Scalar(255, 0, 0), 2);
            drawContours(src_without_treasure, contours, i, Scalar(255, 255, 255), -1);
            array<int, 2> pos = {((int)get_center_point(contours[i]).x) / 80, ((int)get_center_point(contours[i]).y) / 80};
            treasure_pos.push_back(pos);
        }
    }
    cout << "找到宝藏点数：" << treasure_pos.size() << endl;
    for (int i = 0; i < treasure_pos.size(); i++)
        map[treasure_pos[i][1] * 2 + 1][treasure_pos[i][0] * 2 + 1] = 2;
}
void user::block_scan(Mat &src_without_treasure)
{
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            Rect rect(20 + 40 * i, 20 + 40 * j, 40, 40);
            // imshow("rect", src(rect));
            // waitKey();
        }
    }
}