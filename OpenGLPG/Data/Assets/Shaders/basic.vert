#version 450 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform mat4 Model;
uniform mat4 WorldToClip;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position = WorldToClip * Model * gl_Position;
    vertexColor = vec4(0.8, 0.8, 0.0, 1.0);
}