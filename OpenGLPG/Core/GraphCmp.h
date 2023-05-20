#pragma once

#include "Component.h"
#include "Mesh.h"

class GraphCmp : public Component
{
    void Update() override {}
    void Draw();

private:
    Mesh myMesh;
};
