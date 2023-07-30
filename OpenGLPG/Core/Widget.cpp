#include "OpenGLPG_Base.h"

#include "Widget.h"

void WidgetList::Draw()
{
    PreDraw();
    DrawChildren();
    PostDraw();
}

void WidgetList::Connect(Widget* aWidget)
{
    myChildren.emplace_back(aWidget);
}

void WidgetList::DrawChildren()
{
    for (const UniquePtr& child : myChildren)
    {
        child->Draw();
    }
}

Widget::Widget()
    : myUID {UID::CreateUnique()}
{}
