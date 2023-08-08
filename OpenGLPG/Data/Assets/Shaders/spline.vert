#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aRight;
layout (location = 2) in vec3 anUp;
layout (location = 3) in vec3 aForward;
layout (location = 4) in vec3 aVelocity;
layout (location = 5) in vec3 anAngularVelocity;

out VS_OUT {
    mat3 myOrientation;
    vec3 myVelocity;
    vec3 myAngular;
} vs_out;

uniform mat4 ModelView;
uniform mat3 ModelViewIT;

void main()
{
    mat3 orientation = mat3(aRight, anUp, aForward);
    vs_out.myOrientation = ModelViewIT * orientation;
    vs_out.myVelocity = ModelViewIT * aVelocity;
    vs_out.myAngular = ModelViewIT * anAngularVelocity;
    gl_Position = vec4(aPos, 1.0);
    gl_Position = ModelView * gl_Position;
}