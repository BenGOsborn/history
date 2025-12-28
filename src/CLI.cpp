#include "CLI.hpp"
#include <cstdlib>
#include <iostream>

namespace
{
    void clearScreen()
    {
        std::system("clear");
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
        std::cout << "=== Ancestry CLI ===\n"
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

    void AncestorState::render() {}

    void AncestorState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    DescendentState::DescendentState(CLI &cli, const Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void DescendentState::render() {}

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