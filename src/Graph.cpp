#include "Graph.hpp"
#include "Node.hpp"
#include <iostream>
#include <utility>
#include <set>

namespace Graph
{
    std::ostream &operator<<(std::ostream &os, const DisplayNode &displayNode)
    {
        return os << "DisplayNode(" << displayNode.x << "," << displayNode.y << "," << displayNode.node << ")";
    }

    Graph::Graph(const std::vector<Node::Node> &nodes)
    {
        for (auto const &node : nodes)
        {
            GraphNode graphNode{node};
            nodes_[node.id] = graphNode;
        }
        for (auto const &node : nodes)
        {
            GraphNode &parentNode = nodes_[node.id];
            for (auto &child : node.children)
            {
                auto it = nodes_.find(child);
                if (it == nodes_.end())
                {
                    throw std::runtime_error("invalid child");
                }
                GraphNode &childNode = nodes_[child];
                childNode.relationships[Relationship::Parent].push_back(&nodes_[node.id]);
                parentNode.relationships[Relationship::Child].push_back(&nodes_[child]);
            }
        }
    }

    std::vector<Node::Node> Graph::getNodes() const
    {
        std::vector<Node::Node> out;
        for (auto const &[key, val] : nodes_)
        {
            out.push_back(val.node);
        }
        return out;
    }

    void Graph::findRelationshipRecurse(const GraphNode *graphNode, std::set<int> &seen, int &x, const int y, std::vector<DisplayNode> &out, Relationship relationship) const
    {
        if (graphNode == nullptr)
        {
            throw std::runtime_error("encounted nullptr node");
        }
        auto node = graphNode->node;
        if (seen.find(node.id) != seen.end())
        {
            return;
        }
        seen.insert(node.id);
        auto startX = x;
        std::vector<GraphNode *> relationships;
        auto it = graphNode->relationships.find(relationship);
        if (it != graphNode->relationships.end())
        {
            relationships = it->second;
        }
        for (auto const &relation : relationships)
        {
            findRelationshipRecurse(relation, seen, x, y + 1, out, relationship);
        }
        DisplayNode displayNode{node, startX, y};
        out.push_back(displayNode);
        if (relationships.size() == 0)
        {
            x++;
        }
    }

    std::vector<DisplayNode> Graph::findRelationship(const std::vector<Node::Node> &nodes, const Relationship relationship) const
    {
        std::set<int> seen;
        std::vector<DisplayNode> out;
        int x = 0;
        for (auto const &node : nodes)
        {
            auto it = nodes_.find(node.id);
            if (it == nodes_.end())
            {
                throw std::runtime_error("node does not exist");
            }
            findRelationshipRecurse(&it->second, seen, x, 0, out, relationship);
        }
        return out;
    }

    std::vector<DisplayNode> Graph::findAncestors(const std::vector<Node::Node> &nodes) const
    {
        return findRelationship(nodes, Relationship::Parent);
    }

    std::vector<DisplayNode> Graph::findDescendents(const std::vector<Node::Node> &nodes) const
    {
        return findRelationship(nodes, Relationship::Child);
    }
}