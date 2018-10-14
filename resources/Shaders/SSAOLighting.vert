#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

uniform mat4 uWVP;
uniform mat4 uWorld;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
    gl_Position = uWVP * vec4(position, 1.0f);
    texCoord0 = textureCoords;
    normal0 = (uWorld * vec4(normal, 0.0f)).xyz;
    worldPos0 = (uWorld * vec4(position, 1.0f)).xyz;
}
