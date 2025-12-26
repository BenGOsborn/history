#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>

namespace Data
{
    class IFile
    {
    public:
        virtual ~IFile() = default;
        virtual std::vector<std::string> read() const = 0;
        virtual void write(const std::vector<std::string> &lines) const = 0;
    };

    class File : public IFile
    {
        std::string filename_;

    public:
        explicit File(const std::string &filename);
        ~File() = default;
        std::vector<std::string> read() const override;
        void write(const std::vector<std::string> &lines) const override;
    };

    class IData
    {
    public:
        virtual ~IData() = default;
        virtual std::map<std::string, std::vector<std::string>> read() const = 0;
        virtual void write(const std::map<std::string, std::vector<std::string>> &data) const = 0;
    };

    class CSVData : public IData
    {
        std::unique_ptr<IFile> file_;

    public:
        explicit CSVData(std::unique_ptr<IFile> file);
        ~CSVData() = default;
        std::map<std::string, std::vector<std::string>> read() const override;
        void write(const std::map<std::string, std::vector<std::string>> &data) const override;
    };
}