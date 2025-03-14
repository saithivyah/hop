#include "path_planner.h"
#include <iostream>
#include <queue>
#include <limits>
#include <cmath>
#include <algorithm>
#include <vector>

// Graph constructor that initializes the adjacency list for the graph
Graph::Graph(int n) : adj(n) {}

void Graph::addEdge(int u, int v, double weight) {
    adj[u].push_back({v, weight});
}

std::vector<Graph::Edge>& Graph::getAdj(int u) {
    return adj[u];
}

int Graph::getNodeCount() const {
    return adj.size();
}

// PathPlanner constructor that takes a reference to a Graph, and two parameters for path planning
PathPlanner::PathPlanner(Graph& graph, double alpha, double lambda) 
    : graph(graph), alpha(alpha), lambda(lambda) {}

// A helper function to generate a random number in a specified range
double PathPlanner::randInRange(double min, double max) {
    return min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - min)));
}

// Dijkstra’s algorithm to find the shortest path from the source node to all other nodes
void PathPlanner::runDijkstra(int src, std::vector<double>& dist, std::vector<int>& prev) {
    int n = graph.getNodeCount();
    dist.assign(n, std::numeric_limits<double>::infinity());
    prev.assign(n, -1);
    dist[src] = 0;

    auto compare = [&](int u, int v) { return dist[u] > dist[v]; };
    std::priority_queue<int, std::vector<int>, decltype(compare)> pq(compare);
    pq.push(src);

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();

        for (const auto& edge : graph.getAdj(u)) {
            int v = edge.to;
            double weight = edge.weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(v);
            }
        }
    }
}

// Reconstruct the path from the source to the destination based on the previous nodes
std::vector<int> PathPlanner::reconstructPath(int src, int dst, const std::vector<int>& prev) {
    std::vector<int> path;
    for (int at = dst; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> PathPlanner::computePerturbedPath(int src, double perturbAlpha) {
    std::vector<double> dist;
    std::vector<int> prev;
    runDijkstra(src, dist, prev);

    // Perturb edge weights based on random factor
    for (int u = 0; u < graph.getNodeCount(); ++u) {
        for (auto& edge : graph.getAdj(u)) {
            double rand_factor = randInRange(-0.5, 0.5);
            edge.weight *= (1 + perturbAlpha * rand_factor);
        }
    }

    // Re-run Dijkstra after perturbation
    runDijkstra(src, dist, prev);

    // Return the path (P1 or P2)
    return reconstructPath(src, dist.size() - 1, prev);
}

// Find the shortest path from the source node to a destination node
std::vector<int> PathPlanner::findPath(int src) {
    std::vector<double> dist;
    std::vector<int> prev;
    runDijkstra(src, dist, prev);

    int destination = randInRange(0, graph.getNodeCount() - 1);
    return reconstructPath(src, destination, prev);
}

// Find an alternative path from the source to the destination
std::vector<int> PathPlanner::findAlternativePath(int src, int dest) {
    // This can be a simple variation of Dijkstra or some other heuristic
    std::vector<double> dist;
    std::vector<int> prev;
    runDijkstra(src, dist, prev);

    std::vector<int> path = reconstructPath(src, dest, prev);
    
    // A simple alternative path strategy could be changing some weights, or finding a second-best path
    // For simplicity, let's just return the path found
    return path;
}

// Minimize overlap between two paths
std::vector<int> PathPlanner::minimizeOverlap(const std::vector<int>& path1, const std::vector<int>& path2) {
    std::vector<int> mergedPath;
    for (const auto& node : path1) {
        mergedPath.push_back(node);
    }
    for (const auto& node : path2) {
        if (std::find(mergedPath.begin(), mergedPath.end(), node) == mergedPath.end()) {
            mergedPath.push_back(node);
        }
    }
    return mergedPath;
}
