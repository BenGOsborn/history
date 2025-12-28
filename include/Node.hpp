#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <map>

namespace Node
{
    enum class Gender : char
    {
        Male = 'm',
        Female = 'f',
    };

    Gender parseGender(const std::string &s);

    struct Node
    {
        int id;
        std::string name;
        std::time_t birth;
        Gender gender;
        std::vector<int> children;
        std::vector<int> parents;
    };

    std::map<std::string, std::vector<std::string>> toDataframe(const std::vector<Node> &nodes) noexcept;

    std::vector<Node> fromDataframe(const std::map<std::string, std::vector<std::string>> &dataframe) noexcept;

    std::ostream &operator<<(std::ostream &os, const Node &node);
}