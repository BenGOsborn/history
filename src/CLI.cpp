#include "CLI.hpp"
#include <cstdlib>
#include <iostream>
#include <limits>

namespace
{
    void clearScreen()
    {
        std::system("clear");
    }

    void waitForKey()
    {
        std::cout << "Press any key to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

namespace CLI
{
    CLI::CLI() : state_(nullptr) {}

    void CLI::render()
    {
        state_->render();
    }

    void CLI::setState(IState *state)
    {
        state_ = state;
    }

    MenuState::MenuState(CLI &cli) : cli_(cli), ancestorState_(nullptr), descendentState_(nullptr), addState_(nullptr), removeState_(nullptr) {}

    void MenuState::render()
    {
        clearScreen();
        std::cout << "=== Menu ===\n"
                     "1. Show ancestors\n"
                     "2. Show descendents\n"
                     "3. Add person\n"
                     "4. Remove person\n"
                     "0. Quit\n> ";
        int c;
        std::cin >> c;
        switch (c)
        {
        case 1:
            cli_.setState(ancestorState_);
            break;
        case 2:
            cli_.setState(descendentState_);
            break;
        case 3:
            cli_.setState(addState_);
            break;
        case 4:
            cli_.setState(removeState_);
            break;
        default:
            std::cout << "Invalid option" << std::endl;
        }
    }

    void MenuState::setAncestorState(IState *state)
    {
        ancestorState_ = state;
    }

    void MenuState::setDescendentState(IState *state)
    {
        descendentState_ = state;
    }

    void MenuState::setAddState(IState *state)
    {
        addState_ = state;
    }

    void MenuState::setRemoveState(IState *state)
    {
        removeState_ = state;
    }

    AncestorState::AncestorState(CLI &cli, const Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void AncestorState::render()
    {
        clearScreen();
        std::cout << "=== Show ancestors ===\n"
                     "Enter the ID of the node to visualize ancestors for.\n\n"
                     "Your nodes:\n";
        auto nodes = graph_.getNodes();
        for (auto const &node : nodes)
        {
            std::cout << node << std::endl;
        }
        std::cout << "Node ID: ";
        int id;
        std::cin >> id;
        auto ancestors = graph_.findAncestors(id);
        std::cout << ancestors << std::endl;
        waitForKey();
        cli_.setState(menuState_);
    }

    void AncestorState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    DescendentState::DescendentState(CLI &cli, const Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void DescendentState::render()
    {
        clearScreen();
        std::cout << "=== Show descendents ===\n"
                     "Enter the ID of the node to visualize descendents for.\n\n"
                     "Your nodes:\n";
        auto nodes = graph_.getNodes();
        for (auto const &node : nodes)
        {
            std::cout << node << std::endl;
        }
        std::cout << "Node ID: ";
        int id;
        std::cin >> id;
        auto descendents = graph_.findDescendents(id);
        std::cout << descendents << std::endl;
        waitForKey();
        cli_.setState(menuState_);
    }

    void DescendentState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    AddState::AddState(CLI &cli, Graph::Graph &graph, UUID::UUID &uuid) : cli_(cli), graph_(graph), uuid_(uuid), menuState_(nullptr) {}

    void AddState::render() {}

    void AddState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    RemoveState::RemoveState(CLI &cli, Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void RemoveState::render() {}

    void RemoveState::setMenuState(IState *state)
    {
        menuState_ = state;
    }
}