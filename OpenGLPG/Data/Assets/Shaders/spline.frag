#version 450 core

in GS_OUT {
    vec4 myColor;
} fs_in;

out vec4 FragColor;

void main()
{
    FragColor = fs_in.myColor;
}