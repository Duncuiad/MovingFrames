#pragma once

#include "ClientLoader.h"
#include "Serializable.h"

#include <memory>

class Component : public SerializableDynamic
{
public:
    using Ptr = std::unique_ptr<Component>;

    struct LoadParams
    {
        const ClientLoader& myClientLoader;
    };

    virtual void OnLoad(const LoadParams& someParams) {}
    virtual void OnEnterWorld() {}
    virtual void Update() {}
    virtual void OnExitWorld() {}
    virtual void OnUnload() {}
};

inline SerializableDynamic* Unwrap(const Component::Ptr& aPtr)
{
    return aPtr.get();
}

template <>
inline Component::Ptr Wrap<Component::Ptr>(SerializableDynamic* aPtr)
{
    return Component::Ptr {static_cast<Component*>(aPtr)};
}
