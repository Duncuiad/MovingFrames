#pragma once

#include "ClientLoader.h"

#include <memory>

class Component
{
public:
    using Ptr = std::unique_ptr<Component>;

    struct LoadParams
    {
        const ClientLoader& myClientLoader;
    };

    virtual void OnLoad(const LoadParams& someParams) {}
    virtual void OnEnterWorld() {}
    virtual void Update() = 0;
    virtual void OnExitWorld() {}
    virtual void OnUnload() {}
};
