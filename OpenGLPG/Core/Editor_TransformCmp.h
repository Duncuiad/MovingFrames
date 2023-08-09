#pragma once

#include "EditorCmp.h"
#include "TransformEditorWidget.h"

class TransformCmp;

class Editor_TransformCmp : public EditorCmp<TransformEditorWidget>
{
    DECLARE_SUBTYPE(Editor_TransformCmp)

public:
    void Serialize(Serializer& /*aSerializer*/) override {}
    void OnEnterWorld() override;
    void OnChanged() const;

private:
    TransformCmp& GetTransformCmp() const;
};
