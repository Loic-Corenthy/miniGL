#version 330


const int SHADER_TYPE_SSAO    = 0;
const int SHADER_TYPE_NO_SSAO = 1;
const int SHADER_TYPE_ONLY_AO = 2;

const int maxPointLights = 4;
const int maxSpotLights = 4;


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

uniform int uShaderType;
uniform PointLight uPointLight[maxPointLights];
uniform SpotLight uSpotLight[maxSpotLights];
uniform int uPointLightCount;
uniform int uSpotLightCount;
uniform DirectionalLight uDirectionalLight;
uniform sampler2D uSampler;
uniform int uUseSampler;
uniform sampler2D uAOMap;
uniform vec3 uEyeWorldPos;
uniform float uMaterialSpecularIntensity;
uniform float uMaterialSpecularPower;
uniform vec2 uScreenSize;

vec2 calcScreenTexCoords()
{
    return gl_FragCoord.xy / uScreenSize;
}

vec4 calcLightInternal(BaseLight pLight, vec3 pLightDirection, vec3 pNormal)
{
    vec4 lAmbientColor = vec4(pLight.color, 1.0f) * pLight.ambientIntensity;

    if (uShaderType == SHADER_TYPE_SSAO)
        lAmbientColor *= texture(uAOMap, calcScreenTexCoords()).r;

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

    return (lAmbientColor + lDiffuseColor + lSpecularColor);
}

vec4 calcDirectionalLight(vec3 pNormal)
{
    return calcLightInternal(uDirectionalLight.base, uDirectionalLight.direction, pNormal);
}

vec4 calcPointLight(PointLight pPointLight, vec3 pNormal)
{
    vec3 lLightDirection = worldPos0 - pPointLight.position;
    float lDistance = length(lLightDirection);
    lLightDirection = normalize(lLightDirection);

    vec4 lColor = calcLightInternal(pPointLight.base, lLightDirection, pNormal);

    float lAttenuation = pPointLight.attenuation.constant + pPointLight.attenuation.linear * lDistance + pPointLight.attenuation.exponential * lDistance * lDistance;

    return lColor / lAttenuation;
}

vec4 calcSpotLight(SpotLight pSpotLight, vec3 pNormal)
{
    vec3 lLightToPixel = normalize(worldPos0 - pSpotLight.basepl.position);
    float lSpotFactor = dot(lLightToPixel, pSpotLight.direction);

    vec4 lColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lSpotFactor > pSpotLight.cutoff)
    {
        lColor = calcPointLight(pSpotLight.basepl, pNormal);
        lColor = lColor * (1.0f - (1.0f - lSpotFactor) / (1.0f - pSpotLight.cutoff) );
    }

    return lColor;
}


void main()
{
    vec3 lNormal = normalize(normal0);

    vec4 lTotalLight = calcDirectionalLight(lNormal);

    for(int i = 0; i < uPointLightCount; i++)
    {
        lTotalLight += calcPointLight(uPointLight[i], lNormal);
    }

    for(int i = 0; i < uSpotLightCount; i++)
    {
        lTotalLight += calcSpotLight(uSpotLight[i], lNormal);
    }

    vec4 lSampledColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    if (uUseSampler == 1)
        lSampledColor = texture(uSampler, texCoord0.xy);

    /* First case is to debug, just show the value in the ambient occlusion map texture */
    if (uShaderType == SHADER_TYPE_ONLY_AO)
        fragColor = vec4(texture(uAOMap, calcScreenTexCoords()).x);
    else
        fragColor = lSampledColor * lTotalLight;
}














