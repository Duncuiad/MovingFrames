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
