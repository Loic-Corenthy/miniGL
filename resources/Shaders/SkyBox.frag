#version 330

in vec3 texCoord0;

out vec4 fragColor;

uniform samplerCube uCubemapTexture;

void main()
{
    fragColor = texture(uCubemapTexture, texCoord0);
}
