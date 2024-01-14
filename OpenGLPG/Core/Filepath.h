#pragma once

#include "Serializable.h"

#include <string>

class Filepath : public Serializable
{
public:
    // Intentionally not explicit
    Filepath();
    Filepath(const char* aPath);
    Filepath(const std::string& aPath);
    Filepath(const Filepath& aPath);

    void Serialize(Serializer& aSerializer) override;

    const char* GetBuffer() const;
    bool HasExtension(const char* anExtension = "") const;
    void RemoveExtension();
    void Append(const char* aString);

    Filepath operator+(const Filepath& anOther) const;
    bool operator==(const Filepath& anOther) const;

private:
    std::string myPath;

    friend struct std::hash<Filepath>;
};

template <>
struct std::hash<Filepath>
{
    // @improvement: have filepath generate a UID based on its path (a StringID) and use this to get a hash. This allows
    // for faster access to maps indexed by filepaths (since we don't need to process potentially very long strings)
    inline std::size_t operator()(const Filepath& aFilepath) const
    {
        return std::hash<std::string> {}(aFilepath.myPath);
    }
};
