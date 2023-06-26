#include "BFS.hpp"

// 定义迷宫的大小
const int ROWS = 21;
const int COLS = 21;

// 定义方向：上、下、左、右
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

// BFS算法
int shortestDistance(vector<vector<int>> &maze, pair<int, int> start, pair<int, int> end)
{
    // 创建队列用于BFS
    queue<pair<int, int>> q;
    q.push(start);

    // 创建visited数组用于记录节点是否已访问
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    visited[start.first][start.second] = true;

    // 创建距离数组用于记录每个节点到起始节点的距离
    vector<vector<int>> distance(ROWS, vector<int>(COLS, 0));

    while (!q.empty())
    {
        pair<int, int> current = q.front();
        q.pop();

        // 如果当前节点是目标节点，返回距离
        if (current == end)
        {
            return distance[current.first][current.second];
        }

        // 在四个方向上尝试移动
        for (int i = 0; i < 4; ++i)
        {
            int newRow = current.first + dx[i];
            int newCol = current.second + dy[i];

            // 检查新位置是否在迷宫范围内且没有访问过，并且新位置不是墙（值为1）
            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS && !visited[newRow][newCol] && maze[newRow][newCol] == 0)
            {
                q.push(make_pair(newRow, newCol));
                visited[newRow][newCol] = true;
                distance[newRow][newCol] = distance[current.first][current.second] + 1;
            }
        }
    }

    // 如果无法到达目标节点，返回-1表示没有最短路径
    return -1;
}

int user::BFS(vector<vector<int>> maze)
{
    pair<int, int> start = make_pair(19, 0);
    pair<int, int> end = make_pair(19, 3);

    int distance = shortestDistance(maze, start, end);

    if (distance == -1)
    {
        cout << "无法到达目标点" << endl;
    }
    else
    {
        cout << "最短距离为: " << distance << endl;
    }

    return 0;
}
