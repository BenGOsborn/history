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
    static const int ROW_COUNT = 3;
    static const int PIPE_ROW_COUNT = 3;
    static const int PADDING = 3;
    static const std::string EMPTY = " ";
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

    enum TileType
    {
        Empty,
        NoRepeat,
        Repeat,
    };

    struct Tile
    {
        TileType tileType;
        std::string val;
    };

    std::string render(const std::vector<std::vector<Tile>> &grid)
    {
        size_t colWidth = 1;
        for (auto const &row : grid)
        {
            for (auto const &col : row)
            {
                colWidth = std::max(colWidth, col.val.size());
            }
        }
        std::string out;
        for (int i = 0; i < grid.size(); i++)
        {
            auto &row = grid[i];
            for (auto const &col : row)
            {
                int padSize = colWidth + PADDING;
                if (col.tileType == TileType::Repeat)
                {
                    out += padString(col.val, padSize, col.val);
                    continue;
                }
                out += padString(col.val, colWidth + PADDING, EMPTY);
            }
            if (i == grid.size() - 1)
            {
                continue;
            }
            out += '\n';
        }
        return out;
    }

    std::string parseDOB(const time_t &time)
    {
        std::tm tm = *std::localtime(&time);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d/%m/%Y");
        std::string date = oss.str();
        return date;
    }
}

namespace Graph
{
    std::ostream &operator<<(std::ostream &os, const DisplayNode &displayNode)
    {
        return os << "DisplayNode(" << displayNode.x << "," << displayNode.y << "," << displayNode.node << ")";
    }

    void drawEdges(std::vector<std::vector<Tile>> &grid, const std::vector<DisplayNode> &edges, const int &x, const int &edgeOffset)
    {
        if (edges.size() == 0)
        {
            return;
        }
        grid[edgeOffset][x] = Tile{TileType::NoRepeat, PIPE_DOWN};
        if (edges.size() == 1)
        {
            grid[edgeOffset + 1][x] = Tile{TileType::NoRepeat, PIPE_DOWN};
            grid[edgeOffset + 2][x] = Tile{TileType::NoRepeat, PIPE_DOWN};
            return;
        }
        auto endX = x;
        for (int i = 0; i < edges.size(); i++)
        {
            auto &edge = edges[i];
            Tile tile{TileType::Repeat, PIPE_HORIZONTAL};
            if (i == edges.size() - 1)
            {
                tile = Tile{TileType::NoRepeat, PIPE_DOWN};
            }
            grid[edgeOffset + 1][edge.x] = tile;
            grid[edgeOffset + 2][edge.x] = Tile{TileType::NoRepeat, PIPE_DOWN};
            endX = std::max(endX, edge.x);
        }
        for (int i = x; i < endX + 1; i++)
        {
            auto val = grid[edgeOffset + 1][i];
            if (val.tileType == TileType::Empty)
            {
                grid[edgeOffset + 1][i] = Tile{TileType::Repeat, PIPE_HORIZONTAL};
            }
        }
    }

    void drawNode(std::vector<std::vector<Tile>> &grid, const DisplayNode &displayNode, const int &rowSize)
    {
        auto node = displayNode.node;
        std::string id = "ID = " + std::to_string(node.id);
        std::string name = "Name = " + node.name;
        std::string dob = "Date of birth = " + parseDOB(node.birth);
        int yOffset = displayNode.y * rowSize;
        grid[yOffset][displayNode.x] = Tile{TileType::NoRepeat, id};
        grid[yOffset + 1][displayNode.x] = Tile{TileType::NoRepeat, name};
        grid[yOffset + 2][displayNode.x] = Tile{TileType::NoRepeat, dob};
        drawEdges(grid, displayNode.edges, displayNode.x, yOffset + 3);
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
        int rowSize = ROW_COUNT + PIPE_ROW_COUNT;
        std::vector<std::vector<Tile>> grid(height, std::vector<Tile>(width, {TileType::Empty, EMPTY}));
        for (auto const &displayNode : displayNodes)
        {
            drawNode(grid, displayNode, rowSize);
        }
        auto out = render(grid);
        return os << out;
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
            GraphNode &graphNode = nodes_[node.id];
            for (auto &childID : node.children)
            {
                auto it = nodes_.find(childID);
                if (it == nodes_.end())
                {
                    throw std::runtime_error("invalid child");
                }
                auto &childNode = it->second;
                graphNode.relationships[Relationship::Child].push_back(&childNode);
            }
            for (auto &parentID : node.parents)
            {
                auto it = nodes_.find(parentID);
                if (it == nodes_.end())
                {
                    throw std::runtime_error("invalid parent");
                }
                auto &parentNode = it->second;
                graphNode.relationships[Relationship::Parent].push_back(&parentNode);
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

    DisplayNodes Graph::findRelationship(const std::vector<int> &nodes, const Relationship relationship) const
    {
        std::set<int> seen;
        DisplayNodes out;
        int x = 0;
        for (auto const &node : nodes)
        {
            auto it = nodes_.find(node);
            if (it == nodes_.end())
            {
                throw std::runtime_error("node does not exist");
            }
            findRelationshipRecurse(&it->second, seen, x, 0, out, relationship);
        }
        return out;
    }

    DisplayNodes Graph::findAncestors(const std::vector<int> &nodes) const
    {
        return findRelationship(nodes, Relationship::Parent);
    }

    DisplayNodes Graph::findDescendents(const std::vector<int> &nodes) const
    {
        return findRelationship(nodes, Relationship::Child);
    }

    void Graph::addNode(const Node::Node &node)
    {
        auto it = nodes_.find(node.id);
        if (it != nodes_.end())
        {
            throw std::runtime_error("node with id already exists");
        }
        nodes_[node.id] = GraphNode{node};
        GraphNode &graphNode = nodes_[node.id];
        for (auto const &childID : node.children)
        {
            auto it = nodes_.find(childID);
            if (it == nodes_.end())
            {
                throw std::runtime_error("invalid child");
            }
            auto &childNode = it->second;
            graphNode.relationships[Relationship::Child].push_back(&childNode);
            childNode.node.parents.push_back(node.id);
            childNode.relationships[Relationship::Parent].push_back(&graphNode);
        }
        for (auto const &parentID : node.parents)
        {
            auto it = nodes_.find(parentID);
            if (it == nodes_.end())
            {
                throw std::runtime_error("invalid parent");
            }
            auto &parentNode = it->second;
            graphNode.relationships[Relationship::Parent].push_back(&parentNode);
            parentNode.node.children.push_back(node.id);
            parentNode.relationships[Relationship::Child].push_back(&graphNode);
        }
    }

    void Graph::removeNode(const int &id)
    {
        auto it = nodes_.find(id);
        if (it == nodes_.end())
        {
            throw std::runtime_error("node does not exist");
        }
        // Because we can have child -> parent but not parent -> child (and vice versa), simplest way is to scan every node and edge. Optimize this in future.
        for (auto &[_, graphNode] : nodes_)
        {
            auto &node = graphNode.node;
            node.parents.erase(std::remove(node.parents.begin(), node.parents.end(), id), node.parents.end());
            node.children.erase(std::remove(node.children.begin(), node.children.end(), id), node.children.end());
            std::map<Relationship, std::vector<GraphNode *>> finalRelationships;
            for (auto &[relationshipType, relationships] : graphNode.relationships)
            {
                for (auto &relationship : relationships)
                {
                    if (relationship->node.id != id)
                    {
                        finalRelationships[relationshipType].push_back(relationship);
                    }
                }
            }
            graphNode.relationships = finalRelationships;
        }
        nodes_.erase(id);
    }
}