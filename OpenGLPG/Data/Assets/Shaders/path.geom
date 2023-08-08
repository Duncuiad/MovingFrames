#version 450 core
layout (lines) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
    mat3 myOrientation;
    vec3 myVelocity;
    vec3 myAngular;
} gs_in[];

out GS_OUT {
    vec4 myColor;
} gs_out;

const vec4 baseColor = vec4(0.8, 0.8, 0.0, 1.0);

uniform mat4 Projection;

uniform float KeyScale;
uniform float TangentScale;

void main()
{
    gs_out.myColor = baseColor;
    gl_Position = Projection * (gl_in[1].gl_Position - vec4(gs_in[1].myOrientation[0], 0.0) * KeyScale);
    EmitVertex();
    gl_Position = Projection * (gl_in[0].gl_Position - vec4(gs_in[0].myOrientation[0], 0.0) * KeyScale);
    EmitVertex();
    gl_Position = Projection * (gl_in[0].gl_Position + vec4(gs_in[0].myOrientation[0], 0.0) * KeyScale);
    EmitVertex();
    gl_Position = Projection * (gl_in[1].gl_Position + vec4(gs_in[1].myOrientation[0], 0.0) * KeyScale);
    EmitVertex();
    EndPrimitive();
}