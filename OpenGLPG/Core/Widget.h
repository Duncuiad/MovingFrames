#pragma once

#include "Array.h"
#include "UID.h"

#include <memory>

class Widget
{
public:
    Widget();
    virtual void Draw() = 0;

protected:
    const UID myUID;
};

class WidgetList : public Widget
{
public:
    void Draw() override;
    void Connect(Widget* aWidget);

protected:
    virtual void PreDraw() {}
    virtual void PostDraw() {}

private:
    void DrawChildren();

    using UniquePtr = std::unique_ptr<Widget>;
    using List = std::vector<UniquePtr>;
    List myChildren;
};
