#version 450 core
layout (lines) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
    mat3 myOrientation;
    vec3 myVelocity;
    vec3 myAngular;
} gs_in[];

out GS_OUT {
    vec4 myColor;
    vec3 myNormal;
} gs_out;

const vec4 baseColor = vec4(0.8, 0.8, 0.0, 1.0);

uniform mat4 Projection;
uniform mat4 View;

uniform float KeyScale;
uniform float TangentScale;

void main()
{
    gs_out.myColor = baseColor;
    vec4 pos0 = gl_in[0].gl_Position;
    vec4 pos1 = gl_in[1].gl_Position;
    vec4 z = normalize(pos1 - pos0);
    vec4 x = vec4(gs_in[0].myOrientation[0], 0.0);
    x = normalize(x - z * dot(x,z));
    gs_out.myNormal = vec3(inverse(View) * vec4(cross(vec3(z), vec3(x)),0.0));
    // @todo: pass Camera matrix as uniform in order to avoid inverting View matrix

    vec4 v0 = Projection * (pos0 + x * KeyScale);
    vec4 v1 = Projection * (pos1 + x * KeyScale);
    vec4 v2 = Projection * (pos1 - x * KeyScale);
    vec4 v3 = Projection * (pos0 - x * KeyScale);
    
    gl_Position = v0;
    EmitVertex();    
    gl_Position = v1;
    EmitVertex();    
    gl_Position = v3;
    EmitVertex();  
    gl_Position = v3;
    EmitVertex();    
    gl_Position = v1;
    EmitVertex();    
    gl_Position = v2;
    EmitVertex();
    EndPrimitive();
}