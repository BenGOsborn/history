#include "Graph.hpp"
#include "Node.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <set>

namespace Graph
{
    std::ostream &operator<<(std::ostream &os, const DisplayNode &displayNode)
    {
        return os << "DisplayNode(" << displayNode.x << "," << displayNode.y << "," << displayNode.node << ")";
    }

    std::ostream &operator<<(std::ostream &os, const DisplayNodes &displayNodes)
    {
        int height = 0;
        int width = 0;
        for (auto const &displayNode : displayNodes)
        {
            height = std::max(height, displayNode.y);
            width = std::max(width, displayNode.x);
        }
        // Because the coordinates are 0-indexed
        height += 1;
        width += 1;
        std::vector<std::vector<std::string>> grid(height, std::vector<std::string>(width, " "));
        size_t colWidth = 0;
        for (auto const &displayNode : displayNodes)
        {
            std::string val = "(" + std::to_string(displayNode.node.id) + ") " + displayNode.node.name;
            grid[displayNode.y][displayNode.x] = val;
            colWidth = std::max(colWidth, val.size());
        }
        for (auto const &row : grid)
        {
            for (auto const &col : row)
            {
                os << std::left << std::setw(colWidth + 1) << col;
            }
            os << std::endl;
        }
        return os;
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

    void Graph::findRelationshipRecurse(const GraphNode *graphNode, std::set<int> &seen, int &x, const int y, DisplayNodes &out, Relationship relationship) const
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

    DisplayNodes Graph::findRelationship(const std::vector<Node::Node> &nodes, const Relationship relationship) const
    {
        std::set<int> seen;
        DisplayNodes out;
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

    DisplayNodes Graph::findAncestors(const std::vector<Node::Node> &nodes) const
    {
        return findRelationship(nodes, Relationship::Parent);
    }

    DisplayNodes Graph::findDescendents(const std::vector<Node::Node> &nodes) const
    {
        return findRelationship(nodes, Relationship::Child);
    }
}