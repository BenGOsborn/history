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
    auto ancestors = graph.findAncestors({Node::Node{1}});
    std::cout << ancestors << std::endl;

    std::cout << "Descendents" << std::endl;
    auto descendents = graph.findDescendents({Node::Node{1}});
    std::cout << descendents << std::endl;

    return 0;
}