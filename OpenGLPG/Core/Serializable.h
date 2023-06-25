#pragma once

class Serializer;

#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

class Serializable
{
public:
    virtual ~Serializable() = default;
    virtual void Serialize(Serializer& aSerializer) = 0;
};

class SerializableDynamic
{
public:
    virtual ~SerializableDynamic() = default;
    virtual void Serialize(Serializer& aSerializer) = 0;
    virtual std::string GetSubtypeId() const = 0;
};

inline SerializableDynamic* Unwrap(SerializableDynamic* aPtr)
{
    return aPtr;
}

template <typename PtrT>
inline PtrT Wrap(SerializableDynamic* aPtr);

template <>
inline SerializableDynamic* Wrap<SerializableDynamic*>(SerializableDynamic* aPtr)
{
    return aPtr;
}

// @note: This is a singleton. It's like this to make development faster, but it doesn't have to be a singleton: you
// could provide a custom factory to a serializer in order to tell it how to construct your custom types
class SerializationFactory
{
public:
    using Builder = std::function<SerializableDynamic*()>;

    static std::unordered_map<std::string, Builder>& GetBuilders();

    template <typename SubtypeT>
    static void RegisterSubtype()
    {
        static_assert(std::is_base_of_v<SerializableDynamic, SubtypeT>);
        Builder builder {[]() { return new SubtypeT {}; }};
        GetBuilders().emplace(SubtypeT::GetStaticSubtypeId(), std::move(builder));
    }
    template <typename SubtypeT>
    static void UnregisterSubtype()
    {
        static_assert(std::is_base_of_v<SerializableDynamic, SubtypeT>);
        GetBuilders().erase(SubtypeT::GetStaticSubtypeId());
    }

private:
    static SerializationFactory* ourInstance;
    std::unordered_map<std::string, Builder> myBuilders;
};

template <typename SubtypeT>
struct SerializationRegistration
{
    SerializationRegistration() { SerializationFactory::RegisterSubtype<SubtypeT>(); }
    ~SerializationRegistration() { SerializationFactory::UnregisterSubtype<SubtypeT>(); }
};

#define DECLARE_SUBTYPE(SUBTYPE)                                                                                       \
public:                                                                                                                \
    SUBTYPE() = default;                                                                                               \
    static std::string GetStaticSubtypeId()                                                                            \
    {                                                                                                                  \
        return #SUBTYPE;                                                                                               \
    }                                                                                                                  \
    std::string GetSubtypeId() const override                                                                          \
    {                                                                                                                  \
        return SUBTYPE::GetStaticSubtypeId();                                                                          \
    }                                                                                                                  \
    static SerializationRegistration<SUBTYPE> ourSerializationRegistration;                                            \
                                                                                                                       \
private:

#define REGISTER_SUBTYPE(SUBTYPE) SerializationRegistration<SUBTYPE> SUBTYPE::ourSerializationRegistration {};
