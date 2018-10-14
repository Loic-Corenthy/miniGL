#version 410 core

layout (location = 0) in vec3 position_vs_in;
layout (location = 1) in vec2 textureCoord_vs_in;
layout (location = 2) in vec3 normal_vs_in;

uniform mat4 uWorld;

out vec3 worldPosition_cs_in;
out vec2 textureCoord_cs_in;
out vec3 normal_cs_in;

/*We are no longer transforming the local space coordinates to clip space (by multiplying by the world-view-projection matrix). The reason is that there is simply no point in that. We expect to generate a lot of new vertices that will need that transformation anyway. Therefore, this action is postponed until we get to the TES.*/
void main()
{
    worldPosition_cs_in = (uWorld * vec4(position_vs_in, 1.0f)).xyz;
    textureCoord_cs_in = textureCoord_vs_in;
    normal_cs_in = (uWorld * vec4(normal_vs_in, 0.0f)).xyz;
}
