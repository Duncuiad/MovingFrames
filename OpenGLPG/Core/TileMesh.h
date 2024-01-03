#pragma once

#include "Array.h"
#include "Dodec.h"
#include "MathDefines.h"
#include "Serializable.h"
#include "TileFace.h"
#include "TileHalfEdge.h"
#include "TileVertex.h"

#include <functional>
#include <utility>
#include <vector>

class TileMesh : public Serializable
{
public:
    TileMesh() = default;
    TileMesh(TileType aType);

    void Serialize(Serializer& aSerializer) override;

    void Reset(TileType aType);
    void SubdivideAllFaces();
    void RandomizeVertexColors(float aRatio);
    void ColorVertices(const TileVertex::Evaluation& anEvaluation);
    void ColorVerticesSatisfying(const TileVertex::Predicate& aPredicate);
    TileVertex::Data* GetVertexData(const Dodec& someCoords);
    TileFace::Data* GetFaceData(int aFaceIdx);

    int GetMaxHeight() const;
    std::pair<Vec2, float> GetBoundingCircle(const TileFace& aFace) const;
    bool Contains(const TileFace& aFace, const Vec2& aPosition) const;
    Dodec GetClosestVertex(const TileFace& aFace, const Vec2& aPosition) const;
    std::pair<Dodec, int> GetVertexAndFace(const Vec2& aPosition, int aFaceHeight = -1) const;
    Array<Dodec> GetNeighbouringVertices(const Dodec& aVertex, int aHeight);
    const TileVertex::Map& GetVertices() const;
    const Array<TileHalfEdge>& GetEdges() const;
    const Array<TileFace>& GetFaces() const;

private:
    void CreateFace(int aParentFaceIdx, int aHalfEdge0, int aHalfEdge1, int aHalfEdge2, int aHalfEdge3 = -1);
    const TileHalfEdge& CreateFullEdge(const Dodec& aBegin, const Dodec& anEnd, bool anIsAlternating);
    Dodec CreateInflationVertex(const Dodec& aBegin, const Dodec& anEnd);
    void SubdivideFace(int aFaceIdx);
    void SubdivideTriangle(int aTriangleIdx);
    void SubdivideSquare(int aSquareIdx);
    void SubdivideHalfEdge(int aHalfEdgeIdx);

    int FindFirstEdge(int aBeginFromHalfEdgeIdx) const;

    TileVertex::Map myVertices;
    Array<TileHalfEdge> myHalfEdges;
    Array<TileFace> myFaces;
};
