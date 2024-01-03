#version 450 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 uvs;
flat in float faceColor;

uniform int ShowGraphs;
uniform int ShowEdges;
uniform int ShowBlocks;

float smoothSize = 3.;

void DrawBlocks(vec2 UV, vec3 vertColor, inout vec4 fragColor)
{
    float vertexColorValue = max(max(vertColor.x, vertColor.y), vertColor.z);
    float centerColorValue = 0.;
    if (max(UV.x - 0.5, UV.y - 0.5) < 0.)
    {
        centerColorValue = vertColor.x + vertColor.y + vertColor.z;
    }
    else
    {
        vec2 doubleUVs = mod(UV * 2., 1.);
        centerColorValue = (doubleUVs.x - 0.5) * (doubleUVs.y - 0.5) < 0. && vertColor.y > 0. ? 1. : 0.;
    }
    vec4 blockColor = vertexColorValue >= 0.5 || centerColorValue >= 0.5 ? vec4(0.) : vec4(0.,0.,0.,1.);
    fragColor = fragColor * (1. - blockColor.a) + blockColor * blockColor.a;
}

void DrawMarchingSquares(in vec2 UV, in vec3 vertColor, inout vec4 fragColor)
{
    if (max(UV.x - 0.5, UV.y - 0.5) > 0.)
    {   
        UV.x = UV.x > 0.5 ? clamp(UV.x, 0.50001, 0.99999) : clamp(UV.x, 0.00001, 0.49999);
        UV.y = UV.y > 0.5 ? clamp(UV.y, 0.50001, 0.99999) : clamp(UV.y, 0.00001, 0.49999);
        vec2 doubleUVs = mod(UV * 2., 1.);
        vec3 barycentric = doubleUVs.x < doubleUVs.y ? vec3(1. - doubleUVs.y, doubleUVs.y - doubleUVs.x, doubleUVs.x)
                                                     : vec3(1. - doubleUVs.x, doubleUVs.x - doubleUVs.y, doubleUVs.y);
        float relevantColorValue = vertColor.y;
        relevantColorValue = (doubleUVs.x <= 0.5 && doubleUVs.y <= 0.5) ? vertColor.x : relevantColorValue;
        relevantColorValue = (doubleUVs.x >= 0.5 && doubleUVs.y >= 0.5) ? vertColor.z : relevantColorValue;
        float relevantBarycentric = barycentric.y;
        relevantBarycentric = (doubleUVs.x <= 0.5 && doubleUVs.y <= 0.5) ? barycentric.x : relevantBarycentric;
        relevantBarycentric = (doubleUVs.x >= 0.5 && doubleUVs.y >= 0.5) ? barycentric.z : relevantBarycentric;
        vec4 blockColor = vec4(vec3(relevantColorValue / relevantBarycentric), 1.);
        fragColor = fragColor * (1. - blockColor.a) + blockColor * blockColor.a;
    }
}

void DrawMarchingTriangles(in vec2 UV, in vec3 vertColor, inout vec4 fragColor)
{
    if (max(UV.x - 0.5, UV.y - 0.5) < 0.)
    {
        vec2 doubleUVs = clamp(UV, vec2(0.00001), vec2(0.49999)) * 2.;
        vec3 barycentric = doubleUVs.x < doubleUVs.y ? vec3(doubleUVs.x, doubleUVs.y - doubleUVs.x, 1. - doubleUVs.y)
                                                     : vec3(1. - doubleUVs.x, doubleUVs.x - doubleUVs.y, doubleUVs.y);
        float relevantColorValue = 0.;
        relevantColorValue = (barycentric.x >= barycentric.z && barycentric.x >= barycentric.y) ? vertColor.x : relevantColorValue;
        relevantColorValue = (barycentric.y >= barycentric.x && barycentric.y >= barycentric.z) ? vertColor.y : relevantColorValue;
        relevantColorValue = (barycentric.z >= barycentric.y && barycentric.z >= barycentric.x) ? vertColor.z : relevantColorValue;
        float relevantBarycentric = max(max(barycentric.x, barycentric.y), barycentric.z);
        vec4 blockColor = vec4(vec3(relevantColorValue / relevantBarycentric), 1.);
        fragColor = fragColor * (1. - blockColor.a) + blockColor * blockColor.a;
    }
}

void DrawFaces(in float aFaceColor, inout vec4 fragColor)
{
    vec4 blockColor = aFaceColor < 1.0 ? vec4(0.,0.,0.,1.) : vec4(0.);
    fragColor = fragColor * (1. - blockColor.a) + blockColor * blockColor.a;
}

void DrawEdges(in vec2 UV, in vec2 gradX, in vec2 gradY, inout vec4 fragColor)
{
    vec2 doubleUVs = mod(UV * 2., 1.);
    float lenGradX2 = dot(gradX, gradX);
    float lenGradY2 = dot(gradY, gradY);

    float distX2 = min(doubleUVs.x, 1. - doubleUVs.x);
    float distY2 = min(doubleUVs.y, 1. - doubleUVs.y);
    float distD2 = 2. * abs(UV.x - UV.y);
    distX2 = distX2 * distX2 / lenGradX2;
    distY2 = distY2 * distY2 / lenGradY2;
    distD2 = distD2 * distD2 / dot(gradX - gradY, gradX - gradY);
    
    float dist2 = min(min(distX2, distY2), distD2);
    float edgeValue = 1. - smoothstep(0., smoothSize, sqrt(dist2));
    fragColor = fragColor * (1. - edgeValue) + (vec4(1., 1., 1., 2. * fragColor.a) - fragColor) * edgeValue;
}

void DrawDualGraph(in vec2 UV, in vec2 gradX, in vec2 gradY, inout vec4 fragColor)
{
    vec2 doubleUVs = mod(UV * 2., 1.);
    float selectSquares = 2. * step(0., min(0.5 - UV.x, 0.5 - UV.y)) - 1.;
    float selectTriangles = 2. * step(0., max(UV.x - 0.5, UV.y - 0.5)) - 1.;
    float selectTypeA = 2. * step(0., UV.x - UV.y) - 1.;
    float selectDiagonalXY = 2. * step(0., 6. * (UV.y - UV.x) + 1.) - 1.;
    float selectDiagonalYX = 2. * step(0., 6. * (UV.x - UV.y) + 1.) - 1.;

    float distAS = max(abs(doubleUVs.x - 0.5), selectSquares);
    float distBS = max(abs(doubleUVs.y - 0.5), selectSquares);

    float distA21 = max(max(0.67 * abs(6. * UV.x - 3. * UV.y), selectDiagonalYX), selectTriangles);
    float distA12 = max(max(0.67 * abs(3. * UV.x - 6. * UV.y + 1.5), selectDiagonalYX), selectTriangles);
    float distA11 = max(max(2. * abs(UV.y + UV.x - 0.5), -selectDiagonalYX), selectTypeA);
    float distB12 = max(max(0.67 * abs(6. * UV.y - 3. * UV.x), selectDiagonalXY), selectTriangles);
    float distB21 = max(max(0.67 * abs(3. * UV.y - 6. * UV.x + 1.5), selectDiagonalXY), selectTriangles);
    float distB11 = max(max(2. * abs(UV.x + UV.y - 0.5), -selectDiagonalXY), -selectTypeA);
    float lenGradX2 = dot(gradX, gradX);
    float lenGradY2 = dot(gradY, gradY);
    float lenGrad12 = dot(gradX - gradY * 2., gradX - gradY * 2.);
    float lenGrad21 = dot(gradX * 2. - gradY, gradX * 2. - gradY);
    float lenGrad11 = dot(gradX + gradY, gradX + gradY);

    float distA2 = min(min(min(distAS * distAS / lenGradX2, distA12 * distA12 / lenGrad12), distA21 * distA21 / lenGrad21), distA11 * distA11 / lenGrad11);
    float distB2 = min(min(min(distBS * distBS / lenGradY2, distB12 * distB12 / lenGrad12), distB21 * distB21 / lenGrad21), distB11 * distB11 / lenGrad11);

    vec4 graphAColor = vec4(1., 0.7, 0., 1.) * (1. - smoothstep(0., smoothSize, sqrt(distA2)));
    vec4 graphBColor = vec4(0.5, 1., 0., 1.) * (1. - smoothstep(0., smoothSize, sqrt(distB2)));

    fragColor = fragColor * (1. - graphAColor.a) + graphAColor * graphAColor.a;
    fragColor = fragColor * (1. - graphBColor.a) + graphBColor * graphBColor.a;
}

void main()
{
    vec2 UV = uvs;
    vec4 fragColor = vec4(1.);
    vec2 gradX = vec2(dFdxFine(UV.x), dFdyFine(UV.x));
    vec2 gradY = vec2(dFdxFine(UV.y), dFdyFine(UV.y));

    if (ShowBlocks == 1)
    {
        //DrawBlocks(UV, vertexColor, fragColor);
        DrawMarchingSquares(UV, vertexColor, fragColor);
        DrawMarchingTriangles(UV, vertexColor, fragColor);
    }
    else if (ShowBlocks == 2)
    {
        DrawFaces(faceColor, fragColor);
    }
    if (ShowEdges > 0)
    {
        DrawEdges(UV, gradX, gradY, fragColor);
    }
    if (ShowGraphs > 0)
    {
        DrawDualGraph(UV, gradX, gradY, fragColor);
    }

    FragColor = vec4(fragColor.xyz, 1.);
}