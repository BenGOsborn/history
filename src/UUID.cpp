#include "UUID.hpp"
#include <algorithm>

namespace UUID
{
    UUID::UUID(const std::vector<Node::Node> &nodes)
    {
        int mx = -1;
        for (auto const &node : nodes)
        {
            mx = std::max(mx, node.id);
        }
        id_ = mx + 1;
    }

    int UUID::generateID()
    {
        auto out = id_;
        id_ += 1;
        return out;
    }
}