#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

const int numCascade = 3;

uniform mat4 uWVP;
uniform mat4 uLightWVP[numCascade];
uniform mat4 uWorld;

out vec4 lightSpacePos0[numCascade];
out float clipSpacePosZ0;
out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
    vec4 lPosition = vec4(position, 1.0);
    gl_Position = uWVP * lPosition;

    for (int i = 0; i < numCascade; i++)
    {
        lightSpacePos0[i] = uLightWVP[i] * lPosition;
    }

    clipSpacePosZ0 = gl_Position.z;
    texCoord0 = textureCoords;
    normal0 = (uWorld * vec4(normal, 0.0)).xyz;
    worldPos0 = (uWorld * lPosition).xyz;
}

