#include "CLI.hpp"
#include "Data.hpp"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <memory>
#include <iomanip>

namespace
{
    static const char VECTOR_DELIM = ',';

    void clearScreen()
    {
        std::system("clear");
    }

    void waitForKey()
    {
        std::cout << "Press any key to continue...";
        std::string token;
        std::getline(std::cin, token);
    }

    int parseInt(const std::string &i)
    {
        return std::stoi(i);
    }

    std::time_t parseDate(const std::string &date)
    {
        std::tm tm{};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, "%d/%m/%Y");
        if (ss.fail())
        {
            throw std::runtime_error("Invalid date format (dd/mm/yyyy)");
        }
        return std::mktime(&tm);
    }

    std::vector<int> parseVector(const std::string &vector)
    {
        std::vector<int> values;
        std::string token;
        std::istringstream tokenStream(vector);
        while (std::getline(tokenStream, token, VECTOR_DELIM))
        {
            int val = std::stoi(token);
            values.push_back(val);
        }
        return values;
    }

    Node::Gender parseGender(const std::string &gender)
    {
        auto genderI = gender[0];
        return static_cast<Node::Gender>(genderI);
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
                     "0. Exit\n> ";
        std::string c;
        std::getline(std::cin, c);
        switch (parseInt(c))
        {
        case 0:
            std::exit(EXIT_SUCCESS);
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
        std::string id;
        std::getline(std::cin, id);
        auto ancestors = graph_.findAncestors(parseInt(id));
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
        std::string id;
        std::getline(std::cin, id);
        auto descendents = graph_.findDescendents(parseInt(id));
        std::cout << descendents << std::endl;
        waitForKey();
        cli_.setState(menuState_);
    }

    void DescendentState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    AddState::AddState(CLI &cli, Graph::Graph &graph, UUID::UUID &uuid) : cli_(cli), graph_(graph), uuid_(uuid), menuState_(nullptr) {}

    void AddState::render()
    {
        clearScreen();
        std::cout << "=== Add node ===\n";
        int id = uuid_.generateID();
        std::string token;
        std::cout << "Enter the name: ";
        std::getline(std::cin, token);
        auto name = token;
        std::cout << "Enter the date of birth: ";
        std::getline(std::cin, token);
        auto dob = parseDate(token);
        std::cout << "Enter the gender: ";
        std::getline(std::cin, token);
        auto gender = parseGender(token);
        std::cout << "Current nodes:\n";
        auto nodes = graph_.getNodes();
        for (auto const &node : nodes)
        {
            std::cout << node << std::endl;
        }
        std::cout << "Enter the children IDs (comma separated values, no spaces): ";
        std::getline(std::cin, token);
        auto children = parseVector(token);
        std::cout << "Enter the parent IDs (comma separated values, no spaces): ";
        std::getline(std::cin, token);
        auto parents = parseVector(token);
        Node::Node node{id, name, dob, gender, children, parents};
        graph_.addNode(node);
        cli_.setState(menuState_);
    }

    void AddState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    RemoveState::RemoveState(CLI &cli, Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void RemoveState::render()
    {
        clearScreen();
        std::cout << "=== Remove node ===\n"
                     "Enter the ID of the node to remove from the tree.\n\n"
                     "Your nodes:\n";
        auto nodes = graph_.getNodes();
        for (auto const &node : nodes)
        {
            std::cout << node << std::endl;
        }
        std::cout << "Node ID: ";
        std::string id;
        std::getline(std::cin, id);
        graph_.removeNode(parseInt(id));
        cli_.setState(menuState_);
    }

    void RemoveState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    LoadState::LoadState(CLI &cli, Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void LoadState::render()
    {
        clearScreen();
        std::cout << "=== Load state ===\n"
                     "Enter the filename of the file you want to load: ";
        std::string filename;
        std::getline(std::cin, filename);
        std::unique_ptr<Data::IFile> file = std::make_unique<Data::File>(filename);
        Data::CSVData data(std::move(file));
        auto dfNew = data.read();
        graph_.loadNodes(dfNew);
        cli_.setState(menuState_);
    }

    void LoadState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

    SaveState::SaveState(CLI &cli, Graph::Graph &graph) : cli_(cli), graph_(graph), menuState_(nullptr) {}

    void SaveState::render()
    {
        clearScreen();
        cli_.setState(menuState_);
    }

    void SaveState::setMenuState(IState *state)
    {
        menuState_ = state;
    }

}