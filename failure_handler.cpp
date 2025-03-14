#include "failure_handler.h"
#include <iostream>
#include <vector>

FailureHandler::FailureHandler(Node& node) : node(node) {}

// void FailureHandler::detectFailure(int nodeId) {
//     std::cout << "Detecting failure for node " << nodeId << std::endl;
//     // Implement failure detection logic
// }

// void FailureHandler::handleFailure(int nodeId) {
//     std::cout << "Handling failure for node " << nodeId << std::endl;
//     // Implement failure handling logic
// }

// void FailureHandler::checkAndHandleCongestion(int nodeId) {
//     std::cout << "Checking and handling congestion for node " << nodeId << std::endl;
//     // Implement congestion handling logic
// }
bool FailureHandler::detectFailure(int destination) {
    // Detect failure via heartbeats or ACKs (simplified for demonstration)
    // In real-world scenarios, this could be based on timeout or missing acknowledgment.
    // Here we simulate failure by checking if the primary path exists for a destination.

    Node::Path primaryPath = node.getPrimaryPath(destination);
    if (primaryPath.nodes.empty()) {
        std::cout << "Failure detected for destination " << destination << std::endl;
        return true;  // Failure detected if no primary path exists
    }
    
    return false;  // No failure detected
}

void FailureHandler::handleFailure(int destination) {
    // Upon detecting failure, attempt to reroute the flow using backup paths
    Node::Path backupPath = node.reRoute(destination);

    if (!backupPath.nodes.empty()) {
        std::cout << "Rerouting to backup path for destination " << destination << std::endl;
        // Reroute the flow to the backup path to avoid packet loss
        node.addPrimaryPath(destination, backupPath);  // Make the backup path the new primary path
    } else {
        std::cout << "No alternative path available for destination " << destination << std::endl;
        // Handle the case where no backup path exists (e.g., packet loss or failure to find a path)
    }
}

void FailureHandler::checkAndHandleCongestion(int src,int destination) {
    // Check the link usage for the path to the destination
    Node::Path primaryPath = node.getPrimaryPath(destination);
    
    if (primaryPath.nodes.empty()) {
        std::cout << "No primary path found for destination " << destination << std::endl;
        return;
    }

    // Calculate the usage of the path and check for congestion
    double pathUsage = 0.0;
    for (int nodeId : primaryPath.nodes) {
        // Assuming higher usage means higher congestion
        pathUsage += node.getEdgeUsage(nodeId);  // Accumulate usage
    }

    // Calculate the average usage of the path
    pathUsage /= primaryPath.nodes.size();  // Divide by the number of edges in the path to get average usage

    if (pathUsage >= 0.7) {  // Threshold for high congestion (>= 70%)
        std::cout << "Congestion detected on path to destination " << destination << " with usage: " << pathUsage * 100 << "%" << std::endl;
        // Try to find an alternative path if the primary path is congested
        //Node::Path alternativePath = Node::PathPlanner.findAlternativePath(src,destination);

        // if (!alternativePath.nodes.empty()) {
        //     std::cout << "Redirecting flow to alternative path due to congestion" << std::endl;
        //     node.addPrimaryPath(destination, alternativePath);  // Make the alternative path the new primary path
        // } else {
        //     std::cout << "No alternative path available due to congestion" << std::endl;
        // }
    } else {
        std::cout << "No congestion detected on path to destination " << destination << " with usage: " << pathUsage * 100 << "%" << std::endl;
    }
}