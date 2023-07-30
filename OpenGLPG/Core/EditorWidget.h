#pragma once

#include "Widget.h"

#include <functional>

class EditorWidget : public Widget
{
public:
    using OnChangedCallback = std::function<void(void)>;
    void ConnectOnChangedCallback(const OnChangedCallback& anOnChanged);

protected:
    OnChangedCallback myOnChanged {nullptr};
};
