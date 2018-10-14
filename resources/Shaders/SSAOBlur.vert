#version 330

layout (location = 0) in vec3 position;

out vec2 texCoord0;

void main()
{
    gl_Position = vec4(position, 1.0f);
    texCoord0 = (position.xy + vec2(1.0f)) / 2.0f;
}

