#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec4 vertexColor;
out vec2 uvs;

uniform mat4 Model;
uniform mat4 WorldToClip;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position = WorldToClip * Model * gl_Position;
    vertexColor = vec4(aNormal, 1.);
    uvs = aUV;
}