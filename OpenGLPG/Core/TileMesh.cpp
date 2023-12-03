#include "OpenGLPG_Base.h"

#include "TileMesh.h"

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

void TileMesh::SubdivideFace(int aFaceIdx) {}

void TileMesh::SubdivideHalfEdge(int aHalfEdgeIdx)
{
    TileHalfEdge& halfEdge {myHalfEdges[aHalfEdgeIdx]};

    myHalfEdges.EmplaceBack(myHalfEdges.Count(), halfEdge.myHeight + 1,
                            halfEdge.myIsReversed != halfEdge.myIsAlternating,
                            halfEdge.myIsReversed == halfEdge.myIsAlternating, halfEdge.myVertex, -1, -1, -1);
    TileHalfEdge& leftChild {myHalfEdges.GetLast()};
    halfEdge.myLeftChild = leftChild.myIndex;
    leftChild.myParent = halfEdge.myIndex;

    myHalfEdges.EmplaceBack(myHalfEdges.Count(), halfEdge.myHeight + 1,
                            halfEdge.myIsReversed != halfEdge.myIsAlternating,
                            halfEdge.myIsReversed != halfEdge.myIsAlternating, -1, -1, -1, -1);
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
        const Vec2& fromPosition {myVertices[halfEdge.myVertex].myPosition};
        const Vec2& toPosition {myVertices[nextHEdge.myVertex].myPosition};

        constexpr float sinOfPiOverTwelve {0.25881904510252f};
        const float sign {halfEdge.myIsReversed ? 1.f : -1.f};
        const Vec2 orthogonalComponent {toPosition.y - fromPosition.y, fromPosition.x - toPosition.x};
        const Vec2 newVertexPosition {(fromPosition + toPosition + sign * sinOfPiOverTwelve * orthogonalComponent) *
                                      0.5f};
        vertexIdx = myVertices.Count();
        myVertices.EmplaceBack(vertexIdx, halfEdge.myHeight + 1, newVertexPosition);
    }
}
