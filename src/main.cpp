#include "Node.hpp"
#include "Data.hpp"
#include "UUID.hpp"
#include "Graph.hpp"
#include <iostream>
#include <memory>
#include <map>
#include <utility>

const std::string FILE_NAME = "example.csv";

std::vector<Node::Node> getNodes(const Data::CSVData &data)
{
    try
    {
        auto dfNew = data.read();
        return Node::fromDataframe(dfNew);
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Failed to open file, error: " << e.what() << std::endl;
        return std::vector<Node::Node>();
    }
}

int main()
{
    std::unique_ptr<Data::IFile> file = std::make_unique<Data::File>(FILE_NAME);
    Data::CSVData data(std::move(file));
    auto nodes = getNodes(data);
    UUID::UUID uuid(nodes);
    Graph::Graph graph(nodes);

    return 0;
}