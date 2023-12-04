#pragma once

#include "Array.h"
#include "MathDefines.h"
#include "Serializable.h"
#include "TileFace.h"
#include "TileHalfEdge.h"
#include "TileVertex.h"

#include <utility>

class TileMesh : public Serializable
{
public:
    TileMesh() = default;
    TileMesh(TileType aType);

    void Serialize(Serializer& aSerializer) override;

    void Reset(TileType aType);
    void SubdivideAllFaces();

    std::pair<Array<Vec2>, Array<unsigned int>> GetMesh(int aHeight) const;

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
