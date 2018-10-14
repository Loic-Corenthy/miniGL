#version 330

in vec2 texCoord0;

out vec4 fragColor;

uniform sampler2D uColorMap;

void main()
{
    float lOffsets[4] = float[](-1.5f, -0.5f, 0.5f, 1.5f);

    vec3 lColor = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            vec2 texCoord;
            texCoord.x = texCoord0.x + lOffsets[j] / textureSize(uColorMap, 0).x;
            texCoord.y = texCoord0.y + lOffsets[i] / textureSize(uColorMap, 0).y;
            lColor += texture(uColorMap, texCoord).xyz;
        }
    }

    lColor /= 16.0f;

    fragColor = vec4(lColor, 1.0f);
}

