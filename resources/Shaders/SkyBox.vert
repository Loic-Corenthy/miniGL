#version 330

layout (location = 0) in vec3 position;

uniform mat4 uWVP;

out vec3 texCoord0;

void main()
{
    vec4 WVP_Pos = uWVP * vec4(position, 1.0);
    gl_Position = WVP_Pos.xyww;
    texCoord0   = position;
}
