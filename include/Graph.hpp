#pragma once
#include "Node.hpp"
#include <vector>
#include <map>
#include <set>

namespace Graph
{
    struct DisplayNode
    {
        Node::Node node;
        int x;
        int y;
        std::vector<DisplayNode *> edges;
    };

    std::ostream &operator<<(std::ostream &os, const DisplayNode &displayNode);

    using DisplayNodes = std::vector<DisplayNode>;

    std::ostream &operator<<(std::ostream &os, const DisplayNodes &displayNodes);

    class IGraph
    {
    public:
        virtual ~IGraph() = default;
        virtual std::vector<Node::Node> getNodes() const = 0;
        virtual DisplayNodes findAncestors(const std::vector<Node::Node> &nodes) const = 0;
        virtual DisplayNodes findDescendents(const std::vector<Node::Node> &nodes) const = 0;
    };

    class Graph : public IGraph
    {
        enum Relationship
        {
            Parent,
            Child,
        };

        struct GraphNode
        {
            Node::Node node;
            std::map<Relationship, std::vector<GraphNode *>> relationships;
        };

        std::map<int, GraphNode> nodes_;

        DisplayNode *findRelationshipRecurse(const GraphNode *graphNode, std::set<int> &seen, int &x, const int y, std::vector<DisplayNode> &out, const Relationship relationship) const;
        DisplayNodes findRelationship(const std::vector<Node::Node> &nodes, const Relationship relationship) const;

    public:
        explicit Graph(const std::vector<Node::Node> &nodes);
        ~Graph() = default;
        std::vector<Node::Node> getNodes() const override;
        DisplayNodes findAncestors(const std::vector<Node::Node> &nodes) const override;
        DisplayNodes findDescendents(const std::vector<Node::Node> &nodes) const override;
    };
}