#pragma once

#include "Component.h"
#include "EditorWidget.h"

#include <type_traits>

class EditorComponent : public Component
{
public:
    virtual void DrawWidget() = 0;
    bool myIsDisplayingWidget {false};
};

template <typename EditorWidgetT>
class EditorCmp : public EditorComponent
{
    static_assert(std::is_base_of_v<EditorWidget, EditorWidgetT>);

public:
    void DrawWidget() override;

protected:
    EditorWidgetT myWidget;
};

template <typename EditorWidgetT>
inline void EditorCmp<EditorWidgetT>::DrawWidget()
{
    myWidget.Draw();
}
