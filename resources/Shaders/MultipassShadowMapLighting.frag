#version 330

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

struct Attenuation
{
    float constant;
    float linear;
    float exponential;
};

struct PointLight
{
    BaseLight base;
    Attenuation attenuation;
    vec3 position;
};

uniform PointLight uPointLight;
uniform sampler2D uSampler;
uniform samplerCube uShadowMap;
uniform vec3 uEyeWorldPos;
uniform float uMaterialSpecularIntensity;
uniform float uMaterialSpecularPower;
uniform int uUseSampler;
uniform int uUseNormalMap;


float calcShadowFactor(vec3 pLightDirection)
{
    float lSampledDistance = texture(uShadowMap, pLightDirection).r;

    float lDistance = length(pLightDirection);

    if (lDistance <= lSampledDistance + 0.001f)
        return 1.0f; /* In the light */
    else
        return 0.4f; /* In the shadow */
}

vec4 calcLightInternal(BaseLight pLight, vec3 pLightDirection, vec3 pNormal, float pShadowFactor)
{
    vec4 lAmbientColor = vec4(pLight.color, 1.0f) * pLight.ambientIntensity;
    float lDiffuseFactor = dot(pNormal, -pLightDirection);

    vec4 lDiffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 lSpecularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lDiffuseFactor > 0.0f)
    {
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

vec4 calcPointLight(PointLight pPointLight, vec3 pNormal)
{
    vec3 lLightDirection = worldPos0 - pPointLight.position;

    float lShadowFactor = calcShadowFactor(lLightDirection);

    float lDistance = length(lLightDirection);
    lLightDirection = normalize(lLightDirection);

    vec4 lColor = calcLightInternal(pPointLight.base, lLightDirection, pNormal, lShadowFactor);

    float lAttenuation = pPointLight.attenuation.constant + pPointLight.attenuation.linear * lDistance + pPointLight.attenuation.exponential * lDistance * lDistance;

    return lColor / lAttenuation;
}

void main(void)
{
    vec3 lNormal;

    if (uUseNormalMap == 1)
        lNormal = normalize(normal0);
    else
        lNormal = normalize(normal0);

    vec4 lTotalLight = calcPointLight(uPointLight, lNormal);

    vec4 lSampledColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    if(uUseSampler == 1)
        lSampledColor = texture(uSampler, texCoord0.xy);

    fragColor = lSampledColor * lTotalLight;
}
