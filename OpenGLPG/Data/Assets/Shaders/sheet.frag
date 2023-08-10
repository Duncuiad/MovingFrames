#version 450 core

in GS_OUT {
    vec4 myColor;
    vec3 myNormal;
} fs_in;

out vec4 FragColor;

const vec3 LightDir = vec3(0.0, 1.0, 0.0);

void main()
{
    FragColor = fs_in.myColor;
    FragColor *= dot(fs_in.myNormal, LightDir);
    FragColor.w = 1.0;
}