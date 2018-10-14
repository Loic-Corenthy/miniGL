#version 330

in vec2 texCoord0;
in vec2 viewRay0;

out vec4 fragColor;

uniform sampler2D uDepthMap;
uniform float uSampleRadius;
uniform mat4 uProjection;

const int maxKernelSize = 64;
uniform vec3 uKernel[maxKernelSize];

float calcViewZ(vec2 pCoords)
{
    float lDepth = texture(uDepthMap, pCoords).x;
    float lViewZ = uProjection[3][2] / (2.0f * lDepth - 1.0f - uProjection[2][2]);
    return lViewZ;
}

void main()
{
    float lViewZ = calcViewZ(texCoord0);

    float lViewX = viewRay0.x * lViewZ;
    float lViewY = viewRay0.y * lViewZ;

    vec3 lPos = vec3(lViewX, lViewY, lViewZ);

    float lAmbientOcclusionFactor = 0.0f;

    for (int i = 0; i < maxKernelSize; i++)
    {
        vec3 lSamplePos = lPos + uKernel[i];
        vec4 lOffset = vec4(lSamplePos, 1.0f);
        lOffset = uProjection * lOffset;
        lOffset.xy /= lOffset.w;
        lOffset.xy = lOffset.xy * 0.5f + vec2(0.5f);

        float lSampleDepth = calcViewZ(lOffset.xy);

        if (abs(lPos.z - lSampleDepth) < uSampleRadius)
        {
            lAmbientOcclusionFactor += step(lSampleDepth, lSamplePos.z);
        }
    }

    lAmbientOcclusionFactor = 1.0f - lAmbientOcclusionFactor / 64.0f;

    fragColor = vec4(pow(lAmbientOcclusionFactor, 2.0f));
}
