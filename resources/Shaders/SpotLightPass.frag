#version 330

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

struct SpotLight
{
    PointLight basepl;
    vec3 direction;
    float cutoff; /* directly the cosine of the angle!!! */
};

uniform sampler2D uPositionMap;
uniform sampler2D uColorMap;
uniform sampler2D uNormalMap;
uniform SpotLight uSpotLight;
uniform vec3 uEyeWorldPos;
uniform vec2 uScreenSize;
uniform float uMaterialSpecularIntensity;
uniform float uMaterialSpecularPower;


vec4 calcLightInternal(BaseLight pLight, vec3 pLightDirection, vec3 pWorldPosition, vec3 pNormal)
{
    vec4 lAmbientColor = vec4(pLight.color * pLight.ambientIntensity, 1.0f);
    float lDiffuseFactor = dot(pNormal, -pLightDirection);

    vec4 lDiffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 lSpecularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lDiffuseFactor > 0.0f)
    {
        lDiffuseColor = vec4(pLight.color * pLight.diffuseIntensity * lDiffuseFactor, 1.0f);

        vec3 lVertexToEye = normalize(uEyeWorldPos - pWorldPosition);
        vec3 lReflectedLight = reflect(pLightDirection, pNormal);
        lReflectedLight = normalize(lReflectedLight);
        float lSpecularFactor = dot(lVertexToEye, lReflectedLight);

        if (lSpecularFactor > 0.0f)
        {
            lSpecularFactor = pow(lSpecularFactor, uMaterialSpecularPower);
            lSpecularColor = vec4(pLight.color * uMaterialSpecularIntensity * lSpecularFactor, 1.0f);
        }
    }

    return (lAmbientColor + lDiffuseColor + lSpecularColor);
}

vec4 calcPointLight(vec3 pWorldPosition, vec3 pNormal)
{
    vec3 lLightDirection = pWorldPosition - uSpotLight.basepl.position;
    float lDistance = length(lLightDirection);
    lLightDirection = normalize(lLightDirection);

    vec4 lColor = calcLightInternal(uSpotLight.basepl.base, lLightDirection, pWorldPosition, pNormal);

    float lAttenuation = uSpotLight.basepl.attenuation.constant +
    uSpotLight.basepl.attenuation.linear * lDistance +
    uSpotLight.basepl.attenuation.exponential * lDistance * lDistance;

    lAttenuation = max(1.0f, lAttenuation);

    return lColor / lAttenuation;
}

vec4 calcSpotLight(vec3 pWorldPosition, vec3 pNormal)
{
    vec3 lLightToPixel = normalize(pWorldPosition - uSpotLight.basepl.position);
    float lSpotFactor = dot(lLightToPixel, uSpotLight.direction);

    vec4 lColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lSpotFactor > uSpotLight.cutoff)
    {
        lColor = calcPointLight(pWorldPosition, pNormal);
        lColor = lColor * (1.0f - (1.0f - lSpotFactor) / (1.0f - uSpotLight.cutoff) );
    }

    return lColor;
}

vec2 calcTextureCoordinates()
{
    return gl_FragCoord.xy / uScreenSize;
}

out vec4 fragColor;

void main()
{
    vec2 lTexCoord = calcTextureCoordinates();
    vec3 lWorldPos = texture(uPositionMap, lTexCoord).xyz;
    vec3 lColor = texture(uColorMap, lTexCoord).xyz;
    vec3 lNormal = texture(uNormalMap, lTexCoord).xyz;
    lNormal = normalize(lNormal);

    fragColor = vec4(lColor, 1.0) * calcSpotLight(lWorldPos, lNormal);
}
