#include "user.hpp"
#include <iostream>

bool isValid(int x, int y, int rows, int cols, const std::vector<std::vector<int>> &map)
{
    return x >= 0 && x < rows && y >= 0 && y < cols && map[x][y] == 0;
}
namespace direction
{
    enum direction_num
    {
        up = 3,
        down = 4,
        left = 2,
        right = 1,
        mup = 7,
        mdown = 8,
        mleft = 6,
        mright = 5
    };
}

int length(int start, int end, vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos)
{
    if (start == end)
        return 0;
    else
    {
        int rows = map.size();
        int cols = map[0].size();
        queue<array<int, 2>> q;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        // 将起始点加入队列并标记为已访问
        q.push(treasure_pos[start]);
        visited[treasure_pos[start][0]][treasure_pos[start][1]] = true;

        int distance = 0;

        while (!q.empty())
        {
            int size = q.size();

            // 遍历当前层的所有点
            for (int i = 0; i < size; i++)
            {
                array<int, 2> current = q.front();
                q.pop();

                // 检查是否到达目标点
                if (current[0] == treasure_pos[end][0] && current[1] == treasure_pos[end][1])
                {
                    return distance;
                }

                // 尝试四个方向移动
                int dx[] = {0, 0, 1, -1};
                int dy[] = {1, -1, 0, 0};
                for (int j = 0; j < 4; ++j)
                {
                    int newX = current[0] + dx[j];
                    int newY = current[1] + dy[j];

                    // 如果新位置有效且未访问过，则加入队列并标记为已访问
                    if (isValid(newX, newY, rows, cols, map) && !visited[newX][newY])
                    {
                        q.push({newX, newY});
                        visited[newX][newY] = true;
                    }
                }
            }
            distance++;
        }
    }
    return -1;
}

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
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<Point2f> track_point;
    vector<Point2f> center_point;
    for (int i = 0; i < hierarchy.size(); i++)
    {
        if (hierarchy[i][3] == 0 && hierarchy[i][2] != -1 && hierarchy[hierarchy[i][2]][2] != -1 && hierarchy[hierarchy[hierarchy[i][2]][2]][0] == -1)
        {
            track_point.push_back(get_center_point(contours[hierarchy[hierarchy[i][2]][2]]));
        }
    }

    if (track_point.size() != 4)
    {
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
void user::generate_map(vector<vector<int>> &map)
{
    map = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
           {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0},
           {-1, 0, -1, 0, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1},
           {-1, 0, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1},
           {-1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, 0, -1},
           {-1, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1},
           {-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1},
           {-1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1},
           {-1, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1},
           {-1, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
           {-1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1},
           {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1},
           {-1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, -1, -1, 0, -1},
           {-1, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1},
           {-1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1},
           {-1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1},
           {-1, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1},
           {-1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, -1},
           {-1, -1, -1, 0, -1, 0, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, 0, -1, 0, -1},
           {0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
           {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
}
int user::find_treasure(Mat &src, Mat &src_without_treasure, vector<array<int, 2>> &treasure_pos)
{
    resize(src, src, Size(800, 800));

    src_without_treasure = src.clone();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < hierarchy.size(); i++)
    {
        if (boundingRect(contours[i]).width < 47 && contourArea(contours[i]) < 1700 && contourArea(contours[i]) > 800 && ((float)boundingRect(contours[i]).width) / ((float)boundingRect(contours[i]).height) < 1.1 & ((float)boundingRect(contours[i]).width) / ((float)boundingRect(contours[i]).height) > 0.9)
        {
            drawContours(src_without_treasure, contours, i, Scalar(255, 255, 255), -1);
            array<int, 2> pos = {((int)get_center_point(contours[i]).y) / 80 * 2 + 1, ((int)get_center_point(contours[i]).x) / 80 * 2 + 1};
            treasure_pos.push_back(pos);
        }
    }
    return treasure_pos.size();
}
void user::block_scan(Mat &src_without_treasure, vector<vector<int>> &map)
{
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            Rect rect(20 + 40 * j, 20 + 40 * i, 40, 40);
            vector<vector<Point>> contours;
            vector<Vec4i> hierarchy;
            findContours(src_without_treasure(rect), contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
            if (contours.size() == 1)
            {
                if (contourArea(contours[0]) < 1400)
                    map[i + 1][j + 1] = -1;
            }
            else
            {
                map[i + 1][j + 1] = -1;
            }
        }
    }
}
vector<vector<array<int, 2>>> user::treasure_extend(vector<array<int, 2>> &treasure_pos_filtered, array<int, 2> &special_point)
{
    vector<vector<array<int, 2>>> treasure_pos_extended_series;

    for (int i = 0; i < 16; i++)
    {
        vector<array<int, 2>> treasure_pos_extended;
        // 0为我方宝藏，1为我方伪宝藏，2为对方宝藏，3为对方伪宝藏
        switch (i)
        {
            // 我方为00时,2\3;002，003
            // 我方为0时,23\22;023,022
            // 我方为01时,012
            // 我方为1时,22;122
        case 0:
            // 仅去1对面
            // 200
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            break;
        case 1:
            // 仅去2对面
            // 020
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            break;
        case 2:
            // 仅去3对面
            // 002
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 3:
            // 仅去special对面
            // 003,030,300
            treasure_pos_extended.push_back({20 - special_point[0], 20 - special_point[1]});
            break;
        case 4:
            // 去special;1对面
            // 203,230
            treasure_pos_extended.push_back({special_point[0], special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            break;
        case 5:
            // 去special;2对面
            // 023,320,
            treasure_pos_extended.push_back({special_point[0], special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            break;
        case 6:
            // 去special;3对面
            // 302,032
            treasure_pos_extended.push_back({special_point[0], special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 7:
            // 去1和2对面
            // 220
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            break;
        case 8:
            // 去1和3对面
            // 202
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 9:
            // 去2和3对面
            // 022
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 10:
            // 去sp和1对面
            // 210,201
            treasure_pos_extended.push_back({20 - special_point[0], 20 - special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            break;
        case 11:
            // 去sp和2对面
            // 021,120
            treasure_pos_extended.push_back({20 - special_point[0], 20 - special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            break;
        case 12:
            // 去sp和3对面
            // 012,102
            treasure_pos_extended.push_back({20 - special_point[0], 20 - special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 13:
            // 去sp;2和3对面
            // 122
            treasure_pos_extended.push_back({special_point[0], special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 14:
            // 去sp;去1和3对面
            // 212
            treasure_pos_extended.push_back({special_point[0], special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[2][0], 20 - treasure_pos_filtered[2][1]});
            break;
        case 15:
            // 去sp;去1和2对面
            // 221
            treasure_pos_extended.push_back({special_point[0], special_point[1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[0][0], 20 - treasure_pos_filtered[0][1]});
            treasure_pos_extended.push_back({20 - treasure_pos_filtered[1][0], 20 - treasure_pos_filtered[1][1]});
            break;
        }
        treasure_pos_extended_series.push_back(treasure_pos_extended);
    }
    return treasure_pos_extended_series;
}
vector<array<int, 2>> findShortestPath(vector<vector<int>> &map, array<int, 2> start, array<int, 2> end)
{
    int rows = map.size();
    int cols = map[0].size();

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<array<int, 2>>> parent(rows, vector<array<int, 2>>(cols));

    queue<array<int, 2>> q;
    q.push(start);
    visited[start[0]][start[1]] = true;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!q.empty())
    {
        array<int, 2> curr = q.front();
        q.pop();

        int x = curr[0];
        int y = curr[1];

        if (x == end[0] && y == end[1])
        {
            // 到达
            vector<array<int, 2>> path;
            while (!(x == start[0] && y == start[1]))
            {
                path.push_back({x, y});
                array<int, 2> p = parent[x][y];
                x = p[0];
                y = p[1];
            }
            path.push_back({start[0], start[1]});
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValid(newX, newY, rows, cols, map) && !visited[newX][newY])
            {
                q.push({newX, newY});
                visited[newX][newY] = true;
                parent[newX][newY] = {x, y};
            }
        }
    }

    // If no path is found
    return vector<array<int, 2>>();
}

vector<int> user::sort(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos_filtered, array<int, 2> start)
{
    vector<array<int, 2>> path_pass_point;
    for (int i = 0; i < treasure_pos_filtered.size(); i++)
        path_pass_point.push_back(treasure_pos_filtered[i]);
    // 在开始插入起始点
    path_pass_point.insert(path_pass_point.begin(), start);
    // 形成遍历距离表
    vector<vector<int>> graph;
    {
        for (int i = 0; i < path_pass_point.size(); i++)
        {
            vector<int> subgraph;
            for (int j = 0; j < path_pass_point.size(); j++)
            {
                subgraph.push_back(length(i, j, map, path_pass_point));
            }
            graph.push_back(subgraph);
        }
    }
    // 存储所有经过点
    vector<int> numbers;
    for (int i = 1; i < path_pass_point.size(); i++)
    {
        numbers.push_back(i);
    }
    // 排序，规范全排列输出
    std::sort(numbers.begin(), numbers.end());

    // 存储最短距离，循环次数
    int min_length = 0;
    int times = 0;
    vector<int> min_order;
    do
    {
        vector<int> order;
        // 存储每次排列
        for (int number : numbers)
        {
            order.push_back(number);
        }
        // 添加0到1的距离
        int sum_length = graph[0][order[0]];
        // 添加其他距离
        if (numbers.size() > 1)
        {
            for (int i = 0; i < numbers.size() - 1; i++)
            {
                sum_length += graph[order[i]][order[i + 1]];
            }
        }
        // 存储第一次
        if (min_length == 0)
        {
            min_length = sum_length;
            min_order = order;
        }
        // 存储更小的
        if (min_length > sum_length)
        {
            min_length = sum_length;
            min_order = order;
        }
        // 循环太多次就不循环了
        if (times++ > 600000)
            break;
    } while (next_permutation(numbers.begin(), numbers.end()));
    return min_order;
}

vector<int> user::sort_ex(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos_filtered, array<int, 2> start, array<int, 2> end)
{
    vector<array<int, 2>> path_pass_point;
    // 在开始插入起始点
    path_pass_point.push_back(start);
    for (int i = 0; i < treasure_pos_filtered.size(); i++)
        path_pass_point.push_back(treasure_pos_filtered[i]);
    path_pass_point.push_back(end);
    // 形成遍历距离表
    vector<vector<int>> graph;
    {
        for (int i = 0; i < path_pass_point.size(); i++)
        {
            vector<int> subgraph;
            for (int j = 0; j < path_pass_point.size(); j++)
            {
                subgraph.push_back(length(i, j, map, path_pass_point));
            }
            graph.push_back(subgraph);
        }
    }
    // 存储所有经过点
    vector<int> numbers;
    for (int i = 1; i < path_pass_point.size() - 1; i++)
    {
        numbers.push_back(i);
    }
    // 排序，规范全排列输出
    std::sort(numbers.begin(), numbers.end());

    // 存储最短距离，循环次数
    int min_length = 0;
    int times = 0;
    vector<int> min_order;
    do
    {
        vector<int> order;
        // 存储每次排列
        for (int number : numbers)
        {
            order.push_back(number);
        }
        // 添加0到1的距离
        int sum_length = graph[0][order[0]];
        // 添加其他距离
        if (numbers.size() > 1)
        {
            for (int i = 0; i < numbers.size() - 1; i++)
            {
                sum_length += graph[order[i]][order[i + 1]];
            }
        }
        sum_length += graph[order.back()][path_pass_point.size() - 1];
        // 存储第一次
        if (min_length == 0)
        {
            min_length = sum_length;
            min_order = order;
        }
        // 存储更小的
        if (min_length > sum_length)
        {
            min_length = sum_length;
            min_order = order;
        }
        // 循环太多次就不循环了
        if (times++ > 600000)
            break;
    } while (next_permutation(numbers.begin(), numbers.end()));
    return min_order;
}
vector<vector<array<int, 2>>> user::path(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos_filtered, vector<int> &min_order, array<int, 2> start)
{
    vector<array<int, 2>> path_pass_point;
    for (int i = 0; i < treasure_pos_filtered.size(); i++)
        path_pass_point.push_back(treasure_pos_filtered[i]);
    // 最后在开始插入起始点
    path_pass_point.insert(path_pass_point.begin(), start);

    vector<vector<array<int, 2>>> path_series;
    path_series.push_back(findShortestPath(map, path_pass_point[0], path_pass_point[min_order[0]]));
    for (int i = 0; i < min_order.size() - 1; i++)
    {
        vector<array<int, 2>> path;
        path = findShortestPath(map, path_pass_point[min_order[i]], path_pass_point[min_order[i + 1]]);
        path_series.push_back(path);
    }
    return path_series;
}

vector<vector<array<int, 2>>> user::path_ex(vector<vector<int>> &map_z, vector<array<int, 2>> &treasure_pos_extended, vector<int> &min_order_ex, array<int, 2> start, array<int, 2> end)
{
    vector<vector<array<int, 2>>> path_series;
    vector<array<int, 2>> path_pass_point;
    path_pass_point.push_back(start);
    for (int i : min_order_ex)
    {
        path_pass_point.push_back(treasure_pos_extended[i - 1]);
    }
    path_pass_point.push_back(end);

    for (int i = 0; i < path_pass_point.size() - 1; i++)
    {
        vector<array<int, 2>> path;
        path = findShortestPath(map_z, path_pass_point[i], path_pass_point[i + 1]);
        path_series.push_back(path);
    }
    return path_series;
}

vector<int> user::path_solver(vector<array<int, 2>> &path, vector<vector<int>> &map)
{
    vector<int> turn_to;
    int rows = map.size();
    int cols = map[0].size();
    int dx[] = {0, 1, 0, -1, 0};
    int dy[] = {1, 0, -1, 0, 1};
    for (int i = 0; i < path.size() - 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int newX1 = path[i][0] + dx[j];
            int newY1 = path[i][1] + dy[j];
            int newX2 = path[i][0] + dx[j + 1];
            int newY2 = path[i][1] + dy[j + 1];
            if (i == path.size() - 2)
            {
                turn_to.pop_back();
                if (path[i][0] - path[i + 1][0] == 1)
                    turn_to.push_back(direction::mup);
                if (path[i][0] - path[i + 1][0] == -1)
                    turn_to.push_back(direction::mdown);
                if (path[i][1] - path[i + 1][1] == 1)
                    turn_to.push_back(direction::mleft);
                if (path[i][1] - path[i + 1][1] == -1)
                    turn_to.push_back(direction::mright);
                break;
            }
            else if (i != 0 && (isValid(newX1, newY1, rows, cols, map) && isValid(newX2, newY2, rows, cols, map)))
            {
                if (path[i][0] - path[i + 1][0] == 1)
                    turn_to.push_back(direction::up);
                if (path[i][0] - path[i + 1][0] == -1)
                    turn_to.push_back(direction::down);
                if (path[i][1] - path[i + 1][1] == 1)
                    turn_to.push_back(direction::left);
                if (path[i][1] - path[i + 1][1] == -1)
                    turn_to.push_back(direction::right);
                break;
            }
        }
    }
    // for (int i = 0; i < turn_to.size(); i++)
    //     cout << turn_to[i];
    // cout << endl;
    return turn_to;
}

vector<int> user::path_solver_ex(vector<array<int, 2>> &path, vector<vector<int>> &map)
{
    vector<int> turn_to;
    int rows = map.size();
    int cols = map[0].size();
    int dx[] = {0, 1, 0, -1, 0};
    int dy[] = {1, 0, -1, 0, 1};
    if (path[0] == array<int, 2>({5, 9}) || path[0] == array<int, 2>({15, 11}))
    {
        if (path[0][0] - path[1][0] == 1)
            turn_to.push_back(direction::up);
        if (path[0][0] - path[1][0] == -1)
            turn_to.push_back(direction::down);
        if (path[0][1] - path[1][1] == 1)
            turn_to.push_back(direction::left);
        if (path[0][1] - path[1][1] == -1)
            turn_to.push_back(direction::right);
    }
    for (int i = 0; i < path.size() - 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int newX1 = path[i][0] + dx[j];
            int newY1 = path[i][1] + dy[j];
            int newX2 = path[i][0] + dx[j + 1];
            int newY2 = path[i][1] + dy[j + 1];
            if (i != 0 && (isValid(newX1, newY1, rows, cols, map) && isValid(newX2, newY2, rows, cols, map)))
            {
                if (path[i][0] - path[i + 1][0] == 1)
                    turn_to.push_back(direction::up);
                if (path[i][0] - path[i + 1][0] == -1)
                    turn_to.push_back(direction::down);
                if (path[i][1] - path[i + 1][1] == 1)
                    turn_to.push_back(direction::left);
                if (path[i][1] - path[i + 1][1] == -1)
                    turn_to.push_back(direction::right);
                break;
            }
        }
    }
    if (path.back() != array<int, 2>({5, 9}) && path.back() != array<int, 2>({15, 11}))
    {
        switch (turn_to.back())
        {
        case direction::up:
            turn_to.pop_back();
            turn_to.push_back(direction::mup);
            break;
        case direction::down:
            turn_to.pop_back();
            turn_to.push_back(direction::mdown);
            break;
        case direction::left:
            turn_to.pop_back();
            turn_to.push_back(direction::mleft);
            break;
        case direction::right:
            turn_to.pop_back();
            turn_to.push_back(direction::mright);
            break;
        }
    }
    // for (int i = 0; i < turn_to.size(); i++)
    //     cout << turn_to[i];
    // cout << endl;
    return turn_to;
}