#include "Node.hpp"
#include <map>
#include <sstream>
#include <string>
#include <chrono>

namespace
{
    static const char DELIM = ',';

    static const std::string HEAD_ID = "id";
    static const std::string HEAD_NAME = "name";
    static const std::string HEAD_BIRTH = "birth";
    static const std::string HEAD_GENDER = "gender";
    static const std::string HEAD_CHILDREN = "children";
    static const std::string HEAD_PARENTS = "parents";

    std::string serializeVec(const std::vector<int> &vec) noexcept
    {
        std::string out;
        for (int i = 0; i < vec.size(); i++)
        {
            out += std::to_string(vec[i]);
            if (i != vec.size() - 1)
            {
                out += DELIM;
            }
        }
        return out;
    }

    std::vector<int> deserializeVec(const std::string &serialized)
    {
        std::vector<int> out;
        std::string token;
        std::stringstream ss(serialized);
        while (std::getline(ss, token, DELIM))
        {
            out.push_back(std::stoi(token));
        }
        return out;
    }

    const std::string &getRowColumn(const std::map<std::string, std::vector<std::string>> &dataframe, const std::string &col, const int &row)
    {
        auto it = dataframe.find(col);
        if (it == dataframe.end())
        {
            throw std::runtime_error("column is missing");
        }
        auto &vec = it->second;
        if (row >= vec.size() || row < 0)
        {
            throw std::runtime_error("row exceeds bounds");
        }
        auto &ref = vec[row];
        return ref;
    }
}

namespace Node
{
    Gender parseGender(const std::string &s)
    {
        if (s.size() != 1)
        {
            throw std::invalid_argument("invalid gender size");
        }

        char c = s[0];
        switch (c)
        {
        case 'm':
            return Gender::Male;
        case 'f':
            return Gender::Female;
        default:
            throw std::invalid_argument("invalid gender");
        }
    }

    std::map<std::string, std::vector<std::string>> toDataframe(const std::vector<Node> &nodes) noexcept
    {
        std::map<std::string, std::vector<std::string>> dataframe;
        for (auto const &node : nodes)
        {
            dataframe[HEAD_ID].push_back(std::to_string(node.id));
            dataframe[HEAD_NAME].push_back(node.name);
            dataframe[HEAD_BIRTH].push_back(std::to_string(node.birth));
            dataframe[HEAD_GENDER].push_back(std::string(1, static_cast<char>(node.gender)));
            dataframe[HEAD_CHILDREN].push_back(serializeVec(node.children));
            dataframe[HEAD_PARENTS].push_back(serializeVec(node.parents));
        }
        return dataframe;
    }

    std::vector<Node> fromDataframe(const std::map<std::string, std::vector<std::string>> &dataframe) noexcept
    {
        size_t rows = 0;
        for (auto const &[key, val] : dataframe)
        {
            rows = std::max(rows, val.size());
        }
        std::vector<Node> nodes(rows);
        for (int i = 0; i < nodes.size(); i++)
        {
            auto id = getRowColumn(dataframe, HEAD_ID, i);
            auto name = getRowColumn(dataframe, HEAD_NAME, i);
            auto birth = getRowColumn(dataframe, HEAD_BIRTH, i);
            auto gender = getRowColumn(dataframe, HEAD_GENDER, i);
            auto children = getRowColumn(dataframe, HEAD_CHILDREN, i);
            auto parents = getRowColumn(dataframe, HEAD_PARENTS, i);
            auto &node = nodes[i];
            node.id = std::stoi(id);
            node.name = name;
            node.birth = static_cast<std::time_t>(std::stol(birth));
            node.gender = parseGender(gender);
            node.children = deserializeVec(children);
            node.parents = deserializeVec(parents);
        }
        return nodes;
    }

    std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        auto id = std::to_string(node.id);
        auto name = node.name;
        auto birth = std::to_string(node.birth);
        auto gender = std::string(1, static_cast<char>(node.gender));
        auto children = serializeVec(node.children);
        auto parents = serializeVec(node.parents);
        return os << "Node(" + id + "," + name + "," + birth + "," + gender + "," + "(" + children + ")," + "(" + parents + "))";
    }
}
