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
    };

    std::ostream &operator<<(std::ostream &os, const DisplayNode &displayNode);

    class IGraph
    {
    public:
        virtual ~IGraph() = default;
        virtual std::vector<Node::Node> getNodes() const = 0;
        virtual std::vector<DisplayNode> findAncestors(const std::vector<Node::Node> &nodes) const = 0;
        virtual std::vector<DisplayNode> findDescendents(const std::vector<Node::Node> &nodes) const = 0;
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

        void findRelationshipRecurse(const GraphNode *graphNode, std::set<int> &seen, int &x, const int y, std::vector<DisplayNode> &out, const Relationship relationship) const;
        std::vector<DisplayNode> findRelationship(const std::vector<Node::Node> &nodes, const Relationship relationship) const;

    public:
        explicit Graph(const std::vector<Node::Node> &nodes);
        ~Graph() = default;
        std::vector<Node::Node> getNodes() const override;
        std::vector<DisplayNode> findAncestors(const std::vector<Node::Node> &nodes) const override;
        std::vector<DisplayNode> findDescendents(const std::vector<Node::Node> &nodes) const override;
    };
}