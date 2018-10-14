#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

layout (location = 0) out vec3 worldPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalOut;

uniform sampler2D uColorMap;

void main()
{
    worldPosOut = worldPos0;
    diffuseOut = texture(uColorMap, texCoord0).xyz;
    normalOut = normalize(normal0);
}
