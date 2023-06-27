#include "BFS.hpp"

struct Point {
    int row;
    int col;

    Point(int r, int c) : row(r), col(c) {}
};

bool isValid(int row, int col, int numRows, int numCols, const std::vector<std::vector<char>>& maze) {
    return row >= 0 && row < numRows && col >= 0 && col < numCols && maze[row][col] != '#';
}

int findShortestPath(const std::vector<std::vector<char>>& maze, const std::vector<Point>& targets, Point start) {
    int numRows = maze.size();
    int numCols = maze[0].size();
    int numTargets = targets.size();

    std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));

    std::queue<Point> queue;
    queue.push(start);
    visited[start.row][start.col] = true;

    int steps = 0;
    int targetsFound = 0;

    while (!queue.empty()) {
        int size = queue.size();

        for (int i = 0; i < size; ++i) {
            Point curr = queue.front();
            queue.pop();

            if (maze[curr.row][curr.col] == 'T') {
                targetsFound++;
                if (targetsFound == numTargets) {
                    return steps;
                }
            }

            // 尝试四个方向的移动：上、下、左、右
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};

            for (int j = 0; j < 4; ++j) {
                int newRow = curr.row + dr[j];
                int newCol = curr.col + dc[j];

                if (isValid(newRow, newCol, numRows, numCols, maze) && !visited[newRow][newCol]) {
                    queue.push(Point(newRow, newCol));
                    visited[newRow][newCol] = true;
                }
            }
        }

        steps++;
    }

    // 如果无法找到所有目标点，则返回一个特殊值表示无解
    return -1;
}

int user::BFS() {
    std::vector<std::vector<char>> maze = {
        {'S', '.', '#', '#', '.', '.'},
        {'.', '.', '.', '#', '.', '#'},
        {'.', '#', '.', '.', '.', '#'},
        {'.', '#', '#', '#', '.', '.'},
        {'.', '#', 'T', '.', '#', '.'},
    };

    std::vector<Point> targets = {Point(4, 2)};
    Point start(0, 0);

    int shortestPathLength = findShortestPath(maze, targets, start);

    if (shortestPathLength != -1) {
        std::cout << "Shortest path length: " << shortestPathLength << std::endl;
    } else {
        std::cout << "No path found!" << std::endl;
    }

    return 0;
}
