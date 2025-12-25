#include "Data.hpp"
#include <vector>
#include <string>
#include <stdexcept>

File::File(std::string &filename) : filename_(filename)
{
    file_.open(filename, std::ios::in | std::ios::out);
    if (!file_)
    {
        throw std::runtime_error("failed to open file: " + filename);
    }
}

std::vector<std::string> File::read() const
{
    file_.clear();
    file_.seekg(0, std::ios::beg);
    std::vector<std::string> out;
    std::string line;
    while (std::getline(file_, line))
    {
        out.push_back(line);
    }
    return out;
}

void File::write(const std::vector<std::string> &data)
{
    file_.close();
    file_.open(filename_, std::ios::out | std::ios::trunc);
    if (!file_)
        throw std::runtime_error("Failed to open file for writing");
    for (const auto &line : data)
    {
        file_ << line << '\n';
    }
    file_.flush();
    file_.close();
    file_.open(filename_, std::ios::in | std::ios::out);
}

CSVData::CSVData(std::unique_ptr<IFile> file) : file_(std::move(file))
{
    if (!file_)
    {
        throw std::runtime_error("file cannot be null");
    }
}
