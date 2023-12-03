#pragma once

#include "Array.h"
#include "TileFace.h"
#include "TileHalfEdge.h"
#include "TileVertex.h"

class TileMesh
{
    TileMesh() = default;
    TileMesh(TileType aType);

private:
    void SubdivideFace(int aFaceIdx);
    void SubdivideHalfEdge(int aHalfEdgeIdx);

    Array<TileVertex> myVertices;
    Array<TileHalfEdge> myHalfEdges;
    Array<TileFace> myFaces;
};
