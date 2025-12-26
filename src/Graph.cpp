#include "Graph.hpp"
#include "Node.hpp"
#include <algorithm>
#include <utility>
#include <queue>
#include <set>

namespace Graph
{
    Graph::Graph(const std::vector<Node::Node> &nodes)
    {
        maxNodeID_ = -1;
        for (auto const &node : nodes)
        {
            GraphNode graphNode{node};
            nodes_[node.id] = graphNode;
            maxNodeID_ = std::max(maxNodeID_, node.id);
        }
        maxNodeID_ += 1;
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
                childNode.parents.push_back(&nodes_[node.id]);
                parentNode.children.push_back(&nodes_[child]);
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

    std::vector<Node::Node> Graph::findAncestors(const std::vector<Node::Node> &nodes) const
    {
        std::set<int> seen;
        std::queue<const GraphNode *> queue;
        std::vector<Node::Node> ancestors;
        for (auto const &node : nodes)
        {
            auto it = nodes_.find(node.id);
            if (it == nodes_.end())
            {
                throw std::runtime_error("node does not exist");
            }
            queue.push(&it->second);
        }
        while (!queue.empty())
        {
            auto graphNode = queue.front();
            queue.pop();
            auto node = graphNode->node;
            if (seen.find(node.id) != seen.end())
            {
                continue;
            }
            seen.insert(node.id);
            ancestors.push_back(node);
            for (auto const &parent : graphNode->parents)
            {
                queue.push(parent);
            }
        }
        return ancestors;
    }

    std::vector<Node::Node> Graph::findDescendents(const std::vector<Node::Node> &nodes) const
    {
        std::set<int> seen;
        std::queue<const GraphNode *> queue;
        std::vector<Node::Node> ancestors;
        for (auto const &node : nodes)
        {
            auto it = nodes_.find(node.id);
            if (it == nodes_.end())
            {
                throw std::runtime_error("node does not exist");
            }
            queue.push(&it->second);
        }
        while (!queue.empty())
        {
            auto graphNode = queue.front();
            queue.pop();
            auto node = graphNode->node;
            if (seen.find(node.id) != seen.end())
            {
                continue;
            }
            seen.insert(node.id);
            ancestors.push_back(node);
            for (auto const &child : graphNode->children)
            {
                queue.push(child);
            }
        }
        return ancestors;
    }
}