#include "Node.hpp"
#include "Data.hpp"
#include "Graph.hpp"
#include <iostream>
#include <memory>
#include <map>
#include <utility>

const std::string FILE_NAME = "example.csv";

int main()
{
    std::unique_ptr<Data::IFile> file = std::make_unique<Data::File>(FILE_NAME);
    Data::CSVData data(std::move(file));

    auto dfNew = data.read();
    auto nodesNew = Node::fromDataframe(dfNew);
    std::cout << "Read nodes" << std::endl;
    for (auto const &node : nodesNew)
    {
        std::cout << node << std::endl;
    }

    Graph::Graph graph(std::move(nodesNew));

    std::cout << "Ancestors" << std::endl;
    auto ancestors = graph.findAncestors({1});
    std::cout << ancestors << std::endl;

    std::cout << "Add node" << std::endl;
    graph.addNode(Node::Node{10, "BIG TEST", 1766639278, Node::Gender::Male, {}, {1}});
    ancestors = graph.findAncestors({10});
    std::cout << ancestors << std::endl;

    std::cout << "Delete node" << std::endl;
    graph.removeNode(10);
    auto descendents = graph.findDescendents({1});
    std::cout << descendents << std::endl;
    ancestors = graph.findAncestors({1});
    std::cout << ancestors << std::endl;

    return 0;
}