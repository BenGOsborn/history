#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

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

std::ostream &operator<<(std::ostream &os, const Node &node);