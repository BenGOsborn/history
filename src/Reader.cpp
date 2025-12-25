#include "Reader.hpp"
#include <string>
#include <stdexcept>

File::File(std::string &filename) : file_(filename)
{
    if (!file_)
    {
        throw std::runtime_error("failed to open file: " + filename);
    }
}

File::~File() {}

std::string File::next() const
{
}

bool File::hasNext() const {}