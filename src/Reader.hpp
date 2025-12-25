#pragma once
#include <memory>
#include <map>
#include <fstream>
#include <string>

class IFile
{
public:
    virtual ~IFile() = default;
    virtual std::string next() const = 0;
    virtual bool hasNext() const = 0;
};

class File : public IFile
{
    mutable std::ifstream file_;

public:
    explicit File(std::string &filename);
    ~File();
    std::string next() const override;
    bool hasNext() const override;
};

class IReader
{
public:
    virtual ~IReader() = default;
    virtual std::map<std::string, std::string> next() const = 0;
    virtual bool hasNext() const = 0;
};

class CSVReader : public IReader
{
    std::unique_ptr<IFile> file_;

public:
    explicit CSVReader();
    ~CSVReader();
    std::map<std::string, std::string> next() const override;
    bool hasNext() const override;
};