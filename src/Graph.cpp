#include "Graph.hpp"
#include "Node.hpp"
#include <algorithm>
#include <utility>
#include <queue>
#include <set>

namespace Graph
{
    Graph::Graph(std::vector<Node::Node> nodes) : nodes_(std::move(nodes))
    {
        for (auto &node : nodes_)
        {
            GraphNode graphNode{&node};
            IDToNode_[node.id] = graphNode;
        }

        for (auto &node : nodes_)
        {
            GraphNode &parentNode = IDToNode_[node.id];
            for (auto &child : node.children)
            {
                auto it = IDToNode_.find(child);
                if (it == IDToNode_.end())
                {
                    throw std::runtime_error("child is missing");
                }
                GraphNode &childNode = IDToNode_[child];
                childNode.parents.push_back(&IDToNode_[node.id]);
                parentNode.children.push_back(&IDToNode_[child]);
            }
        }
    }

    std::vector<Node::Node> Graph::getNodes() const
    {
        return nodes_;
    }

    std::vector<Node::Node> Graph::findAncestors(const std::vector<Node::Node> &nodes) const
    {
        std::set<int> seen;
        std::queue<const GraphNode *> queue;
        std::vector<Node::Node> ancestors;
        for (auto const &node : nodes)
        {
            auto it = IDToNode_.find(node.id);
            if (it == IDToNode_.end())
            {
                throw std::runtime_error("node does not exist");
            }
            queue.push(&it->second);
        }
        while (!queue.empty())
        {
            auto graphNode = queue.front();
            queue.pop();
            if (graphNode->node == nullptr)
            {
                throw std::runtime_error("invalid node has null data");
            }
            auto node = graphNode->node;
            int id = node->id;
            if (seen.find(id) != seen.end())
            {
                continue;
            }
            seen.insert(id);
            ancestors.push_back(*node);
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
            auto it = IDToNode_.find(node.id);
            if (it == IDToNode_.end())
            {
                throw std::runtime_error("node does not exist");
            }
            queue.push(&it->second);
        }
        while (!queue.empty())
        {
            auto graphNode = queue.front();
            queue.pop();
            if (graphNode->node == nullptr)
            {
                throw std::runtime_error("invalid node has null data");
            }
            auto node = graphNode->node;
            int id = node->id;
            if (seen.find(id) != seen.end())
            {
                continue;
            }
            seen.insert(id);
            ancestors.push_back(*node);
            for (auto const &child : graphNode->children)
            {
                queue.push(child);
            }
        }
        return ancestors;
    }
}