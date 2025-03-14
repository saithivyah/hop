#ifndef PACKET_FORWARDER_H
#define PACKET_FORWARDER_H

#include <vector>
#include "node_routing.h"

class PacketForwarder {
public:
    PacketForwarder(Node& node);

    void forwardPacket(int destination, int flowId);
    void updateEntropy();
    void simulatePacketForwarding(int destination, int flowId);

private:
    Node& node;
};

#endif // PACKET_FORWARDER_H
