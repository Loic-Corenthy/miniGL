#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 uWVP;

void main()
{
    gl_Position = uWVP * vec4(position, 1.0);
}
