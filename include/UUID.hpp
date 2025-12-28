#pragma once
#include "Node.hpp"

namespace UUID
{
    class UUID
    {
        int id_;

    public:
        explicit UUID(const std::vector<Node::Node> &nodes);
        ~UUID() = default;
        int generateID();
    };
}
