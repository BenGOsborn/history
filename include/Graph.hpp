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
        std::vector<DisplayNode> edges;
    };

    std::ostream &operator<<(std::ostream &os, const DisplayNode &displayNode);

    using DisplayNodes = std::vector<DisplayNode>;

    std::ostream &operator<<(std::ostream &os, const DisplayNodes &displayNodes);

    class Graph
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
        DisplayNodes findRelationship(const int &id, const Relationship relationship) const;

    public:
        explicit Graph(const std::vector<Node::Node> &nodes);
        ~Graph() = default;
        std::vector<Node::Node> getNodes() const;
        void loadNodes(const std::vector<Node::Node> &nodes);
        DisplayNodes findAncestors(const int &id) const;
        DisplayNodes findDescendents(const int &id) const;
        void addNode(const Node::Node &node);
        void removeNode(const int &id);
    };
}