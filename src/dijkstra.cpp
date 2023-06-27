#include "dijkstra.hpp"

// 边的结构体
struct Edge
{
    int destination;
    int weight;
};

// 图的结构体
struct Graph
{
    int numVertices;
    vector<vector<Edge>> adjList;
};

// Dijkstra算法
vector<int> dijkstra_path(const Graph &graph, int source)
{
    vector<int> distances(graph.numVertices, INT_MAX);
    distances[source] = 0;

    // 优先队列用于选择下一个距离最小的节点
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, source));

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        for (const Edge &edge : graph.adjList[u])
        {
            int v = edge.destination;
            int weight = edge.weight;

            if (distances[u] != INT_MAX && distances[u] + weight < distances[v])
            {
                distances[v] = distances[u] + weight;
                pq.push(make_pair(distances[v], v));
            }
        }
    }

    return distances;
}

vector<array<int, 2>> user::dijkstra(vector<vector<int>> &maze, vector<array<int, 2>> &treasure)
{
    // 创建一个示例图
    Graph graph;
    graph.numVertices = treasure.size() + 2;

    graph.adjList.resize(graph.numVertices);
    cout << treasure.size();
    // 添加边
    for (int i = 0; i < treasure.size() + 1; i++)
    {
        if (i == 0)
            for (int j = 1; j < treasure.size() + 1; j++)
                graph.adjList[0].push_back({j, 1});
        else
        {
            if (i == treasure.size() + 1)
                for (int j = 1; j < treasure.size() + 1; j++)
                    graph.adjList[treasure.size() + 1].push_back({j, 1});
            else
            {
                for (int j = 1; j < treasure.size() + 1; j++)
                    if (j != i)
                        graph.adjList[i].push_back({j, 1});
            }
        }
    }

    int source = 0;
    vector<int> shortestDistances = dijkstra_path(graph, source);

    // 打印最短距离
    for (int i = 0; i < graph.numVertices; ++i)
    {
        cout << "Shortest distance from node " << source << " to " << i << " is " << shortestDistances[i] << endl;
    }
    return treasure;
}
