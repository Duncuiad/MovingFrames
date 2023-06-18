#pragma once

#include <functional>
#include <string>

class UID
{
public:
    static UID CreateUnique();

    UID() = default;
    UID(const UID& anOther);

    bool IsValid();
    bool operator==(const UID& anOther) const;
    std::string GetString() const;

    static const UID Empty;

private:
    static unsigned long int ourCurrent;
    unsigned long int myInternalID {0u};

    friend struct std::hash<UID>;
};

template <>
struct std::hash<UID>
{
    inline std::size_t operator()(const UID& aUID) const { return static_cast<std::size_t>(aUID.myInternalID); }
};
