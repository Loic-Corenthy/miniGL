#version 410

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 uVP;
uniform vec3 uCameraPosition;
uniform float uBillboardSize;

out vec2 texCoord;

void main()
{
    vec3 lPos = gl_in[0].gl_Position.xyz;
    vec3 lToCamera = normalize(uCameraPosition - lPos);
    vec3 lUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 lRight = cross(lToCamera, lUp) * uBillboardSize;

    lPos -= (lRight * 0.5f);
    gl_Position = uVP * vec4(lPos, 1.0f);
    texCoord = vec2(0.0f, 0.0f);
    EmitVertex();

    lPos.y += uBillboardSize;
    gl_Position = uVP * vec4(lPos, 1.0f);
    texCoord = vec2(0.0f, 1.0f);
    EmitVertex();

    lPos.y -= uBillboardSize;
    lPos += lRight;
    gl_Position = uVP * vec4(lPos, 1.0f);
    texCoord = vec2(1.0f, 0.0f);
    EmitVertex();

    lPos.y += uBillboardSize;
    gl_Position = uVP * vec4(lPos, 1.0f);
    texCoord = vec2(1.0f, 1.0f);
    EmitVertex();

    EndPrimitive();
}
