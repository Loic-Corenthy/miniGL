

/*******************************/
/* Interface between VS and FS */
/*******************************/

interface VSOutput
{
    vec3 worldPosition;
    vec2 textureCoords;
    vec3 normal;
};

struct VSOutput1
{
    vec3 worldPosition;
    vec2 textureCoords;
    vec3 normal;
};

/*****************/
/* Vertex shader */
/*****************/

uniform mat4 uWVP;
uniform mat4 uWorld;

shader VSmain(in vec3 pPosition, in vec2 pTexCoords, in vec3 pNormal, out VSOutput pVSOutput)
{
    gl_Position = uWVP * vec4(pPosition, 1.0f);
    pVSOutput.worldPosition = (uWorld * vec4(pPosition, 1.0f)).xyz;
    pVSOutput.textureCoords = pTexCoords;
    pVSOutput.normal = (uWorld * vec4(pNormal, 0.0f)).xyz;
}

/*******************/
/* Fragment shader */
/*******************/

const int maxPointLights = 4;
const int maxSpotLights = 4;

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
uniform vec3 uEyeWorldPos;
uniform float uMaterialSpecularIntensity;
uniform float uMaterialSpecularPower;
uniform vec4 uColor;

vec4 calcLightInternal(BaseLight pLight, vec3 pLightDirection, VSOutput1 pFSInput)
{
    vec4 lAmbientColor = vec4(pLight.color, 1.0f) * pLight.ambientIntensity;
    float lDiffuseFactor = dot(pFSInput.normal, -pLightDirection);

    vec4 lDiffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 lSpecularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lDiffuseFactor > 0.0f)
    {
        lDiffuseColor = vec4(pLight.color, 1.0f) * pLight.diffuseIntensity * lDiffuseFactor;

        vec3 lVertexToEye = normalize(uEyeWorldPos - pFSInput.worldPosition);
        vec3 lReflectedLight = reflect(pLightDirection, pFSInput.normal);
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

vec4 calcDirectionalLight(VSOutput1 pFSInput)
{
    return calcLightInternal(uDirectionalLight.base, uDirectionalLight.direction, pFSInput);
}

vec4 calcPointLight(PointLight pPointLight, VSOutput1 pFSInput)
{
    vec3 lLightDirection = pFSInput.worldPosition - pPointLight.position;
    float lDistance = length(lLightDirection);
    lLightDirection = normalize(lLightDirection);

    vec4 lColor = calcLightInternal(pPointLight.base, lLightDirection, pFSInput);

    float lAttenuation = pPointLight.attenuation.constant + pPointLight.attenuation.linear * lDistance + pPointLight.attenuation.exponential * lDistance * lDistance;

    return lColor / lAttenuation;
}

vec4 calcSpotLight(SpotLight pSpotLight, VSOutput1 pFSInput)
{
    vec3 lLightToPixel = normalize(pFSInput.worldPosition - pSpotLight.basepl.position);
    float lSpotFactor = dot(lLightToPixel, pSpotLight.direction);

    vec4 lColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lSpotFactor > pSpotLight.cutoff)
    {
        lColor = calcPointLight(pSpotLight.basepl, pFSInput);
        lColor = lColor * (1.0f - (1.0f - lSpotFactor) / (1.0f - pSpotLight.cutoff) );
    }

    return lColor;
}

shader FSmain(in VSOutput pFSInput, out vec4 pFragColor)
{
    VSOutput1 lFSInput;
    lFSInput.worldPosition = pFSInput.worldPosition;
    lFSInput.textureCoords = pFSInput.textureCoords;
    lFSInput.normal = normalize(pFSInput.normal);

    vec4 lTotalLight = calcDirectionalLight(lFSInput);

    for(int i = 0; i < uPointLightCount; i++)
    {
        lTotalLight += calcPointLight(uPointLight[i], lFSInput);
    }

    for(int i = 0; i < uSpotLightCount; i++)
    {
        lTotalLight += calcSpotLight(uSpotLight[i], lFSInput);
    }

    if(uUseSampler == 1)
        lTotalLight *= texture(uSampler, lFSInput.textureCoords.xy);

    pFragColor = uColor * lTotalLight;
}

/***********/
/* Program */
/***********/

program GLFXLighting
{
    vs(330) = VSmain();
    fs(330) = FSmain();
};
