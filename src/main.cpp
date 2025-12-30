#include "CLI.hpp"
#include "Graph.hpp"
#include <iostream>
#include <memory>
#include <map>
#include <utility>

void initCLI(
    CLI::CLI &cli,
    CLI::MenuState *menuState,
    CLI::AncestorState *ancestorState,
    CLI::DescendentState *descendentState,
    CLI::AddState *addState,
    CLI::RemoveState *removeState,
    CLI::LoadState *loadState,
    CLI::SaveState *saveState)
{
    menuState->setAncestorState(ancestorState);
    menuState->setDescendentState(descendentState);
    menuState->setAddState(addState);
    menuState->setRemoveState(removeState);
    menuState->setLoadState(loadState);
    menuState->setSaveState(saveState);
    ancestorState->setMenuState(menuState);
    descendentState->setMenuState(menuState);
    addState->setMenuState(menuState);
    removeState->setMenuState(menuState);
    loadState->setMenuState(menuState);
    saveState->setMenuState(menuState);
    cli.setState(menuState);
}

int main()
{
    Graph::Graph graph;
    CLI::CLI cli;
    auto menuState = std::make_unique<CLI::MenuState>(cli);
    auto ancestorState = std::make_unique<CLI::AncestorState>(cli, graph);
    auto descendentState = std::make_unique<CLI::DescendentState>(cli, graph);
    auto addState = std::make_unique<CLI::AddState>(cli, graph);
    auto removeState = std::make_unique<CLI::RemoveState>(cli, graph);
    auto loadState = std::make_unique<CLI::LoadState>(cli, graph);
    auto saveState = std::make_unique<CLI::SaveState>(cli, graph);
    initCLI(cli, menuState.get(), ancestorState.get(), descendentState.get(), addState.get(), removeState.get(), loadState.get(), saveState.get());
    while (true)
    {
        cli.render();
    }
    return 0;
}