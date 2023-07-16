#version 450 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    mat3 myNormalMatrix;
} gs_in[];

uniform mat4 Projection;

const float MAGNITUDE = 0.2;

void GenerateLine(int index)
{
    gl_Position = Projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = Projection * (gl_in[0].gl_Position + vec4(gs_in[0].myNormalMatrix[index], 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // X axis
    GenerateLine(1); // Y axis
    GenerateLine(2); // Z axis
}