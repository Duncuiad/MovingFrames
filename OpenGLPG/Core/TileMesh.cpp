#include "OpenGLPG_Base.h"

#include "TileMesh.h"

#include "Assert.h"
#include "MathDefines.h"

#include <algorithm>
#include <deque>
#include <random>

namespace
{
float Deviation(const Vec2& aVector, const Vec2& anOtherVector)
{
    return aVector.x * anOtherVector.y - aVector.y * anOtherVector.x;
}
} // namespace

TileMesh::TileMesh(TileType aType)
{
    Reset(aType);
}

void TileMesh::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myVertices", myVertices);
    aSerializer.Process("myHalfEdges", myHalfEdges);
    aSerializer.Process("myFaces", myFaces);

    for (const TileFace& face : myFaces)
    {
        if (face.myParent != -1)
        {
            myFaces[face.myParent].myChildren.PushBack(face.myIndex);
        }
    }
}

void TileMesh::Reset(TileType aType)
{
    myVertices.RemoveAll();
    myHalfEdges.RemoveAll();
    myFaces.RemoveAll();

    myFaces.EmplaceBack(0, 0, aType, 0);

    myVertices.EmplaceBack(0, 0, Dodec {0, 0, 0, 0});
    myVertices.EmplaceBack(1, 0, Dodec {1, 0, 0, 0});
    if (aType == TileType::TriangleA || aType == TileType::TriangleB)
    {
        myVertices.EmplaceBack(2, 0, Dodec {1, 1, -1, 0});

        myHalfEdges.EmplaceBack(0, 0, false, true, 0, 1, -1, 0);
        myHalfEdges.EmplaceBack(1, 0, true, true, 1, 2, -1, 0);
        myHalfEdges.EmplaceBack(2, 0, true, true, 2, 0, -1, 0);
    }
    else
    {
        myVertices.EmplaceBack(2, 0, Dodec {1, 1, 0, 0});
        myVertices.EmplaceBack(3, 0, Dodec {0, 1, 0, 0});

        myHalfEdges.EmplaceBack(0, 0, false, true, 0, 1, -1, 0);
        myHalfEdges.EmplaceBack(1, 0, false, false, 1, 2, -1, 0);
        myHalfEdges.EmplaceBack(2, 0, true, true, 2, 3, -1, 0);
        myHalfEdges.EmplaceBack(3, 0, true, false, 3, 0, -1, 0);
    }

    if (aType == TileType::TriangleB)
    {
        myHalfEdges[0].myIsAlternating = false;
        myHalfEdges[1].myIsAlternating = false;
        myHalfEdges[2].myIsAlternating = false;
    }
    if (aType == TileType::SquareB)
    {
        myHalfEdges[0].myIsAlternating = false;
        myHalfEdges[1].myIsAlternating = true;
        myHalfEdges[2].myIsAlternating = false;
        myHalfEdges[3].myIsAlternating = true;
    }
    if (aType == TileType::SquareC)
    {
        myHalfEdges[2].myIsReversed = false;
        myHalfEdges[3].myIsReversed = false;
    }
}

void TileMesh::SubdivideAllFaces()
{
    const int originalFaceCount {myFaces.Count()};
    for (int i = 0; i < originalFaceCount; ++i)
    {
        if (!myFaces[i].myChildren.Count())
        {
            SubdivideFace(i);
        }
    }
}

void TileMesh::RandomizeVertexColors(float aRatio)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist {};
    for (TileVertex& vertex : myVertices)
    {
        vertex.myData.myColor = dist(e2) < aRatio;
    }
}

void TileMesh::ColorVerticesSatisfying(const TileVertex::Predicate& aPredicate)
{
    for (TileVertex& vertex : myVertices)
    {
        vertex.myData.myColor = aPredicate(vertex);
    }
}

TileVertex::Data* TileMesh::GetVertexData(int aVertexIdx)
{
    if (0 <= aVertexIdx && aVertexIdx < myVertices.Count())
    {
        return &myVertices[aVertexIdx].myData;
    }
    return nullptr;
}

TileFace::Data* TileMesh::GetFaceData(int aFaceIdx)
{
    if (0 <= aFaceIdx && aFaceIdx < myFaces.Count())
    {
        return &myFaces[aFaceIdx].myData;
    }
    return nullptr;
}

const Dodec& TileMesh::GetCoordinates(int aVertexIdx)
{
    return myVertices[aVertexIdx].myCoordinates;
}

int TileMesh::GetMaxHeight() const
{
    if (!myVertices.Count())
    {
        return -1;
    }

    return std::max_element(
               myVertices.begin(), myVertices.end(),
               [](const TileVertex& aLeft, const TileVertex& aRight) { return aLeft.myHeight < aRight.myHeight; })
        ->myHeight;
}

std::pair<Vec2, float> TileMesh::GetBoundingCircle(const TileFace& aFace) const
{
    // Upper bounds for the max distance from the center of the face to any point in any face in its subhierarchy
    constexpr float triangleUpperBound {0.59f * 0.59f};
    constexpr float squareUpperBound {0.73f * 0.73f};

    const TileHalfEdge& edge0 {myHalfEdges[aFace.myEdge]};
    const TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
    const TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};
    const Vec2& vertex0 {myVertices[edge0.myVertex].GetPosition()};
    const Vec2& vertex1 {myVertices[edge1.myVertex].GetPosition()};
    const Vec2& vertex2 {myVertices[edge2.myVertex].GetPosition()};
    Vec2 resultCenter {vertex0 + vertex1 + vertex2};
    float resultRadiusSqd {glm::dot(vertex1 - vertex0, vertex1 - vertex0)};

    if (aFace.IsTriangle())
    {
        resultCenter /= 3.f;
        resultRadiusSqd *= triangleUpperBound;
    }
    else
    {
        const TileHalfEdge& edge3 {myHalfEdges[edge2.myNext]};
        const Vec2& vertex3 {myVertices[edge3.myVertex].GetPosition()};
        resultCenter = (resultCenter + vertex3) * 0.25f;
        resultRadiusSqd *= squareUpperBound;
    }

    return {resultCenter, resultRadiusSqd};
}

bool TileMesh::Contains(const TileFace& aFace, const Vec2& aPosition) const
{
    bool isInside {true};
    const TileHalfEdge& edge0 {myHalfEdges[aFace.myEdge]};
    const TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
    const TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};
    const Vec2& vertex0 {myVertices[edge0.myVertex].GetPosition()};
    const Vec2& vertex1 {myVertices[edge1.myVertex].GetPosition()};
    const Vec2& vertex2 {myVertices[edge2.myVertex].GetPosition()};
    isInside &= Deviation(vertex1 - vertex0, aPosition - vertex0) > 0.f;
    isInside &= Deviation(vertex2 - vertex1, aPosition - vertex1) > 0.f;
    if (aFace.IsTriangle())
    {
        isInside &= Deviation(vertex0 - vertex2, aPosition - vertex2) > 0.f;
    }
    else
    {
        const TileHalfEdge& edge3 {myHalfEdges[edge2.myNext]};
        const Vec2& vertex3 {myVertices[edge3.myVertex].GetPosition()};
        isInside &= Deviation(vertex3 - vertex2, aPosition - vertex2) > 0.f;
        isInside &= Deviation(vertex0 - vertex3, aPosition - vertex3) > 0.f;
    }
    return isInside;
}

int TileMesh::GetClosestVertex(const TileFace& aFace, const Vec2& aPosition) const
{
    const TileHalfEdge& edge0 {myHalfEdges[aFace.myEdge]};
    const TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
    const TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};
    const Vec2& vertex0 {myVertices[edge0.myVertex].GetPosition()};
    const Vec2& vertex1 {myVertices[edge1.myVertex].GetPosition()};
    const Vec2& vertex2 {myVertices[edge2.myVertex].GetPosition()};

    int vertexIdx = edge0.myVertex;
    float minDistance {glm::distance(vertex0, aPosition)};
    if (const float distance = glm::distance(vertex1, aPosition); distance < minDistance)
    {
        minDistance = distance;
        vertexIdx = edge1.myVertex;
    }
    if (const float distance = glm::distance(vertex2, aPosition); distance < minDistance)
    {
        minDistance = distance;
        vertexIdx = edge2.myVertex;
    }
    if (aFace.IsSquare())
    {
        const TileHalfEdge& edge3 {myHalfEdges[edge2.myNext]};
        const Vec2& vertex3 {myVertices[edge3.myVertex].GetPosition()};
        if (const float distance = glm::distance(vertex3, aPosition); distance < minDistance)
        {
            minDistance = distance;
            vertexIdx = edge3.myVertex;
        }
    }
    return vertexIdx;
}

std::pair<Array<Vec2>, Array<unsigned int>> TileMesh::GetMesh(int aHeight) const
{
    Array<Vec2> vertices;
    Array<unsigned int> indices;

    std::unordered_map<int, int> vertexInverseMapping;

    for (const TileVertex& vertex : myVertices)
    {
        if (vertex.myHeight <= aHeight)
        {
            vertexInverseMapping[vertex.myIndex] = vertices.Count();
            vertices.PushBack(vertex.GetPosition());
        }
    }

    for (const TileFace& face : myFaces)
    {
        if (face.myHeight != aHeight)
        {
            continue;
        }

        const TileHalfEdge& edge0 {myHalfEdges[face.myEdge]};
        const TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
        const TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};

        indices.PushBack(vertexInverseMapping[edge0.myVertex]);
        indices.PushBack(vertexInverseMapping[edge1.myVertex]);
        indices.PushBack(vertexInverseMapping[edge2.myVertex]);

        if (face.IsSquare())
        {
            const TileHalfEdge& edge3 {myHalfEdges[edge2.myNext]};
            indices.PushBack(vertexInverseMapping[edge2.myVertex]);
            indices.PushBack(vertexInverseMapping[edge3.myVertex]);
            indices.PushBack(vertexInverseMapping[edge0.myVertex]);
        }
    }

    return std::make_pair(std::move(vertices), std::move(indices));
}

std::pair<int, int> TileMesh::GetVertexAndFace(const Vec2& aPosition) const
{
    if (myFaces.Count() == 0)
    {
        return {-1, -1};
    }

    int maxHeight {-1};
    int faceIdx {-1};
    int vertexIdx {-1};
    std::deque<int> faceQueue;
    ASSERT(myFaces.GetFirst().myHeight == 0, "Missing root face");
    faceQueue.push_back(myFaces.GetFirst().myIndex);

    while (!faceQueue.empty())
    {
        const TileFace& face {myFaces[faceQueue.front()]};
        faceQueue.pop_front();

        if (Contains(face, aPosition))
        {
            if (face.myHeight > maxHeight)
            {
                faceIdx = face.myIndex;
            }
        }

        for (const int childIdx : face.myChildren)
        {
            const TileFace& child {myFaces[childIdx]};
            const auto [center, radiusSqd] = GetBoundingCircle(child);
            if (glm::dot(aPosition - center, aPosition - center) < radiusSqd)
            {
                faceQueue.push_back(child.myIndex);
            }
        }
    }

    if (faceIdx != -1)
    {
        vertexIdx = GetClosestVertex(myFaces[faceIdx], aPosition);
    }
    return {vertexIdx, faceIdx};
}

std::vector<std::reference_wrapper<const TileVertex>> TileMesh::GetTriangles(int aHeight, int aTriangleTypeMask) const
{
    std::vector<std::reference_wrapper<const TileVertex>> vertices;

    for (const TileFace& face : myFaces)
    {
        if (face.myHeight != aHeight || !face.IsTriangle() ||
            (face.myType == TileType::TriangleA && !(aTriangleTypeMask & 1)) ||
            (face.myType == TileType::TriangleB && !(aTriangleTypeMask & 2)))
        {
            continue;
        }

        const TileHalfEdge& edge0 {myHalfEdges[face.myEdge]};
        const TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
        const TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};

        vertices.push_back(myVertices[edge0.myVertex]);
        vertices.push_back(myVertices[edge1.myVertex]);
        vertices.push_back(myVertices[edge2.myVertex]);
    }

    return vertices;
}

std::vector<std::reference_wrapper<const TileVertex>> TileMesh::GetSquares(int aHeight, int aTriangleTypeMask) const
{
    std::vector<std::reference_wrapper<const TileVertex>> vertices;

    for (const TileFace& face : myFaces)
    {
        if (face.myHeight != aHeight || !face.IsSquare() ||
            (face.myType == TileType::SquareA && !(aTriangleTypeMask & 1)) ||
            (face.myType == TileType::SquareB && !(aTriangleTypeMask & 2)) ||
            (face.myType == TileType::SquareC && !(aTriangleTypeMask & 4)))
        {
            continue;
        }

        const TileHalfEdge& edge0 {myHalfEdges[face.myEdge]};
        const TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
        const TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};
        const TileHalfEdge& edge3 {myHalfEdges[edge2.myNext]};

        vertices.push_back(myVertices[edge0.myVertex]);
        vertices.push_back(myVertices[edge1.myVertex]);
        vertices.push_back(myVertices[edge2.myVertex]);
        vertices.push_back(myVertices[edge3.myVertex]);
    }

    return vertices;
}

void TileMesh::CreateFace(int aParentFaceIdx, int aHalfEdge0, int aHalfEdge1, int aHalfEdge2, int aHalfEdge3 /*= -1*/)
{
    TileHalfEdge& edge0 {myHalfEdges[aHalfEdge0]};
    TileHalfEdge& edge1 {myHalfEdges[aHalfEdge1]};
    TileHalfEdge& edge2 {myHalfEdges[aHalfEdge2]};

    ASSERT(edge0.myHeight == edge1.myHeight && edge1.myHeight == edge2.myHeight && edge2.myHeight == edge0.myHeight,
           "Faces are made of edges at the same height");

    const int faceIdx {myFaces.Count()};
    const TileType type {edge0.myIsAlternating ? TileType::TriangleA : TileType::TriangleB};
    myFaces.EmplaceBack(faceIdx, edge0.myHeight, type, -1);
    edge0.myFace = faceIdx;
    edge1.myFace = faceIdx;
    edge2.myFace = faceIdx;
    edge0.myNext = edge1.myIndex;
    edge1.myNext = edge2.myIndex;
    edge2.myNext = edge0.myIndex;

    if (aHalfEdge3 != -1)
    {
        TileHalfEdge& edge3 {myHalfEdges[aHalfEdge3]};
        edge3.myFace = faceIdx;
        edge2.myNext = edge3.myIndex;
        edge3.myNext = edge0.myIndex;

        TileType squareType {edge0.myIsReversed || edge1.myIsReversed || edge2.myIsReversed ? TileType::SquareA
                                                                                            : TileType::SquareC};
        myFaces[faceIdx].myType = squareType;
    }
    myFaces[faceIdx].myEdge = FindFirstEdge(edge0.myIndex);
    myFaces[faceIdx].myParent = myFaces[aParentFaceIdx].myIndex;
    myFaces[aParentFaceIdx].myChildren.PushBack(faceIdx);
}

const TileHalfEdge& TileMesh::CreateFullEdge(int aBeginIdx, int anEndIdx, bool anIsAlternating)
{
    const int height = glm::max(myVertices[aBeginIdx].myHeight, myVertices[anEndIdx].myHeight);
    const int newIndex0 {myHalfEdges.Count()};
    const int newIndex1 {newIndex0 + 1};
    myHalfEdges.EmplaceBack(newIndex0, height, false, anIsAlternating, aBeginIdx, -1, newIndex1, -1);
    myHalfEdges.EmplaceBack(newIndex1, height, true, anIsAlternating, anEndIdx, -1, newIndex0, -1);

    return myHalfEdges[newIndex0];
}

const TileVertex& TileMesh::CreateInflationVertex(const TileVertex& aBegin, const TileVertex& anEnd)
{
    const Dodec coordinates {aBegin.myCoordinates + (anEnd.myCoordinates - aBegin.myCoordinates) * Dodec::N()};
    myVertices.EmplaceBack(myVertices.Count(), glm::max(aBegin.myHeight, anEnd.myHeight) + 1, coordinates);
    TileVertex& vertex {myVertices.GetLast()};
    return vertex;
}

void TileMesh::SubdivideFace(int aFaceIdx)
{
    if (myFaces[aFaceIdx].IsTriangle())
    {
        SubdivideTriangle(aFaceIdx);
    }
    else
    {
        SubdivideSquare(aFaceIdx);
    }
}

void TileMesh::SubdivideTriangle(int aTriangleIdx)
{
    const TileFace& triangle {myFaces[aTriangleIdx]};
    const int edge0Idx {triangle.myEdge};
    const int edge1Idx {myHalfEdges[edge0Idx].myNext};
    const int edge2Idx {myHalfEdges[edge1Idx].myNext};

    SubdivideHalfEdge(edge0Idx);
    SubdivideHalfEdge(edge1Idx);
    SubdivideHalfEdge(edge2Idx);

    const int vertex0Idx {myHalfEdges[myHalfEdges[edge0Idx].myRightChild].myVertex};
    const int vertex1Idx {myHalfEdges[myHalfEdges[edge1Idx].myRightChild].myVertex};
    const int vertex2Idx {myHalfEdges[myHalfEdges[edge2Idx].myRightChild].myVertex};

    const int edgeAIdx {CreateFullEdge(vertex0Idx, vertex1Idx, triangle.myType == TileType::TriangleA).myIndex};
    const int edgeBIdx {CreateFullEdge(vertex2Idx, vertex0Idx, triangle.myType == TileType::TriangleB).myIndex};

    CreateFace(aTriangleIdx, myHalfEdges[edge0Idx].myRightChild, myHalfEdges[edge1Idx].myLeftChild,
               myHalfEdges[edgeAIdx].myOpposite);
    CreateFace(aTriangleIdx, myHalfEdges[edge0Idx].myLeftChild, myHalfEdges[edgeBIdx].myOpposite,
               myHalfEdges[edge2Idx].myRightChild);
    CreateFace(aTriangleIdx, myHalfEdges[edgeAIdx].myIndex, myHalfEdges[edge1Idx].myRightChild,
               myHalfEdges[edge2Idx].myLeftChild, myHalfEdges[edgeBIdx].myIndex);
}

void TileMesh::SubdivideSquare(int aSquareIdx)
{
    const TileFace& square {myFaces[aSquareIdx]};
    const int edge0Idx {square.myEdge};
    const int edge1Idx {myHalfEdges[edge0Idx].myNext};
    const int edge2Idx {myHalfEdges[edge1Idx].myNext};
    const int edge3Idx {myHalfEdges[edge2Idx].myNext};

    SubdivideHalfEdge(edge0Idx);
    SubdivideHalfEdge(edge1Idx);
    SubdivideHalfEdge(edge2Idx);
    SubdivideHalfEdge(edge3Idx);

    const int vertex0Idx {myHalfEdges[myHalfEdges[edge0Idx].myRightChild].myVertex};
    const int vertex1Idx {myHalfEdges[myHalfEdges[edge1Idx].myRightChild].myVertex};
    const int vertex2Idx {myHalfEdges[myHalfEdges[edge2Idx].myRightChild].myVertex};
    const int vertex3Idx {myHalfEdges[myHalfEdges[edge3Idx].myRightChild].myVertex};

    if (square.myType == TileType::SquareC)
    {
        const int edgeAIdx {CreateFullEdge(vertex0Idx, vertex1Idx, true).myIndex};
        const int edgeBIdx {CreateFullEdge(vertex1Idx, vertex2Idx, false).myIndex};
        const int edgeCIdx {CreateFullEdge(vertex2Idx, vertex3Idx, true).myIndex};
        const int edgeDIdx {CreateFullEdge(vertex3Idx, vertex0Idx, false).myIndex};

        CreateFace(aSquareIdx, myHalfEdges[edge0Idx].myRightChild, myHalfEdges[edge1Idx].myLeftChild,
                   myHalfEdges[edgeAIdx].myOpposite);
        CreateFace(aSquareIdx, myHalfEdges[edge1Idx].myRightChild, myHalfEdges[edge2Idx].myLeftChild,
                   myHalfEdges[edgeBIdx].myOpposite);
        CreateFace(aSquareIdx, myHalfEdges[edge2Idx].myRightChild, myHalfEdges[edge3Idx].myLeftChild,
                   myHalfEdges[edgeCIdx].myOpposite);
        CreateFace(aSquareIdx, myHalfEdges[edge3Idx].myRightChild, myHalfEdges[edge0Idx].myLeftChild,
                   myHalfEdges[edgeDIdx].myOpposite);
        CreateFace(aSquareIdx, myHalfEdges[edgeAIdx].myIndex, myHalfEdges[edgeBIdx].myIndex,
                   myHalfEdges[edgeCIdx].myIndex, myHalfEdges[edgeDIdx].myIndex);
    }
    else
    {
        const TileVertex& begin {myVertices[myHalfEdges[edge0Idx].myVertex]};
        const TileVertex& end {myVertices[myHalfEdges[edge2Idx].myVertex]};
        const int vertex4Idx {CreateInflationVertex(begin, end).myIndex};
        const int vertex5Idx {myHalfEdges[edge3Idx].myVertex};

        const bool isTypeA {square.myType == TileType::SquareA};
        const int edgeAIdx {CreateFullEdge(vertex0Idx, vertex1Idx, isTypeA).myIndex};
        const int edgeBIdx {CreateFullEdge(vertex4Idx, vertex1Idx, isTypeA).myIndex};
        const int edgeCIdx {CreateFullEdge(vertex0Idx, vertex4Idx, isTypeA).myIndex};
        const int edgeDIdx {CreateFullEdge(vertex2Idx, vertex4Idx, !isTypeA).myIndex};
        const int edgeFIdx {CreateFullEdge(vertex4Idx, vertex3Idx, !isTypeA).myIndex};

        CreateFace(aSquareIdx, myHalfEdges[edge0Idx].myRightChild, myHalfEdges[edge1Idx].myLeftChild,
                   myHalfEdges[edgeAIdx].myOpposite);
        CreateFace(aSquareIdx, myHalfEdges[edgeAIdx].myIndex, myHalfEdges[edgeBIdx].myOpposite,
                   myHalfEdges[edgeCIdx].myOpposite);
        CreateFace(aSquareIdx, myHalfEdges[edgeBIdx].myIndex, myHalfEdges[edge1Idx].myRightChild,
                   myHalfEdges[edge2Idx].myLeftChild, myHalfEdges[edgeDIdx].myIndex);
        CreateFace(aSquareIdx, myHalfEdges[edgeCIdx].myIndex, myHalfEdges[edgeFIdx].myIndex,
                   myHalfEdges[edge3Idx].myRightChild, myHalfEdges[edge0Idx].myLeftChild);

        if (isTypeA)
        {
            const int edgeEIdx {CreateFullEdge(vertex4Idx, vertex5Idx, false).myIndex};
            CreateFace(aSquareIdx, myHalfEdges[edge2Idx].myRightChild, myHalfEdges[edgeEIdx].myOpposite,
                       myHalfEdges[edgeDIdx].myOpposite);
            CreateFace(aSquareIdx, myHalfEdges[edgeEIdx].myIndex, myHalfEdges[edge3Idx].myLeftChild,
                       myHalfEdges[edgeFIdx].myOpposite);
        }
        else
        {
            const int edgeEIdx {CreateFullEdge(vertex5Idx, vertex4Idx, true).myIndex};
            CreateFace(aSquareIdx, myHalfEdges[edgeEIdx].myIndex, myHalfEdges[edgeDIdx].myOpposite,
                       myHalfEdges[edge2Idx].myRightChild);
            CreateFace(aSquareIdx, myHalfEdges[edge3Idx].myLeftChild, myHalfEdges[edgeFIdx].myOpposite,
                       myHalfEdges[edgeEIdx].myOpposite);
        }
    }
}

void TileMesh::SubdivideHalfEdge(int aHalfEdgeIdx)
{
    const int newHeight {myHalfEdges[aHalfEdgeIdx].myHeight + 1};
    const bool isReversed {myHalfEdges[aHalfEdgeIdx].myIsReversed};
    const bool isAlternating {myHalfEdges[aHalfEdgeIdx].myIsAlternating};
    myHalfEdges.EmplaceBack(myHalfEdges.Count(), newHeight, isReversed != isAlternating, isReversed == isAlternating,
                            myHalfEdges[aHalfEdgeIdx].myVertex, -1, -1, -1);
    myHalfEdges.EmplaceBack(myHalfEdges.Count(), newHeight, isReversed != isAlternating, isReversed != isAlternating,
                            -1, -1, -1, -1);
    TileHalfEdge& halfEdge {myHalfEdges[aHalfEdgeIdx]};

    TileHalfEdge& leftChild {myHalfEdges[myHalfEdges.Count() - 2]};
    halfEdge.myLeftChild = leftChild.myIndex;
    leftChild.myParent = halfEdge.myIndex;
    TileHalfEdge& rightChild {myHalfEdges.GetLast()};
    halfEdge.myRightChild = rightChild.myIndex;
    rightChild.myParent = halfEdge.myIndex;

    int& vertexIdx {rightChild.myVertex};
    if (halfEdge.myOpposite != -1)
    {
        const TileHalfEdge& oppositeHEdge {myHalfEdges[halfEdge.myOpposite]};
        if (oppositeHEdge.myLeftChild != -1)
        {
            TileHalfEdge& oppositeLeftChild {myHalfEdges[oppositeHEdge.myLeftChild]};
            TileHalfEdge& oppositeRightChild {myHalfEdges[oppositeHEdge.myRightChild]};
            oppositeLeftChild.myOpposite = rightChild.myIndex;
            oppositeRightChild.myOpposite = leftChild.myIndex;
            leftChild.myOpposite = oppositeRightChild.myIndex;
            rightChild.myOpposite = oppositeLeftChild.myIndex;
            vertexIdx = oppositeRightChild.myVertex;
        }
    }
    if (vertexIdx == -1)
    {
        const TileHalfEdge& nextHEdge {myHalfEdges[halfEdge.myNext]};
        vertexIdx = myVertices.Count();
        if (halfEdge.myIsReversed)
        {
            CreateInflationVertex(myVertices[nextHEdge.myVertex], myVertices[halfEdge.myVertex]);
        }
        else
        {
            CreateInflationVertex(myVertices[halfEdge.myVertex], myVertices[nextHEdge.myVertex]);
        }
    }
}

int TileMesh::FindFirstEdge(int aBeginFromHalfEdgeIdx) const
{
    const TileFace& face {myFaces[myHalfEdges[aBeginFromHalfEdgeIdx].myFace]};
    const TileType type {face.myType};

    int firstEdgeIdx {aBeginFromHalfEdgeIdx};
    if (type == TileType::SquareC)
    {
        if (!myHalfEdges[firstEdgeIdx].myIsAlternating)
        {
            firstEdgeIdx = myHalfEdges[firstEdgeIdx].myNext;
        }
    }
    else if (type == TileType::SquareA || type == TileType::SquareB)
    {
        while (myHalfEdges[firstEdgeIdx].myIsReversed || myHalfEdges[myHalfEdges[firstEdgeIdx].myNext].myIsReversed)
        {
            firstEdgeIdx = myHalfEdges[firstEdgeIdx].myNext;
        }
    }
    else
    {
        while (myHalfEdges[firstEdgeIdx].myIsReversed)
        {
            firstEdgeIdx = myHalfEdges[firstEdgeIdx].myNext;
        }
    }

    return firstEdgeIdx;
}
