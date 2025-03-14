#include "node_routing.h"
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <vector>

Node::Node(int id, PathPlanner& pathPlanner) 
    : id(id), pathPlanner(pathPlanner) {}

void Node::addPrimaryPath(int destination, const Path& path) {
    primaryPaths[destination] = path;
}

void Node::addBackupPath(int destination, const Path& path) {
    backupPaths[destination] = path;
}

Node::Path Node::getPrimaryPath(int destination) {
    return primaryPaths[destination];
}

Node::Path Node::getBackupPath(int destination) {
    return backupPaths[destination];
}

Node::Path Node::reRoute(int destination) {
    // Try to use the backup path if available
    Path backupPath = getBackupPath(destination);
    if (!backupPath.nodes.empty()) {
        std::cout << "Rerouting to backup path for destination " << destination << std::endl;
        return backupPath;
    } else {
        std::cout << "No backup path available for destination " << destination << std::endl;
        return getPrimaryPath(destination);
    }
}

void Node::updateEdgeUsage(int edgeId) {
    edgeUsage[edgeId]++;
}

double Node::getEdgeUsage(int edgeID) {
    return edgeUsage[edgeID];
}

double Node::calculateEntropyForPath(const Path& path) {
    std::unordered_map<int, int> nodeFrequency;
    for (int node : path.nodes) {
        nodeFrequency[node]++;
    }

    double entropy = 0.0;
    int totalNodes = path.nodes.size();
    for (const auto& entry : nodeFrequency) {
        double p = static_cast<double>(entry.second) / totalNodes;
        entropy -= p * log(p);
    }

    return entropy;
}

double Node::calculateEntropy(const std::vector<Path>& paths) {
    double totalEntropy = 0.0;

    for (const auto& path : paths) {
        totalEntropy += calculateEntropyForPath(path);
    }

    return totalEntropy / paths.size();
}

void Node::adjustPathWeightsBasedOnEntropy(std::vector<Path>& paths) {
    for (auto& path : paths) {
        double entropy = calculateEntropyForPath(path);

        // Adjust the weight based on entropy
        if (entropy > 1.0) {
            path.weight *= 0.9;  // Reduce weight for congested paths
        } else {
            path.weight *= 1.1;  // Increase weight for less congested paths
        }
    }
}

std::vector<Node::Path> Node::getAllPaths() {
    std::vector<Path> paths;
    for (const auto& entry : primaryPaths) {
        paths.push_back(entry.second);
    }
    for (const auto& entry : backupPaths) {
        paths.push_back(entry.second);
    }
    return paths;
}

int Node::getId() const {
    return id;
}

// std::vector<int> Node::getRoutingTable() {
//     std::vector<int> routingTable;
//     std::cout << "Routing Table for Node " << id << ":\n";
//     for (const auto& entry : primaryPaths) {
//         std::cout << "Destination: " << entry.first << " -> Path: ";
//         for (int nodeId : entry.second) {
//             std::cout << nodeId << " ";
//         }
//         std::cout << std::endl;
//     }
// }

std::vector<int> Node::getRoutingTable() {
    std::vector<int> routingTable;
    
    // Add primary path destinations
    for (const auto& entry : primaryPaths) {
        routingTable.push_back(entry.first);  // Add the destination from primaryPaths
    }
    
    // Add backup path destinations
    for (const auto& entry : backupPaths) {
        routingTable.push_back(entry.first);  // Add the destination from backupPaths
    }

    // Remove duplicates (in case a destination exists in both primary and backup)
    // std::sort(routingTable.begin(), routingTable.end());
    // routingTable.erase(std::unique(routingTable.begin(), routingTable.end()), routingTable.end());

    return routingTable;
}
// Graph* Node::getGraph() {
//     return &graph;  // Return the address of the graph object
// }

std::vector<int> Node::getBackupPaths() {
    std::cout << "Backup Paths for Node " << id << ":\n";
    for (const auto& entry : backupPaths) {
        std::cout << "Destination: " << entry.first << " -> Backup Path: ";
        for (int nodeId : entry.second.nodes) {
            std::cout << nodeId << " ";
        }
        std::cout << std::endl;
    }
   
}
