#version 450 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 uvs;

uniform int ShowGraphs;

float smoothSize = 3.;

void main()
{
    vec2 UV = uvs;
    vec2 doubleUVs = mod(UV * 2., 1.);
    vec2 gradX = vec2(dFdxFine(UV.x), dFdyFine(UV.x));
    vec2 gradY = vec2(dFdxFine(UV.y), dFdyFine(UV.y));
    float lenGradX2 = dot(gradX, gradX);
    float lenGradY2 = dot(gradY, gradY);

    float distX2 = min(doubleUVs.x, 1. - doubleUVs.x);
    float distY2 = min(doubleUVs.y, 1. - doubleUVs.y);
    float distD2 = 2. * abs(UV.x - UV.y);
    distX2 = distX2 * distX2 / lenGradX2;
    distY2 = distY2 * distY2 / lenGradY2;
    distD2 = distD2 * distD2 / dot(gradX - gradY, gradX - gradY);
    
    float dist2 = min(min(distX2, distY2), distD2);
    vec4 edgeColor = vec4(smoothstep(0., smoothSize, sqrt(dist2)));
    edgeColor.a = 1. - edgeColor.a;

    if (ShowGraphs > 0)
    {
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
        float lenGrad12 = dot(gradX - gradY * 2., gradX - gradY * 2.);
        float lenGrad21 = dot(gradX * 2. - gradY, gradX * 2. - gradY);
        float lenGrad11 = dot(gradX + gradY, gradX + gradY);

        float distA2 = min(min(min(distAS * distAS / lenGradX2, distA12 * distA12 / lenGrad12), distA21 * distA21 / lenGrad21), distA11 * distA11 / lenGrad11);
        float distB2 = min(min(min(distBS * distBS / lenGradY2, distB12 * distB12 / lenGrad12), distB21 * distB21 / lenGrad21), distB11 * distB11 / lenGrad11);

        vec4 graphAColor = vec4(1., 0.7, 0., 1.) * (1. - smoothstep(0., smoothSize, sqrt(distA2)));
        vec4 graphBColor = vec4(0.5, 1., 0., 1.) * (1. - smoothstep(0., smoothSize, sqrt(distB2)));

        edgeColor = graphAColor * (1. - edgeColor.a) + edgeColor * edgeColor.a;
        edgeColor = graphBColor * (1. - edgeColor.a) + edgeColor * edgeColor.a;
    }

    edgeColor = vec4(1.) * (1. - edgeColor.a) + edgeColor * edgeColor.a;
    FragColor = vec4(edgeColor.xyz, 1.);
}