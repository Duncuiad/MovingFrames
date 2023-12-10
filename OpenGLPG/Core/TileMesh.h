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
    void RandomizeVertexColors(float aRatio);
    TileVertex::Data* GetVertexData(int aVertexIdx);

    int GetMaxHeight() const;
    bool Contains(const TileFace& aFace, const Vec2& aPosition) const;
    int GetClosestVertex(const TileFace& aFace, const Vec2& aPosition) const;
    std::pair<Array<Vec2>, Array<unsigned int>> GetMesh(int aHeight) const;
    std::pair<int, int> GetVertexAndFace(const Vec2& aPosition) const;
    Array<TileVertex::Data> GetTriangles(int aHeight, int aTriangleTypeMask = 3) const;
    Array<TileVertex::Data> GetSquares(int aHeight, int aSquareTypeMask = 3) const;

private:
    void CreateFace(int aParentFaceIdx, int aHalfEdge0, int aHalfEdge1, int aHalfEdge2, int aHalfEdge3 = -1);
    const TileHalfEdge& CreateFullEdge(int aBeginIdx, int anEndIdx, bool anIsAlternating);
    const TileVertex& CreateInflationVertex(const TileVertex& aBegin, const TileVertex& anEnd);
    void SubdivideFace(int aFaceIdx);
    void SubdivideTriangle(int aTriangleIdx);
    void SubdivideSquare(int aSquareIdx);
    void SubdivideHalfEdge(int aHalfEdgeIdx);

    int FindFirstEdge(int aBeginFromHalfEdgeIdx) const;

    Array<TileVertex> myVertices;
    Array<TileHalfEdge> myHalfEdges;
    Array<TileFace> myFaces;
};
