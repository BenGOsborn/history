#include "Node.hpp"
#include "Data.hpp"
#include "UUID.hpp"
#include "CLI.hpp"
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

std::unique_ptr<CLI::CLI> initCLI(Graph::Graph &graph, UUID::UUID &uuid)
{
    auto cli = std::make_unique<CLI::CLI>();
    auto menuState = std::make_unique<CLI::MenuState>(*cli);
    auto ancestorState = std::make_unique<CLI::AncestorState>(*cli, graph);
    auto descendentState = std::make_unique<CLI::DescendentState>(*cli, graph);
    auto addState = std::make_unique<CLI::AddState>(*cli, graph, uuid);
    auto removeState = std::make_unique<CLI::RemoveState>(*cli, graph);
    menuState->setAncestorState(ancestorState.get());
    menuState->setDescendentState(descendentState.get());
    menuState->setAddState(addState.get());
    menuState->setRemoveState(removeState.get());
    ancestorState->setMenuState(menuState.get());
    descendentState->setMenuState(menuState.get());
    addState->setMenuState(menuState.get());
    removeState->setMenuState(menuState.get());
    return cli;
}

int main()
{
    std::unique_ptr<Data::IFile> file = std::make_unique<Data::File>(FILE_NAME);
    Data::CSVData data(std::move(file));
    auto nodes = getNodes(data);
    UUID::UUID uuid(nodes);
    Graph::Graph graph(nodes);

    auto cli = initCLI(graph, uuid);

    return 0;
}