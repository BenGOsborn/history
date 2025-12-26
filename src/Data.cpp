#include "Data.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>

namespace
{
    static const char DELIM = ',';
    static const char ESCAPE = '\\';

    std::vector<std::string> deserializeRow(const std::string &row) noexcept
    {
        std::vector<std::string> columns;
        if (row.size() == 0)
        {
            return columns;
        }
        std::string chunk;
        bool escaped = false;
        for (auto const &c : row)
        {
            if (c == ESCAPE)
            {
                if (!escaped)
                {
                    escaped = true;
                    continue;
                }
            }
            if (c == DELIM)
            {
                if (!escaped)
                {
                    columns.push_back(chunk);
                    chunk = "";
                    continue;
                }
            }
            chunk += c;
            escaped = false;
        }
        columns.push_back(chunk);
        return columns;
    }

    std::string escapeString(const std::string &str) noexcept
    {
        std::string escaped;
        for (auto const &c : str)
        {
            if (c == ESCAPE)
            {
                escaped += ESCAPE;
            }
            escaped += c;
        }
        std::string out;
        for (auto const &c : escaped)
        {
            if (c == DELIM)
            {
                out += ESCAPE;
            }
            out += c;
        }
        return out;
    }

    std::string serializeRow(const std::vector<std::string> &columns) noexcept
    {
        std::string out;
        for (int i = 0; i < columns.size(); i++)
        {
            out += escapeString(columns.at(i));
            if (i != columns.size() - 1)
            {
                out += DELIM;
            }
        }
        return out;
    }
}

namespace Data
{
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

    void File::write(const std::vector<std::string> &data) const
    {
        std::ofstream file(filename_);
        if (!file)
        {
            throw std::runtime_error("Failed to open file for writing");
        }
        for (auto const &line : data)
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

    std::map<std::string, std::vector<std::string>> CSVData::read() const
    {
        std::vector<std::string> rows = file_->read();
        if (rows.size() == 0)
        {
            throw std::runtime_error("file must have at least one row");
        }
        std::map<std::string, std::vector<std::string>> data;
        auto header = deserializeRow(rows.at(0));
        for (int i = 1; i < rows.size(); i++)
        {
            auto columns = deserializeRow(rows.at(i));
            if (columns.size() != header.size())
            {
                throw std::runtime_error("row size does not match header size");
            }
            for (int i = 0; i < columns.size(); i++)
            {
                data[header.at(i)].push_back(columns.at(i));
            }
        }
        return data;
    }

    void CSVData::write(const std::map<std::string, std::vector<std::string>> &data) const
    {
        std::vector<std::string> headers;
        size_t rows = 0;
        for (auto const &[key, val] : data)
        {
            headers.push_back(key);
            rows = std::max(rows, val.size());
        }
        std::vector<std::string> columns{serializeRow(headers)};
        for (int i = 0; i < rows; i++)
        {
            std::vector<std::string> row;
            for (auto const &header : headers)
            {
                auto it = data.find(header);
                if (it == data.end())
                {
                    throw std::runtime_error("header is missing");
                }
                auto col = it->second;
                if (i >= col.size())
                {
                    throw std::runtime_error("invalid sized column");
                }
                row.push_back(col.at(i));
            }
            columns.push_back(serializeRow(row));
        }
        file_->write(columns);
    }
}