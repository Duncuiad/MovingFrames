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

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    using List = std::vector<Mesh>;

    Mesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices);

    void DeleteBuffers();
    void Draw() const;

private:
    Vertex::List myVertices;
    Vertex::IndexList myIndices;

    bool myIsWireframe {true};

    GLUID myVAO {0};
    GLUID myVBO {0};
    GLUID myEBO {0};
};
