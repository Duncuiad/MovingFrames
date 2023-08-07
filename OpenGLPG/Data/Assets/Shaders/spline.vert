#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aRight;
layout (location = 2) in vec3 anUp;
layout (location = 3) in vec3 aForward;
layout (location = 4) in vec3 aVelocity;
layout (location = 5) in vec3 anAngularVelocity;

out vec4 vertexColor;

uniform mat4 Model;
uniform mat4 WorldToClip;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position = WorldToClip * Model * gl_Position;
    vertexColor = vec4(0.8, 0.8, 0.0, 1.0);
}