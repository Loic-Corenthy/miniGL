#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 uWVP;
uniform mat4 uWorld;

out vec3 worldPos0;

void main()
{
    vec4 lPos = vec4(position, 1.0f);
    gl_Position = uWVP * lPos;
    worldPos0 = (uWorld * lPos).xyz;
}
