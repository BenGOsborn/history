#pragma once
#include "Node.hpp"

struct GraphNode
{
    Node *node;
    std::vector<Node *> parents;
    std::vector<Node *> children;
};