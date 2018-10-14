#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in float type;
layout (location = 2) in vec3 velocity;
layout (location = 3) in float age;

out vec3 position0;
out float type0;
out vec3 velocity0;
out float age0;

void main()
{
    position0 = position;
    type0 = type;
    velocity0 = velocity;
    age0 = age;
   /* gl_Position = vec4(position, 1.0f); */
}
