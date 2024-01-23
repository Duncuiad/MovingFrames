#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 anExtraData;
layout (location = 4) in vec2 aUV;

out vec3 redChannel;
out vec3 greenChannel;
out vec3 blueChannel;
out vec2 uvs;

uniform mat4 Model;
uniform mat4 WorldToClip;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position = WorldToClip * Model * gl_Position;
    redChannel = aNormal;
    greenChannel = aColor;
    blueChannel = anExtraData;
    uvs = aUV;
}