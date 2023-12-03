#include "OpenGLPG_Base.h"

#include "TileMesh.h"

#include "Assert.h"
#include "MathDefines.h"

TileMesh::TileMesh(TileType aType)
{
    myFaces.EmplaceBack(0, 0, aType, 0);

    myVertices.EmplaceBack(0, 0, Vec2 {0.f, 0.f});
    myVertices.EmplaceBack(1, 0, Vec2 {1.f, 0.f});
    if (aType == TileType::TriangleA || aType == TileType::TriangleB)
    {
        constexpr float sqrt3over2 {0.86602540378f};
        myVertices.EmplaceBack(2, 0, Vec2 {0.5f, sqrt3over2});

        myHalfEdges.EmplaceBack(0, 0, false, true, 0, 1, -1, 0);
        myHalfEdges.EmplaceBack(1, 0, true, true, 1, 2, -1, 0);
        myHalfEdges.EmplaceBack(2, 0, true, true, 2, 0, -1, 0);
    }
    else
    {
        myVertices.EmplaceBack(2, 0, Vec2 {0.f, 1.f});
        myVertices.EmplaceBack(3, 0, Vec2 {1.f, 1.f});

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

void TileMesh::CreateFace(TileFace& aParentFace, int aHalfEdge0, int aHalfEdge1, int aHalfEdge2,
                          int aHalfEdge3 /*= -1*/)
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
        TileType squareType {edge3.myIsReversed ? edge0.myIsAlternating ? TileType::SquareA : TileType::SquareB
                                                : TileType::SquareC};
        myFaces[faceIdx].myType = squareType;
    }
    myFaces[faceIdx].myEdge = FindFirstEdge(edge0.myIndex);
    myFaces[faceIdx].myParent = aParentFace.myIndex;
    aParentFace.myChildren.PushBack(faceIdx);
}

const TileHalfEdge& TileMesh::CreateFullEdge(const TileVertex& aBegin, const TileVertex& anEnd, bool anIsAlternating)
{
    const int height = glm::max(aBegin.myHeight, anEnd.myHeight);
    const int newIndex0 {myHalfEdges.Count()};
    const int newIndex1 {newIndex0 + 1};
    myHalfEdges.EmplaceBack(newIndex0, height, false, anIsAlternating, aBegin.myIndex, -1, newIndex1, -1);
    myHalfEdges.EmplaceBack(newIndex1, height, true, anIsAlternating, anEnd.myIndex, -1, newIndex0, -1);

    return myHalfEdges[newIndex0];
}

const TileVertex& TileMesh::CreateInflationVertex(const TileVertex& aBegin, const TileVertex& anEnd)
{
    ASSERT(aBegin.myHeight == anEnd.myHeight, "Inflating an Edge between Vertices at different heights");
    const Vec2& fromPosition {aBegin.myPosition};
    const Vec2& toPosition {anEnd.myPosition};

    constexpr float sinOfPiOverTwelve {0.25881904510252f};
    const Vec2 orthogonalComponent {toPosition.y - fromPosition.y, fromPosition.x - toPosition.x};
    const Vec2 newVertexPosition {(fromPosition + toPosition - sinOfPiOverTwelve * orthogonalComponent) * 0.5f};
    myVertices.EmplaceBack(myVertices.Count(), aBegin.myHeight + 1, newVertexPosition);
    return myVertices.GetLast();
}

void TileMesh::SubdivideFace(TileFace& aFace)
{
    if (aFace.myType == TileType::TriangleA || aFace.myType == TileType::TriangleB)
    {
        SubdivideTriangle(aFace);
    }
    else
    {
        SubdivideSquare(aFace);
    }
}

void TileMesh::SubdivideTriangle(TileFace& aTriangle)
{
    TileHalfEdge& edge0 {myHalfEdges[aTriangle.myEdge]};
    TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
    TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};

    SubdivideHalfEdge(edge0);
    SubdivideHalfEdge(edge1);
    SubdivideHalfEdge(edge2);

    const TileVertex& vertex0 {myVertices[myHalfEdges[edge0.myRightChild].myVertex]};
    const TileVertex& vertex1 {myVertices[myHalfEdges[edge1.myRightChild].myVertex]};
    const TileVertex& vertex2 {myVertices[myHalfEdges[edge2.myRightChild].myVertex]};

    const TileHalfEdge& edgeA {CreateFullEdge(vertex0, vertex1, aTriangle.myType == TileType::TriangleA)};
    const TileHalfEdge& edgeB {CreateFullEdge(vertex2, vertex0, aTriangle.myType == TileType::TriangleB)};

    CreateFace(aTriangle, edge0.myRightChild, edge1.myLeftChild, edgeA.myOpposite);
    CreateFace(aTriangle, edge0.myLeftChild, edgeB.myOpposite, edge2.myRightChild);
    CreateFace(aTriangle, edgeA.myIndex, edge1.myRightChild, edge2.myLeftChild, edgeB.myIndex);
}

void TileMesh::SubdivideSquare(TileFace& aSquare)
{
    TileHalfEdge& edge0 {myHalfEdges[aSquare.myEdge]};
    TileHalfEdge& edge1 {myHalfEdges[edge0.myNext]};
    TileHalfEdge& edge2 {myHalfEdges[edge1.myNext]};
    TileHalfEdge& edge3 {myHalfEdges[edge2.myNext]};

    SubdivideHalfEdge(edge0);
    SubdivideHalfEdge(edge1);
    SubdivideHalfEdge(edge2);
    SubdivideHalfEdge(edge3);

    const TileVertex& vertex0 {myVertices[myHalfEdges[edge0.myRightChild].myVertex]};
    const TileVertex& vertex1 {myVertices[myHalfEdges[edge1.myRightChild].myVertex]};
    const TileVertex& vertex2 {myVertices[myHalfEdges[edge2.myRightChild].myVertex]};
    const TileVertex& vertex3 {myVertices[myHalfEdges[edge3.myRightChild].myVertex]};

    if (aSquare.myType == TileType::SquareC)
    {
        const TileHalfEdge& edgeA {CreateFullEdge(vertex0, vertex1, true)};
        const TileHalfEdge& edgeB {CreateFullEdge(vertex1, vertex2, false)};
        const TileHalfEdge& edgeC {CreateFullEdge(vertex2, vertex3, true)};
        const TileHalfEdge& edgeD {CreateFullEdge(vertex3, vertex0, false)};

        CreateFace(aSquare, edge0.myRightChild, edge1.myLeftChild, edgeA.myOpposite);
        CreateFace(aSquare, edge1.myRightChild, edge2.myLeftChild, edgeB.myOpposite);
        CreateFace(aSquare, edge2.myRightChild, edge3.myLeftChild, edgeC.myOpposite);
        CreateFace(aSquare, edge3.myRightChild, edge0.myLeftChild, edgeD.myOpposite);
        CreateFace(aSquare, edgeA.myIndex, edgeB.myIndex, edgeC.myIndex, edgeD.myIndex);
    }
    else
    {
        const TileVertex& begin {myVertices[edge0.myVertex]};
        const TileVertex& end {myVertices[edge2.myVertex]};
        const TileVertex& vertex4 {CreateInflationVertex(begin, end)};
        const TileVertex& vertex5 {myVertices[edge3.myVertex]};

        const bool isTypeA {aSquare.myType == TileType::SquareA};
        const TileHalfEdge& edgeA {CreateFullEdge(vertex0, vertex1, isTypeA)};
        const TileHalfEdge& edgeB {CreateFullEdge(vertex4, vertex1, isTypeA)};
        const TileHalfEdge& edgeC {CreateFullEdge(vertex0, vertex4, isTypeA)};
        const TileHalfEdge& edgeD {CreateFullEdge(vertex2, vertex4, !isTypeA)};
        const TileHalfEdge& edgeF {CreateFullEdge(vertex4, vertex3, !isTypeA)};

        CreateFace(aSquare, edge0.myRightChild, edge1.myLeftChild, edgeA.myOpposite);
        CreateFace(aSquare, edgeA.myIndex, edgeB.myOpposite, edgeC.myOpposite);
        CreateFace(aSquare, edgeB.myIndex, edge1.myRightChild, edge2.myLeftChild, edgeD.myIndex);
        CreateFace(aSquare, edgeC.myIndex, edgeF.myIndex, edge3.myRightChild, edge0.myLeftChild);

        if (isTypeA)
        {
            const TileHalfEdge& edgeE {CreateFullEdge(vertex4, vertex5, false)};
            CreateFace(aSquare, edge2.myRightChild, edgeE.myOpposite, edgeD.myOpposite);
            CreateFace(aSquare, edgeE.myIndex, edge3.myLeftChild, edgeF.myOpposite);
        }
        else
        {
            const TileHalfEdge& edgeE {CreateFullEdge(vertex5, vertex4, true)};
            CreateFace(aSquare, edgeE.myIndex, edgeD.myOpposite, edge2.myRightChild);
            CreateFace(aSquare, edge3.myLeftChild, edgeF.myOpposite, edgeE.myOpposite);
        }
    }
}

void TileMesh::SubdivideHalfEdge(TileHalfEdge& aHalfEdge)
{
    myHalfEdges.EmplaceBack(myHalfEdges.Count(), aHalfEdge.myHeight + 1,
                            aHalfEdge.myIsReversed != aHalfEdge.myIsAlternating,
                            aHalfEdge.myIsReversed == aHalfEdge.myIsAlternating, aHalfEdge.myVertex, -1, -1, -1);
    TileHalfEdge& leftChild {myHalfEdges.GetLast()};
    aHalfEdge.myLeftChild = leftChild.myIndex;
    leftChild.myParent = aHalfEdge.myIndex;

    myHalfEdges.EmplaceBack(myHalfEdges.Count(), aHalfEdge.myHeight + 1,
                            aHalfEdge.myIsReversed != aHalfEdge.myIsAlternating,
                            aHalfEdge.myIsReversed != aHalfEdge.myIsAlternating, -1, -1, -1, -1);
    TileHalfEdge& rightChild {myHalfEdges.GetLast()};
    aHalfEdge.myRightChild = rightChild.myIndex;
    rightChild.myParent = aHalfEdge.myIndex;

    int& vertexIdx {rightChild.myVertex};
    if (aHalfEdge.myOpposite != -1)
    {
        const TileHalfEdge& oppositeHEdge {myHalfEdges[aHalfEdge.myOpposite]};
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
        const TileHalfEdge& nextHEdge {myHalfEdges[aHalfEdge.myNext]};
        vertexIdx = myVertices.Count();
        if (aHalfEdge.myIsReversed)
        {
            CreateInflationVertex(myVertices[nextHEdge.myVertex], myVertices[aHalfEdge.myVertex]);
        }
        else
        {
            CreateInflationVertex(myVertices[aHalfEdge.myVertex], myVertices[nextHEdge.myVertex]);
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
