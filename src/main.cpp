#include "Node.hpp"
#include "Data.hpp"
#include <iostream>
#include <memory>
#include <map>

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
    std::unique_ptr<Data::IData> data = std::make_unique<Data::CSVData>(std::move(file));

    auto dfOriginal = Node::toDataframe(nodes);
    data->write(dfOriginal);

    auto dfNew = data->read();
    auto nodesNew = Node::fromDataframe(dfNew);
    for (auto const &node : nodesNew)
    {
        std::cout << node << std::endl;
    }

    return 0;
}