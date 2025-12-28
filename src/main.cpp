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

void initCLI(CLI::CLI &cli, CLI::MenuState *menuState, CLI::AncestorState *ancestorState, CLI::DescendentState *descendentState, CLI::AddState *addState, CLI::RemoveState *removeState)
{
    menuState->setAncestorState(ancestorState);
    menuState->setDescendentState(descendentState);
    menuState->setAddState(addState);
    menuState->setRemoveState(removeState);
    ancestorState->setMenuState(menuState);
    descendentState->setMenuState(menuState);
    addState->setMenuState(menuState);
    removeState->setMenuState(menuState);
    cli.setState(menuState);
}

int main()
{
    std::unique_ptr<Data::IFile> file = std::make_unique<Data::File>(FILE_NAME);
    Data::CSVData data(std::move(file));
    auto nodes = getNodes(data);
    UUID::UUID uuid(nodes);
    Graph::Graph graph(nodes);

    CLI::CLI cli;
    auto menuState = std::make_unique<CLI::MenuState>(cli);
    auto ancestorState = std::make_unique<CLI::AncestorState>(cli, graph);
    auto descendentState = std::make_unique<CLI::DescendentState>(cli, graph);
    auto addState = std::make_unique<CLI::AddState>(cli, graph, uuid);
    auto removeState = std::make_unique<CLI::RemoveState>(cli, graph);
    initCLI(cli, menuState.get(), ancestorState.get(), descendentState.get(), addState.get(), removeState.get());

    while (true)
    {
        cli.render();
    }

    return 0;
}