#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 ModelView;

out VS_OUT {
    vec3 myNormal;
} vs_out;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(ModelView)));
    vs_out.myNormal = vec3(vec4(normalMatrix * aNormal, 0.0));
    gl_Position = vec4(aPos, 1.0);
    gl_Position = ModelView * gl_Position;
}