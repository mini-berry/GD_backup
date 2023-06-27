#ifndef __BFS_HPP__
#define __BFS_HPP__

#include <iostream>
#include <queue>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

namespace user
{
    vector<int> BFS(vector<vector<int>> &map, vector<array<int, 2>> &treasure_pos);
}
#endif
