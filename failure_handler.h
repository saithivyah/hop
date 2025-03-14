#ifndef FAILURE_HANDLER_H
#define FAILURE_HANDLER_H

#include "node_routing.h"
#include "path_planner.h"

class FailureHandler {
public:
    FailureHandler(Node& node);

    bool detectFailure(int nodeId);
    void handleFailure(int nodeId);
    void checkAndHandleCongestion(int src,int nodeId);

private:
    Node& node;
};

#endif // FAILURE_HANDLER_H
