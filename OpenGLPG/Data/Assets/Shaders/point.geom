#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 myNormal;
} gs_in[];

uniform mat4 Projection;

const float MAGNITUDE = 0.2;

void GenerateLine()
{
    gl_Position = Projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = Projection * (gl_in[0].gl_Position + vec4(gs_in[0].myNormal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine();
}