#include "CLI.hpp"

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

    void MenuState::render() {}

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