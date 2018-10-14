#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 12) in ivec4 boneID;
layout (location = 13) in vec4 boneWeight;
/*layout (location = 4) in ivec4 boneID;
 layout (location = 5) in vec4 boneWeight;*/ /*This configuration is used with MeshAOS because there is no instance rendering implemented yet */

const int MAX_BONES = 100;

uniform mat4 uWVP;
uniform mat4 uLightWVP;
uniform mat4 uWorld;
uniform mat4 uBone[MAX_BONES];
uniform mat4 uPreviousBone[MAX_BONES];

out vec4 lightSpacePos;
out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;
out vec3 tangent0;
out vec4 clipSpacePos0;
out vec4 clipSpacePreviousPos0;

void main()
{
    mat4 lBoneTransform = uBone[boneID[0]] * boneWeight[0];
    lBoneTransform 	   += uBone[boneID[1]] * boneWeight[1];
    lBoneTransform 	   += uBone[boneID[2]] * boneWeight[2];
    lBoneTransform 	   += uBone[boneID[3]] * boneWeight[3];

    vec4 lPos = lBoneTransform * vec4(position, 1.0);
    vec4 lClipSpacePos = uWVP * lPos;
    gl_Position = lClipSpacePos;
    lightSpacePos = uLightWVP * lPos;

    texCoord0 = textureCoords;

    vec4 lNormal = lBoneTransform * vec4(normal, 0.0);
    normal0 = (uWorld * lNormal).xyz;

    vec4 lTangent = lBoneTransform * vec4(tangent, 0.0);
    tangent0 = (uWorld * lTangent).xyz;

    worldPos0 = (uWorld * lPos).xyz;

    mat4 lPreviousBoneTransform = uPreviousBone[boneID[0]] * boneWeight[0];
    lPreviousBoneTransform 	   += uPreviousBone[boneID[1]] * boneWeight[1];
    lPreviousBoneTransform 	   += uPreviousBone[boneID[2]] * boneWeight[2];
    lPreviousBoneTransform 	   += uPreviousBone[boneID[3]] * boneWeight[3];

    clipSpacePos0 = lClipSpacePos;
    vec4 lPreviousPos = lPreviousBoneTransform * vec4(position, 1.0f);
    clipSpacePreviousPos0 = uWVP * lPreviousPos;
}
