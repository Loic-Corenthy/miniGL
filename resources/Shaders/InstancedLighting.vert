#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in mat4 wvp;
layout (location = 8) in mat4 world;

uniform mat4 uLightWVP;

out vec4 lightSpacePos;
out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;
out vec3 tangent0;
flat out int instanceID;

void main()
{
    gl_Position = wvp * vec4(position, 1.0);
    lightSpacePos = uLightWVP * vec4(position, 1.0);
    texCoord0 = textureCoords;
    normal0 = (world * vec4(normal, 0.0)).xyz;
    worldPos0 = (world * vec4(position, 1.0)).xyz;
    tangent0 = (world * vec4(tangent, 0.0)).xyz;
    instanceID = gl_InstanceID;
}
