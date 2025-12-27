#include "Graph.hpp"
#include "Node.hpp"
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <set>
#include <cwchar>

namespace
{
    static const std::string PIPE_DOWN = "|";
    static const std::string PIPE_HORIZONTAL = "-";

    std::string padString(const std::string &str, const int &size, const std::string &token)
    {
        auto out = str;
        while (out.size() < size)
        {
            out += token;
        }
        return out;
    }
}

namespace
{
    static const int ROW_COUNT = 3;
    static const int PIPE_ROW_COUNT = 3;
    static const int PADDING = 3;
}

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
        height = ((height + 1) * ROW_COUNT) + (height * PIPE_ROW_COUNT);
        width += 1;
        std::vector<std::vector<std::string>> grid(height, std::vector<std::string>(width, " "));
        for (auto const &displayNode : displayNodes)
        {
            auto node = displayNode.node;
            std::string id = "ID = " + std::to_string(node.id);
            std::string name = "Name = " + node.name;
            std::tm tm = *std::localtime(&node.birth);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%d/%m/%Y");
            std::string date = oss.str();
            std::string dob = "Date of birth = " + date;
            int yOffset = displayNode.y * (ROW_COUNT + PIPE_ROW_COUNT);
            grid[yOffset][displayNode.x] = id;
            grid[yOffset + 1][displayNode.x] = name;
            grid[yOffset + 2][displayNode.x] = dob;
            if (displayNode.edges.size() == 0)
            {
                continue;
            }
            grid[yOffset + 3][displayNode.x] = PIPE_DOWN;
            if (displayNode.edges.size() == 1)
            {
                grid[yOffset + 4][displayNode.x] = PIPE_DOWN;
                grid[yOffset + 5][displayNode.x] = PIPE_DOWN;
            }
            auto endX = displayNode.x;
            for (int i = 0; i < displayNode.edges.size(); i++)
            {
                auto &edge = displayNode.edges[i];
                auto val = PIPE_HORIZONTAL;
                if (i == displayNode.edges.size() - 1)
                {
                    val = PIPE_DOWN;
                }
                grid[yOffset + 4][edge.x] = val;
                grid[yOffset + 5][edge.x] = PIPE_DOWN;
                endX = std::max(endX, edge.x);
            }
            for (int x = displayNode.x; x < endX + 1; x++)
            {
                auto val = grid[yOffset + 4][x];
                if (val == " ")
                {
                    grid[yOffset + 4][x] = PIPE_HORIZONTAL;
                }
            }
        }
        size_t colWidth = 0;
        for (auto const &row : grid)
        {
            for (auto const &col : row)
            {
                colWidth = std::max(colWidth, col.size());
            }
        }
        for (auto const &row : grid)
        {
            for (auto const &col : row)
            {
                std::string token = " ";
                if (col == PIPE_HORIZONTAL)
                {
                    token = PIPE_HORIZONTAL;
                }
                os << padString(col, colWidth + PADDING, token);
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

    DisplayNode *Graph::findRelationshipRecurse(const GraphNode *graphNode, std::set<int> &seen, int &x, const int y, DisplayNodes &out, Relationship relationship) const
    {
        if (graphNode == nullptr)
        {
            throw std::runtime_error("encounted nullptr node");
        }
        auto node = graphNode->node;
        if (seen.find(node.id) != seen.end())
        {
            return nullptr;
        }
        seen.insert(node.id);
        auto startX = x;
        std::vector<GraphNode *> relationships;
        auto it = graphNode->relationships.find(relationship);
        if (it != graphNode->relationships.end())
        {
            relationships = it->second;
        }
        std::vector<DisplayNode> edges;
        for (auto const &relation : relationships)
        {
            auto edge = findRelationshipRecurse(relation, seen, x, y + 1, out, relationship);
            if (edge != nullptr)
            {
                edges.push_back(*edge);
            }
        }
        out.emplace_back(node, startX, y, edges);
        if (edges.size() == 0)
        {
            x++;
        }
        return &out.back();
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