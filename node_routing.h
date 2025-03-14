#ifndef NODE_ROUTING_H
#define NODE_ROUTING_H

#include <vector>
#include <unordered_map>
#include <list>
#include <map>
#include <limits>
#include <iostream>
#include "path_planner.h" 

class Node {
public:
    struct Path {
        std::vector<int> nodes;  // List of nodes in the path
        double weight;           // Weight of the path (can be adjusted)
    };
    using RoutingTable = std::unordered_map<int, Path>;  // Maps destination node to path
    using BackupPaths = std::unordered_map<int, Path>;  // Backup paths for each destination
    using EdgeUsage = std::unordered_map<int, int>;  // Maps edge ID to usage count

    Node(int id, PathPlanner& pathPlanner);

    // Methods to handle paths
    void addPrimaryPath(int destination, const Path& path);
    void addBackupPath(int destination, const Path& path);
    Path getPrimaryPath(int destination);
    Path getBackupPath(int destination);

    // Re-route to a backup path if the primary path is congested or fails
    Path reRoute(int destination);
    
    // Method to update edge usage (to simulate congestion)
    void updateEdgeUsage(int edgeId);

    // Calculate entropy based on the paths
    double calculateEntropy(const std::vector<Path>& paths);

    // Adjust the weights of paths based on entropy
    void adjustPathWeightsBasedOnEntropy(std::vector<Path>& paths);

    //Method to get the edge usage 
    double getEdgeUsage(int edgeID);

    // // Getter for the graph
    // Graph* getGraph();  // Returns the graph associated with the node


    // Helper methods
    int getId() const;
    std::vector<int> getRoutingTable();
    std::vector<int> getBackupPaths();
    std::vector<Path> getAllPaths();

private:
    int id;
    std::unordered_map<int, Path> primaryPaths;
    std::unordered_map<int, Path> backupPaths;
    PathPlanner& pathPlanner;
    std::unordered_map<int, int> edgeUsage;  // Edge usage tracking

    double calculateEntropyForPath(const Path& path);
    
};

#endif // NODE_ROUTING_H
