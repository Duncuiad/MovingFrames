#pragma once

#include "Defines.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct Vertex
{
    using List = std::vector<Vertex>;
    using Index = unsigned int;
    using IndexList = std::vector<Index>;

    glm::vec3 myPosition;
    glm::vec3 myNormal;
    glm::vec3 myColor;
    glm::vec3 myExtraData;
    glm::vec2 myTexCoords;
};

class Mesh
{
public:
    using List = std::vector<Mesh>;

    Mesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices);

    void SetIsWireframe(bool anIsWireframe) { myIsWireframe = anIsWireframe; }
    void DeleteBuffers();
    void Draw() const;

protected:
    Mesh() = default;

    Vertex::List myVertices;
    Vertex::IndexList myIndices;

    bool myIsWireframe {true};

    GLUID myVAO {0};
    GLUID myVBO {0};
    GLUID myEBO {0};
};
