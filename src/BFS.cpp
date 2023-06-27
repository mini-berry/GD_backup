#include "BFS.hpp"

bool isValid(int x, int y, int rows, int cols, const std::vector<std::vector<int>> &map)
{
    return x >= 0 && x < rows && y >= 0 && y < cols && map[x][y] == 0;
}

int length(int start, int end, vector<vector<int>> map, vector<array<int, 2>> treasure_pos)
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

vector<int> user::BFS(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos)
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
    graph[0][treasure_pos.size() - 1] = -1;
    graph[treasure_pos.size() - 1][0] = -1;
    vector<int> numbers;
    for (int i = 1; i < treasure_pos.size() - 1; i++)
    {
        numbers.push_back(i);
    }
    sort(numbers.begin(), numbers.end());
    // 输出初始排列
    int min_length = 0;
    int times = 0;
    vector<int> min_order;
    
    do
    {
        vector<int> order;
        for (int number : numbers)
        {
            order.push_back(number);
        }
        // 0到1
        int sum_length = graph[0][order[0]];
        for (int i = 0; i < numbers.size() - 1; i++)
        {
            sum_length += graph[order[i]][order[i + 1]];
        }
        sum_length += graph[order[numbers.size() - 1]][treasure_pos.size() - 1];
        if (min_length == 0)
        {
            min_length = sum_length;
            min_order = order;
        }
        if (min_length > sum_length)
        {
            min_length = sum_length;
            min_order = order;
        }
        if (times++ > 600000)
            break;
    } while (next_permutation(numbers.begin(), numbers.end()));
    cout << "最短路径为:" << min_length << " ";
    cout << "循环次数:" << times << endl;
    for (int i = 0; i < min_order.size(); i++)
        cout << min_order[i] << " ";
    return min_order;
}