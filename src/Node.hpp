#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

const char DELIM = ',';
const char ESCAPE = '\\';

enum class Gender : char
{
    Male = 'm',
    Female = 'f',
};

struct Node
{
    int id;
    std::string name;
    std::time_t birth;
    Gender gender;
    std::vector<int> parents;
    std::vector<int> children;
};

std::string escapeString(const std::string &str) noexcept
{
    std::string escaped;
    for (const char &c : str)
    {
        if (c == ESCAPE)
        {
            escaped += ESCAPE;
        }
        escaped += c;
    }
    std::string out;
    for (const char &c : escaped)
    {
        if (c == DELIM)
        {
            out += ESCAPE;
        }
        out += c;
    }
    return out;
}

template <typename T>
std::string serializeVec(const std::vector<T> vec) noexcept
{
    std::string out;
    for (int i = 0; i < vec.size(); i++)
    {
        out += std::to_string(vec.at(i));
        if (i != vec.size() - 1)
        {
            out += DELIM;
        }
    }
    return out;
}

std::vector<std::string> buildColumns(const Node &node) noexcept
{
    std::vector<std::string> out;
    out.push_back(std::to_string(node.id));
    out.push_back(escapeString(node.name));
    out.push_back(std::to_string(node.birth));
    out.push_back(std::to_string(static_cast<char>(node.gender)));
    out.push_back(escapeString(serializeVec(node.parents)));
    out.push_back(escapeString(serializeVec(node.children)));
    return out;
}

std::ostream &operator<<(std::ostream &os, const Node &node)
{
    std::vector<std::string> columns = buildColumns(node);
    for (int i = 0; i < columns.size(); i++)
    {
        os << columns.at(i);
        if (i != columns.size() - 1)
        {
            os << DELIM;
        }
    }
    return os;
}
