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
    std::vector<Node::Node> nodes{
        {
            1,
            "Ben Osborn",
            1766639278,
            Node::Gender::Male,
            {},
        },
        {
            2,
            "David Osborn",
            1766639278,
            Node::Gender::Male,
            {1},
        },
        {
            3,
            "Kristin Lidbury",
            1766639278,
            Node::Gender::Female,
            {1},
        },
    };
    std::unique_ptr<Data::IFile> file = std::make_unique<Data::File>(FILE_NAME);
    Data::CSVData data(std::move(file));

    auto dfOriginal = Node::toDataframe(nodes);
    data.write(dfOriginal);

    auto dfNew = data.read();
    auto nodesNew = Node::fromDataframe(dfNew);
    std::cout << "Read nodes" << std::endl;
    for (auto const &node : nodesNew)
    {
        std::cout << node << std::endl;
    }

    Graph::Graph graph(std::move(nodesNew));

    std::cout << "Ancestors" << std::endl;
    auto ancestors = graph.findAncestors({nodes[0]});
    for (auto const &node : ancestors)
    {
        std::cout << node << std::endl;
    }

    std::cout << "Descendents" << std::endl;
    auto descendents = graph.findDescendents({nodes[1]});
    for (auto const &node : descendents)
    {
        std::cout << node << std::endl;
    }

    return 0;
}