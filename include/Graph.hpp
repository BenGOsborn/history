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
        virtual std::vector<Node::Node> findAncestors(const std::vector<Node::Node> &nodes) const = 0;
        virtual std::vector<Node::Node> findDescendents(const std::vector<Node::Node> &nodes) const = 0;
    };

    class Graph : public IGraph
    {
        struct GraphNode
        {
            Node::Node node;
            std::vector<GraphNode *> parents;
            std::vector<GraphNode *> children;
        };

        std::map<int, GraphNode> nodes_;
        int maxNodeID_;

    public:
        explicit Graph(const std::vector<Node::Node> &nodes);
        ~Graph() = default;
        std::vector<Node::Node> getNodes() const override;
        std::vector<Node::Node> findAncestors(const std::vector<Node::Node> &nodes) const override;
        std::vector<Node::Node> findDescendents(const std::vector<Node::Node> &nodes) const override;
    };
}