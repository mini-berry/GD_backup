#ifndef __DIJKSTRA_HPP__
#define __DIJKSTRA_HPP__

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <array>
#include "BFS.hpp"

using namespace std;

namespace user
{
    vector<array<int, 2>> dijkstra(vector<vector<int>> &maze, vector<array<int, 2>> &treasure);
}

#endif
