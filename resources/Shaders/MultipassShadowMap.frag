#version 330

in vec3 worldPos0;

uniform vec3 uLightWorldPos;

out float fragColor;

void main()
{
    vec3 lLightToVertex = worldPos0 - uLightWorldPos;
    fragColor = length(lLightToVertex);
}
