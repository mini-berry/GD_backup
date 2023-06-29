#include "BFS.hpp"

bool isValid(int x, int y, int rows, int cols, const std::vector<std::vector<int>> &map)
{
    return x >= 0 && x < rows && y >= 0 && y < cols && map[x][y] == 0;
}
namespace direction
{
    enum
    {
        up = 0,
        down = 1,
        left = 2,
        right = 3,
    };
}
namespace turn_to_d
{
    enum
    {
        straight = 0,
        back = 1,
        left = 2,
        right = 3,
        end = 4,
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

void show_map_BFS(vector<vector<int>> &map, vector<array<int, 2>> point_series)
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

vector<array<int, 2>> findShortestPath(vector<vector<int>> &map, array<int, 2> &start, array<int, 2> &end)
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

int cal_turn(array<int, 2> q1, array<int, 2> q2, array<int, 2> q3)
{ // 入弯前方向
    int origin_direction = -1;
    int after_direction = -1;
    int y_move_1 = q1[0] - q2[0];
    int x_move_1 = q1[1] - q2[1];
    if (x_move_1 == 0 && y_move_1 == 1)
        origin_direction = direction::up;
    if (x_move_1 == 0 && y_move_1 == -1)
        origin_direction = direction::down;
    if (x_move_1 == 1 && y_move_1 == 0)
        origin_direction = direction::left;
    if (x_move_1 == -1 && y_move_1 == 0)
        origin_direction = direction::right;
    // 过弯后方向
    int y_move_2 = q2[0] - q3[0];
    int x_move_2 = q2[1] - q3[1];
    if (x_move_2 == 0 && y_move_2 == 1)
        after_direction = direction::up;
    if (x_move_2 == 0 && y_move_2 == -1)
        after_direction = direction::down;
    if (x_move_2 == 1 && y_move_2 == 0)
        after_direction = direction::left;
    if (x_move_2 == -1 && y_move_2 == 0)
        after_direction = direction::right;

    if (origin_direction == direction::up)
    {
        switch (after_direction)
        {
        case direction::up:
            return turn_to_d::straight;
            break;
        case direction::down:
            return turn_to_d::back;
            break;
        case direction::left:
            return turn_to_d::left;
            break;
        case direction::right:
            return turn_to_d::right;
            break;
        }
    }
    if (origin_direction == direction::down)
    {
        switch (after_direction)
        {
        case direction::up:
            return turn_to_d::back;
            break;
        case direction::down:
            return turn_to_d::straight;
            break;
        case direction::left:
            return turn_to_d::right;
            break;
        case direction::right:
            return turn_to_d::left;
            break;
        }
    }
    if (origin_direction == direction::left)
    {
        switch (after_direction)
        {
        case direction::up:
            return turn_to_d::right;
            break;
        case direction::down:
            return turn_to_d::left;
            break;
        case direction::left:
            return turn_to_d::straight;
            break;
        case direction::right:
            return turn_to_d::back;
            break;
        }
    }
    if (origin_direction == direction::right)
    {
        switch (after_direction)
        {
        case direction::up:
            return turn_to_d::left;
            break;
        case direction::down:
            return turn_to_d::right;
            break;
        case direction::left:
            return turn_to_d::back;
            break;
        case direction::right:
            return turn_to_d::straight;
            break;
        }
    };
    return -1;
}

vector<int> turn_to(vector<vector<int>> &map, vector<array<int, 2>> path)
{
    vector<array<int, 3>> turn_point;

    // 判断是否是转向点
    int rows = map.size();
    int cols = map[0].size();
    int dx[] = {0, 1, 0, -1, 0};
    int dy[] = {1, 0, -1, 0, 1};
    for (int i = 1; i < path.size() - 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (isValid(path[i][0] + dx[j], path[i][1] + dy[j], rows, cols, map) & isValid(path[i][0] + dx[j + 1], path[i][1] + dy[j + 1], rows, cols, map))
            {
                turn_point.push_back({path[i][0], path[i][1], i});
                break;
            }
        }
    }
    vector<int> turn_to_v;
    for (int i = 0; i < turn_point.size(); i++)
    {
        // 入弯前方向
        int origin_direction = -1;
        int after_direction = -1;
        int y_move_1 = path[turn_point[i][2] - 1][0] - path[turn_point[i][2]][0];
        int x_move_1 = path[turn_point[i][2] - 1][1] - path[turn_point[i][2]][1];
        if (x_move_1 == 0 && y_move_1 == 1)
            origin_direction = direction::up;
        if (x_move_1 == 0 && y_move_1 == -1)
            origin_direction = direction::down;
        if (x_move_1 == 1 && y_move_1 == 0)
            origin_direction = direction::left;
        if (x_move_1 == -1 && y_move_1 == 0)
            origin_direction = direction::right;
        // 如弯后方向
        int y_move_2 = path[turn_point[i][2]][0] - path[turn_point[i][2] + 1][0];
        int x_move_2 = path[turn_point[i][2]][1] - path[turn_point[i][2] + 1][1];
        if (x_move_2 == 0 && y_move_2 == 1)
            after_direction = direction::up;
        if (x_move_2 == 0 && y_move_2 == -1)
            after_direction = direction::down;
        if (x_move_2 == 1 && y_move_2 == 0)
            after_direction = direction::left;
        if (x_move_2 == -1 && y_move_2 == 0)
            after_direction = direction::right;

        if (origin_direction == direction::up)
        {
            switch (after_direction)
            {
            case direction::up:
                turn_to_v.push_back(turn_to_d::straight);
                break;
            case direction::down:
                turn_to_v.push_back(turn_to_d::back);
                break;
            case direction::left:
                turn_to_v.push_back(turn_to_d::left);
                break;
            case direction::right:
                turn_to_v.push_back(turn_to_d::right);
                break;
            }
        }
        if (origin_direction == direction::down)
        {
            switch (after_direction)
            {
            case direction::up:
                turn_to_v.push_back(turn_to_d::back);
                break;
            case direction::down:
                turn_to_v.push_back(turn_to_d::straight);
                break;
            case direction::left:
                turn_to_v.push_back(turn_to_d::right);
                break;
            case direction::right:
                turn_to_v.push_back(turn_to_d::left);
                break;
            }
        }
        if (origin_direction == direction::left)
        {
            switch (after_direction)
            {
            case direction::up:
                turn_to_v.push_back(turn_to_d::right);
                break;
            case direction::down:
                turn_to_v.push_back(turn_to_d::left);
                break;
            case direction::left:
                turn_to_v.push_back(turn_to_d::straight);
                break;
            case direction::right:
                turn_to_v.push_back(turn_to_d::back);
                break;
            }
        }
        if (origin_direction == direction::right)
        {
            switch (after_direction)
            {
            case direction::up:
                turn_to_v.push_back(turn_to_d::left);
                break;
            case direction::down:
                turn_to_v.push_back(turn_to_d::right);
                break;
            case direction::left:
                turn_to_v.push_back(turn_to_d::back);
                break;
            case direction::right:
                turn_to_v.push_back(turn_to_d::straight);
                break;
            }
        }
    }
    turn_to_v.push_back(turn_to_d::end);
    return turn_to_v;
}

vector<vector<array<int, 2>>> user::BFS(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos)
{
    // 把起始点加入
    treasure_pos.insert(treasure_pos.begin(), {19, 0});
    treasure_pos.push_back({1, 20});
    // 遍历节约算力
    vector<vector<int>> graph;
    {
        for (int i = 0; i < treasure_pos.size(); i++)
        {
            vector<int> subgraph;
            for (int j = 0; j < treasure_pos.size(); j++)
                subgraph.push_back(length(i, j, map, treasure_pos));
            graph.push_back(subgraph);
        }
    }
    // 起始点不直接相连
    graph[0][treasure_pos.size() - 1] = -1;
    graph[treasure_pos.size() - 1][0] = -1;

    // 排列组合
    vector<int> numbers;
    for (int i = 1; i < treasure_pos.size() - 1; i++)
    {
        numbers.push_back(i);
    }
    // 排序，规范全排列输出
    sort(numbers.begin(), numbers.end());
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
        for (int i = 0; i < numbers.size() - 1; i++)
        {
            sum_length += graph[order[i]][order[i + 1]];
        }
        // 添加终点距离
        sum_length += graph[order[numbers.size() - 1]][treasure_pos.size() - 1];
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
    cout << "最短路径为:" << min_length << " ";
    cout << "循环次数:" << times << endl;

    // 输出序列
    cout << "最短路径: ";
    for (int i = 0; i < min_order.size(); i++)
        cout << min_order[i] << " ";
    cout << endl;

    vector<vector<array<int, 2>>> shortest_path_series;
    shortest_path_series.push_back(findShortestPath(map, treasure_pos[0], treasure_pos[min_order[0]]));
    // treasure_pos维数为n+2，min_order维数为n
    // 最后两路径为n-2到n-1
    for (int i = 0; i < min_order.size() - 1; i++)
    {
        vector<array<int, 2>> shortest_path = findShortestPath(map, treasure_pos[min_order[i]], treasure_pos[min_order[i + 1]]);
        shortest_path_series.push_back(shortest_path);
    }
    shortest_path_series.push_back(findShortestPath(map, treasure_pos[min_order.size()], treasure_pos[min_order.size() + 1]));

    for (int i = 0; i < shortest_path_series.size(); i++)
    {

        vector<int> turn_to_v = turn_to(map, shortest_path_series[i]);
        if (i != 0)
        {
            turn_to_v.insert(turn_to_v.begin(), cal_turn(shortest_path_series[i - 1][shortest_path_series[i - 1].size() - 2], shortest_path_series[i][0], shortest_path_series[i][1]));
        }
        for (int i = 0; i < turn_to_v.size(); i++)
            cout << turn_to_v[i];
        cout << endl;
    }
    show_map_BFS(map, shortest_path_series[8]);
    return shortest_path_series;
}