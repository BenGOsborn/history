#pragma once
#include "Graph.hpp"

namespace CLI
{
    class IState
    {
    public:
        virtual ~IState() = default;
        virtual void render() = 0;
    };

    class CLI
    {
        IState *state_;

    public:
        CLI();
        ~CLI() = default;
        void render();
        void setState(IState *state);
    };

    class MenuState : IState
    {
        CLI &cli_;
        IState *ancestorState_;
        IState *descendentState_;
        IState *addState_;
        IState *removeState_;

    public:
        MenuState(CLI &cli);
        ~MenuState() = default;
        void render() override;
        void setAncestorState(IState *state);
        void setDescendentState(IState *state);
        void setAddState(IState *state);
        void setRemoveState(IState *state);
    };

    class AncestorState : IState
    {
        CLI &cli_;
        const Graph::Graph &graph_;
        IState *menuState_;

    public:
        AncestorState(CLI &cli, const Graph::Graph &graph);
        ~AncestorState() = default;
        void render() override;
        void setMenuState(IState *state);
    };

    class DescendentState : IState
    {
        CLI &cli_;
        const Graph::Graph &graph_;
        IState *menuState_;

    public:
        DescendentState(CLI &cli, const Graph::Graph &graph);
        ~DescendentState() = default;
        void render() override;
        void setMenuState(IState *state);
    };

    class AddState : IState
    {
        CLI &cli_;
        Graph::Graph &graph_;
        IState *menuState_;

    public:
        AddState(CLI &cli, Graph::Graph &graph);
        ~AddState() = default;
        void render() override;
        void setMenuState(IState *state);
    };

    class RemoveState : IState
    {
        CLI &cli_;
        Graph::Graph &graph_;
        IState *menuState_;

    public:
        RemoveState(CLI &cli, Graph::Graph &graph);
        ~RemoveState() = default;
        void render() override;
        void setMenuState(IState *state);
    };
}