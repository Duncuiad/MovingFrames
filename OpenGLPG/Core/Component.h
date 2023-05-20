#pragma once

#include <memory>

class Component
{
public:
    using Ptr = std::unique_ptr<Component>;

    virtual void OnLoad() {}
    virtual void OnEnterWorld() {}
    virtual void Update() = 0;
    virtual void OnExitWorld() {}
    virtual void OnUnload() {}
};
