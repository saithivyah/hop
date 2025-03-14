#include "packet_forwarder.h"
#include "failure_handler.h"
#include "node_routing.h"
#include <iostream>
#include <algorithm>

// int main() {
//     // Create nodes and graph setup
//     Node node1(1);
//     Node node2(2);
//     Node node3(3);

//     // Example path setup for nodes
//     Node::Path path1 = {{1, 2}, 1.0};
//     node1.addPrimaryPath(2, path1);
    
//     PacketForwarder forwarder(node1);

//     // Simulate packet forwarding
//     forwarder.simulatePacketForwarding(2, 1);

//     // Handle failures
//     FailureHandler failureHandler(node1);
//     failureHandler.checkAndHandleCongestion(1,2);
    
//     return 0;
// }

#include <iostream>
#include "node_routing.h"
#include "path_planner.h"
#include "failure_handler.h"

int main() {
    Graph graph(5);
    graph.addEdge(0, 1, 10);
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 15);
    graph.addEdge(3, 4, 5);
    graph.addEdge(4, 0, 20);
    

    PathPlanner pathPlanner(graph, 0.5, 0.1);
    

    // Create nodes for the mesh topology
    Node node1(1, pathPlanner);
    Node node2(2, pathPlanner);
    Node node3(3, pathPlanner);

    Node::Path path1 = {{1, 2}, 1.0};
    node1.addPrimaryPath(2, path1);
    Node::Path path2 = {{2,3}, 1.0};
    node2.addPrimaryPath(3, path2);
    node3.addPrimaryPath(1, {{3, 1},1.0});

    // Create failure handler instances for nodes
    FailureHandler failureHandler1(node1);
    FailureHandler failureHandler2(node2);
    FailureHandler failureHandler3(node3);

    // Simulate failure and congestion for destination node 2
    int destination = 2;
    int src =1;

    std::cout << "Simulating failure detection and congestion handling for destination " << destination << "..." << std::endl;

    // Detect failure and reroute if necessary
    if (failureHandler1.detectFailure(destination)) {
        failureHandler1.handleFailure(destination);
    }

    // Check and handle congestion (70% threshold)
    failureHandler1.checkAndHandleCongestion(src,destination);

    // Simulate failure and congestion for destination node 3
    destination = 3;

    std::cout << "\nSimulating failure detection and congestion handling for destination " << destination << "..." << std::endl;

    if (failureHandler2.detectFailure(destination)) {
        failureHandler2.handleFailure(destination);
    }

    failureHandler2.checkAndHandleCongestion(src,destination);

    return 0;
}