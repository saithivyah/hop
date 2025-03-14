#ifndef PATH_PLANNER_H
#define PATH_PLANNER_H

#include <vector>
#include <queue>
#include <limits>

class Graph {
public:
    struct Edge {
        int to;
        double weight;
    };

    Graph(int n);
    void addEdge(int u, int v, double weight);
    std::vector<Edge>& getAdj(int u);
    int getNodeCount() const;

private:
    std::vector<std::vector<Edge>> adj;
};

class PathPlanner {
public:
    PathPlanner(Graph& graph, double alpha, double lambda);

    void runDijkstra(int src, std::vector<double>& dist, std::vector<int>& prev);
    std::vector<int> reconstructPath(int src, int dst, const std::vector<int>& prev);
    std::vector<int> computePerturbedPath(int src, double perturbAlpha);
    std::vector<int> findPath(int src);
    std::vector<int> findAlternativePath(int src, int dest);

private:
    Graph& graph;
    double alpha;
    double lambda;

    double randInRange(double min, double max);
    std::vector<int> minimizeOverlap(const std::vector<int>& path1, const std::vector<int>& path2);
};

#endif // PATH_PLANNER_H
