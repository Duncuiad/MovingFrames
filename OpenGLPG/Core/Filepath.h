#pragma once

#include <string>

class Filepath
{
public:
    // Intentionally not explicit
    Filepath(const char* aPath);
    Filepath(const std::string& aPath);
    Filepath(const Filepath& aPath);
    const char* GetBuffer() const;

    bool HasExtension(const char* anExtension) const;
    void Append(const char* aString);

    bool operator==(const Filepath& anOther) const;

private:
    std::string myPath;

    friend struct std::hash<Filepath>;
};

template <>
struct std::hash<Filepath>
{
    inline std::size_t operator()(const Filepath& aFilepath) const
    {
        return std::hash<std::string> {}(aFilepath.myPath);
    }
};
