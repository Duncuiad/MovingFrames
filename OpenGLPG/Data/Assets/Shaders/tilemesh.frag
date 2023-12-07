#version 450 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 uvs;

float smoothSize = 3.;

void main()
{
    vec2 doubleUVs = mod(uvs * 2., 1.);
    vec2 gradX = vec2(dFdxFine(uvs.x), dFdyFine(uvs.x));
    vec2 gradY = vec2(dFdxFine(uvs.y), dFdyFine(uvs.y));

    float distX2 = min(doubleUVs.x, 1. - doubleUVs.x);
    float distY2 = min(doubleUVs.y, 1. - doubleUVs.y);
    float distD2 = 2. * abs(uvs.x - uvs.y);
    distX2 = distX2 * distX2 / dot(gradX, gradX);
    distY2 = distY2 * distY2 / dot(gradY, gradY);
    distD2 = distD2 * distD2 / dot(gradX - gradY, gradX - gradY);
    
    float dist2 = min(min(distX2, distY2), distD2);
    FragColor = vec4(vec3(smoothstep(0., smoothSize, sqrt(dist2))), 1.);
}