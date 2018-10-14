#version 330

/* This fragment shader is actually not used! The shadow map uses only the vertex shader. The fragment shader is used in the lighting rendering */

in vec2 texCoordOut;
uniform sampler2D uShadowMap;

out vec4 fragColor;

void main()
{
    float lDepth = texture(uShadowMap, texCoordOut).x;
    /*lDepth = 1.0 - (1.0 - lDepth) * 25.0;*/
    fragColor = vec4(lDepth);
}
