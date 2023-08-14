#version 450 core
layout (lines) in;
layout (line_strip, max_vertices = 10) out;

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

uniform bool IsControlKey;
uniform float KeyScale;
uniform float TangentScale;

void GenerateTripodLine(int index)
{
    gl_Position = Projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = Projection * (gl_in[0].gl_Position + vec4(gs_in[0].myOrientation[index], 0.0) * (IsControlKey ? 0.2 : KeyScale));
    EmitVertex();
    EndPrimitive();
}

void GenerateVector(vec3 vector)
{
    gl_Position = Projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = Projection * (gl_in[0].gl_Position + vec4(vector, 0.0) * (IsControlKey ? 1.0 : TangentScale));
    EmitVertex();
    EndPrimitive();
}

void main()
{
    gs_out.myColor = IsControlKey ? vec4(1.0, 0.0, 0.0, 1.0) : baseColor;
    GenerateTripodLine(0); // X axis
    gs_out.myColor = IsControlKey ? vec4(0.0, 1.0, 0.0, 1.0) : baseColor;
    GenerateTripodLine(1); // Y axis
    gs_out.myColor = IsControlKey ? vec4(0.0, 0.0, 1.0, 1.0) : baseColor;
    GenerateTripodLine(2); // Z axis

    gs_out.myColor = vec4(0.0, 0.8, 0.8, 1.0);
    GenerateVector(gs_in[0].myVelocity);
    gs_out.myColor = vec4(0.8, 0.4, 0.0, 1.0);
    GenerateVector(gs_in[0].myAngular);
}