#include <iostream>
#include "Node.hpp"
#include <vector>
#include <fstream>
#include <string>

const std::string FILE_NAME = "example.csv";

void saveData(const std::vector<Node> &nodes, std::ofstream &file)
{
    for (const Node &node : nodes)
    {
        file << node << std::endl;
    }
}

std::vector<Node> loadData(std::ifstream &file)
{
    std::vector<Node> out;
    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << '\n';
    }
    return out;
}

int main()
{
    std::vector<Node> nodes{
        {
            1,
            "Ben Osborn",
            1766639278,
            Gender::Male,
            {2, 3},
            {},
        },
        {
            2,
            "David Osborn",
            1766639278,
            Gender::Male,
            {},
            {1},
        },
        {
            3,
            "Kristin Lidbury",
            1766639278,
            Gender::Female,
            {},
            {1},
        },
    };

    std::ofstream outFile(FILE_NAME);
    if (!outFile)
    {
        return 1;
    }
    saveData(nodes, outFile);
    outFile.close();

    std::ifstream inFile(FILE_NAME);
    if (!inFile)
    {
        return 1;
    }
    std::vector<Node> finalNodes = loadData(inFile);
    inFile.close();

    return 0;
}