#version 410

uniform sampler2D uColorMap;
uniform bool uActivateCutoffMax;
uniform bool uActivateCutoffMin;
uniform float uCutoffMax;
uniform float uCutoffMin;

in vec2 texCoord;

out vec4 fragColor;

void main()
{
    fragColor = texture(uColorMap, texCoord);

    if (uActivateCutoffMax && fragColor.r > uCutoffMax && fragColor.g > uCutoffMax && fragColor.b > uCutoffMax)
        discard;

    if (uActivateCutoffMin && fragColor.r < uCutoffMin && fragColor.g < uCutoffMin && fragColor.b < uCutoffMin)
        discard;
}
