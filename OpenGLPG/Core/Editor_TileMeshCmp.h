#pragma once

#include "EditorCmp.h"
#include "TileMeshEditorWidget.h"

class TileMeshCmp;
class DynamicMeshGraphCmp;
class TileMeshColliderCmp;

class Editor_TileMeshCmp : public EditorCmp<TileMeshEditorWidget>
{
    DECLARE_SUBTYPE(Editor_TileMeshCmp)

public:
    void Serialize(Serializer& /*aSerializer*/) override {}
    void OnEnterWorld() override;
    void Update() override;
    void OnChanged() const;

private:
    TileMeshCmp& GetTileMeshCmp() const;
    DynamicMeshGraphCmp& GetDynamicMeshGraphCmp() const;
    TileMeshColliderCmp& GetTileMeshColliderCmp() const;
};
