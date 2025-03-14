#include "packet_forwarder.h"
#include <iostream>
#include <vector>
PacketForwarder::PacketForwarder(Node& node) : node(node) {}

void PacketForwarder::forwardPacket(int destination, int flowId) {
    Node::Path primaryPath = node.getPrimaryPath(destination);

    if (!primaryPath.nodes.empty()) {
        std::cout << "Forwarding packet through primary path to destination " << destination << std::endl;
        int nextHop = primaryPath.nodes.front();
        node.updateEdgeUsage(nextHop);
    } else {
        std::cout << "Primary path not available. Checking backup path..." << std::endl;
        Node::Path backupPath = node.getBackupPath(destination);

        if (!backupPath.nodes.empty()) {
            std::cout << "Forwarding packet through backup path to destination " << destination << std::endl;
            int nextHop = backupPath.nodes.front();
            node.updateEdgeUsage(nextHop);
        } else {
            std::cout << "No path available for destination " << destination << std::endl;
        }
    }
}

void PacketForwarder::updateEntropy() {
    std::vector<Node::Path> paths = node.getAllPaths();
    node.adjustPathWeightsBasedOnEntropy(paths);
}

void PacketForwarder::simulatePacketForwarding(int destination, int flowId) {
    forwardPacket(destination, flowId);
    updateEntropy();
}
