#pragma once

#include "EditorWidget.h"
#include "TileMesh.h"

class TileMeshEditorWidget : public EditorWidget
{
public:
    void AttachTileMeshObject(TileMesh& aTileMesh);
    void Draw() override;

    TileMesh* myTileMesh {nullptr};
    int myHeightToDisplay {0};
};
