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
    void CreateFace(TileFace& aParentFace, int aHalfEdge0, int aHalfEdge1, int aHalfEdge2, int aHalfEdge3 = -1);
    const TileHalfEdge& CreateFullEdge(const TileVertex& aBegin, const TileVertex& anEnd, bool anIsAlternating);
    const TileVertex& CreateInflationVertex(const TileVertex& aBegin, const TileVertex& anEnd);
    void SubdivideFace(TileFace& aFace);
    void SubdivideTriangle(TileFace& aTriangle);
    void SubdivideSquare(TileFace& aSquare);
    void SubdivideHalfEdge(TileHalfEdge& aHalfEdge);

    int FindFirstEdge(int aBeginFromHalfEdgeIdx) const;

    Array<TileVertex> myVertices;
    Array<TileHalfEdge> myHalfEdges;
    Array<TileFace> myFaces;
};
