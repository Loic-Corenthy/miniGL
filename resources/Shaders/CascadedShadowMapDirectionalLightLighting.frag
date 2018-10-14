#version 330

const int numCascade = 3;

in vec4 lightSpacePos0[numCascade];
in float clipSpacePosZ0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

uniform DirectionalLight uDirectionalLight;
uniform sampler2D uSampler;
uniform int uUseSampler;
uniform sampler2D uShadowMap[numCascade];
uniform vec3 uEyeWorldPos;
uniform float uMaterialSpecularIntensity;
uniform float uMaterialSpecularPower;
uniform float uCascadeEndClipSpace[numCascade];

float calcShadowFactor(int pCascadeIndex, vec4 pLightSpacePos)
{
    vec3 lProjCoords = pLightSpacePos.xyz / pLightSpacePos.w;

    vec2 lUVCoords;
    lUVCoords.x = 0.5f * lProjCoords.x + 0.5f;
    lUVCoords.y = 0.5f * lProjCoords.y + 0.5f;
    float lZ = 0.5f * lProjCoords.z + 0.5f;

    float lDepth = texture(uShadowMap[pCascadeIndex], lUVCoords).x;

    if (lDepth < lZ + 0.0001f)
    {
        return 0.5f;
    }
    else
    {
        return 1.0f;
    }
}

vec4 calcLightInternal(BaseLight pLight, vec3 pLightDirection, vec3 pNormal, float pShadowFactor)
{
    vec4 lAmbientColor = vec4(pLight.color, 1.0f) * pLight.ambientIntensity;
    float lDiffuseFactor = dot(pNormal, -pLightDirection);

    vec4 lDiffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 lSpecularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lDiffuseFactor > 0.0f)
    {
        /*! \todo multiply pLight.color directly with diffuse intensity and diffuse factor instead of the whole vec4? */
        lDiffuseColor = vec4(pLight.color, 1.0f) * pLight.diffuseIntensity * lDiffuseFactor;

        vec3 lVertexToEye = normalize(uEyeWorldPos - worldPos0);
        vec3 lReflectedLight = reflect(pLightDirection,pNormal);
        lReflectedLight = normalize(lReflectedLight);
        float lSpecularFactor = dot(lVertexToEye, lReflectedLight);

        if (lSpecularFactor > 0.0f)
        {
            lSpecularFactor = pow(lSpecularFactor, uMaterialSpecularPower);
            lSpecularColor = vec4(pLight.color, 1.0f) * uMaterialSpecularIntensity * lSpecularFactor;
        }
    }

    return (lAmbientColor + (pShadowFactor * (lDiffuseColor + lSpecularColor)));
}

vec4 calcDirectionalLight(vec3 pNormal, float pShadowFactor)
{
    return calcLightInternal(uDirectionalLight.base, uDirectionalLight.direction, pNormal, pShadowFactor);
}

void main()
{
    vec3 lNormal = normalize(normal0);

    float lShadowFactor = 0.0f;
    vec4 lCascadeIndicator = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < numCascade; i++)
    {
        if (clipSpacePosZ0 <= uCascadeEndClipSpace[i])
        {
            lShadowFactor = calcShadowFactor(i, lightSpacePos0[i]);

            if (i == 0)
                lCascadeIndicator = vec4(1.0f, 0.0f, 0.0f, 0.0f);
            else if (i == 1)
                lCascadeIndicator = vec4(0.0f, 1.0f, 0.0f, 0.0f);
            else if (i == 2)
                lCascadeIndicator = vec4(0.0f, 0.0f, 1.0f, 0.0f);

            break;
        }
    }

    vec4 lTotalLight = calcDirectionalLight(lNormal, lShadowFactor);

    vec4 lSampledColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    if(uUseSampler == 1)
        lSampledColor = texture(uSampler, texCoord0.xy);

    fragColor = lSampledColor * lTotalLight + lCascadeIndicator * 0.2f;
}

