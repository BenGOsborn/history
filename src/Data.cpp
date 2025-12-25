#include "Data.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

const char DELIM = ',';
const char ESCAPE = '\\';

File::File(std::string &filename) : filename_(filename)
{
}

std::vector<std::string> File::read() const
{

    std::ifstream file(filename_);
    if (!file)
    {
        throw std::runtime_error("failed to open file");
    }
    std::vector<std::string> out;
    std::string line;
    while (std::getline(file, line))
    {
        out.push_back(line);
    }
    return out;
}

void File::write(const std::vector<std::string> &data)
{
    std::ofstream file(filename_);
    if (!file)
    {
        throw std::runtime_error("Failed to open file for writing");
    }
    for (const auto &line : data)
    {
        file << line << '\n';
    }
}

CSVData::CSVData(std::unique_ptr<IFile> file) : file_(std::move(file))
{
    if (!file_)
    {
        throw std::runtime_error("file cannot be null");
    }
}

std::string unescapeString(const std::string &str) noexcept
{
}

std::map<std::string, std::vector<std::string>> CSVData::read() const
{
    std::vector<std::string> rows = file_->read();
    if (rows.size() < 1)
    {
        throw std::runtime_error("file must have at least one row");
    }
    std::string header = rows.at(0);
}

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

void CSVData::write(const std::map<std::string, std::vector<std::string>> &data)
{
}