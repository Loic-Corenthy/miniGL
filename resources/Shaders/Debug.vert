#version 410

layout (location = 0) in vec3 position;

uniform mat4 uWVP;

void main()
{
	gl_Position = uWVP * vec4(position, 1.0f);
}
