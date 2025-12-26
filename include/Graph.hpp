#pragma once
#include "Node.hpp"
#include <vector>
#include <map>

namespace Graph
{
    class IGraph
    {
    public:
        virtual ~IGraph() = default;
        virtual std::vector<Node::Node> getNodes() const = 0;
        // TODO - add descendents and also return the values using levels which can then easily be rendered
        virtual std::vector<Node::Node> findAncestors(const std::vector<Node::Node> &children) const = 0;
    };

    class Graph : public IGraph
    {
        struct GraphNode
        {
            Node::Node *node;
            std::vector<GraphNode *> parents;
        };

        std::vector<Node::Node> nodes_;
        std::map<int, GraphNode> IDToNode_;

    public:
        explicit Graph(std::vector<Node::Node> nodes);
        ~Graph() = default;
        std::vector<Node::Node> getNodes() const override;
        std::vector<Node::Node> findAncestors(const std::vector<Node::Node> &children) const override;
    };
}