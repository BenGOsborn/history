#pragma once
#include "Node.hpp"

namespace Graph
{
    struct GraphNode
    {
        Node::Node *node;
        std::vector<Node::Node *> parents;
        std::vector<Node::Node *> children;
    };
}