#version 330

const int maxPointLights = 4;
const int maxSpotLights = 4;

in vec4 lightSpacePos;
in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;
in vec3 tangent0;
flat in int instanceID;


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

struct SpotLight
{
    PointLight basepl;
    vec3 direction;
    float cutoff; /* directly the cosine of the angle!!! */
};

uniform PointLight uPointLight[maxPointLights];
uniform SpotLight uSpotLight[maxSpotLights];
uniform int uPointLightCount;
uniform int uSpotLightCount;
uniform DirectionalLight uDirectionalLight;
uniform sampler2D uSampler;
uniform int uUseSampler;
uniform sampler2D uShadowMap;
uniform int uUseShadowMap;
uniform sampler2D uNormalMap;
uniform int uUseNormalMap;
uniform vec3 uEyeWorldPos;
uniform float uMaterialSpecularIntensity;
uniform float uMaterialSpecularPower;
uniform vec4 uInstanceColor[4];

float calcShadowFactor(vec4 pLightSpacePos)
{
    vec3 lProjCoords = lightSpacePos.xyz / lightSpacePos.w;

    vec2 lUVCoords;
    lUVCoords.x = 0.5f * lProjCoords.x + 0.5f;
    lUVCoords.y = 0.5f * lProjCoords.y + 0.5f;
    float lZ = 0.5f * lProjCoords.z + 0.5f;

    float lDepth = texture(uShadowMap, lUVCoords).x;

    if (lDepth < (lZ + 0.00001))
        return 0.5f;
    else
        return 1.0f;
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

vec4 calcDirectionalLight(vec3 pNormal)
{
    return calcLightInternal(uDirectionalLight.base, uDirectionalLight.direction, pNormal, 1.0f);
}

vec4 calcPointLight(PointLight pPointLight, vec3 pNormal, vec4 pLightSpacePos)
{
    vec3 lLightDirection = worldPos0 - pPointLight.position;
    float lDistance = length(lLightDirection);
    lLightDirection = normalize(lLightDirection);

    float lShadowFactor = 1.0f;

    if(uUseShadowMap == 1)
        lShadowFactor = calcShadowFactor(pLightSpacePos);

    vec4 lColor = calcLightInternal(pPointLight.base, lLightDirection, pNormal, lShadowFactor);

    float lAttenuation = pPointLight.attenuation.constant + pPointLight.attenuation.linear * lDistance + pPointLight.attenuation.exponential * lDistance * lDistance;

    return lColor / lAttenuation;

}

vec4 calcSpotLight(SpotLight pSpotLight, vec3 pNormal, vec4 pLightSpacePos)
{
    vec3 lLightToPixel = normalize(worldPos0 - pSpotLight.basepl.position);
    float lSpotFactor = dot(lLightToPixel, pSpotLight.direction);

    vec4 lColor = vec4(0,0,0,0);

    if (lSpotFactor > pSpotLight.cutoff)
    {
        lColor = calcPointLight(pSpotLight.basepl, pNormal, pLightSpacePos);
        lColor = lColor * (1.0f - (1.0f - lSpotFactor) / (1.0f - pSpotLight.cutoff) );
    }

    return lColor;
}

vec3 calcBumpedNormal(void)
{
    vec3 lNormal = normalize(normal0);
    vec3 lTangent = normalize(tangent0);
    lTangent = normalize(lTangent - dot(lTangent, lNormal) * lNormal);
    vec3 lBitangent = cross(lTangent, lNormal);
    vec3 lBumpMapNormal = texture(uNormalMap, texCoord0).xyz;
    lBumpMapNormal = 2.0f * lBumpMapNormal - vec3(1.0f, 1.0f, 1.0f);

    mat3 TBN = mat3(lTangent, lBitangent, lNormal);
    vec3 lNewNormal = TBN * lBumpMapNormal;
    return normalize(lNewNormal);
}

void main(void)
{
    vec3 lNormal;

    if (uUseNormalMap == 1)
    {
        lNormal = calcBumpedNormal();
    }
    else
        lNormal = normalize(normal0);

    vec4 lTotalLight = calcDirectionalLight(lNormal);

    for(int i = 0; i < uPointLightCount; i++)
    {
        lTotalLight += calcPointLight(uPointLight[i], lNormal, lightSpacePos);
    }

    for(int i = 0; i < uSpotLightCount; i++)
    {
        lTotalLight += calcSpotLight(uSpotLight[i], lNormal, lightSpacePos);
    }

    vec4 lSampledColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    if(uUseSampler == 1)
        lSampledColor = texture(uSampler, texCoord0.xy);

    fragColor = lSampledColor * lTotalLight * uInstanceColor[instanceID % 4];
}

