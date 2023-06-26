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

int user::dijkstra(vector<vector<int>> maze)
{
    // 创建一个示例图
    Graph graph;
    graph.numVertices = 6;
    graph.adjList.resize(graph.numVertices);

    // 添加边
    graph.adjList[0].push_back({1, 2});
    graph.adjList[0].push_back({2, 4});
    graph.adjList[1].push_back({2, 1});
    graph.adjList[1].push_back({3, 7});
    graph.adjList[2].push_back({3, 3});
    graph.adjList[2].push_back({4, 2});
    graph.adjList[3].push_back({4, 5});
    graph.adjList[4].push_back({5, 3});

    int source = 0;
    vector<int> shortestDistances = dijkstra_path(graph, source);

    // 打印最短距离
    for (int i = 0; i < graph.numVertices; ++i)
    {
        cout << "Shortest distance from node " << source << " to " << i << " is " << shortestDistances[i] << endl;
    }

    return 0;
}
