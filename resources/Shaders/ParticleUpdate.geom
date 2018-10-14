#version 330

const float PARTICLE_TYPE_LAUNCHER = 0.0f;
const float PARTICLE_TYPE_SHELL = 1.0f;
const float PARTICLE_TYPE_SECONDARY_SHELL = 2.0f;

layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in vec3 position0[];
in float type0[];
in vec3 velocity0[];
in float age0[];

out vec3 position1;
out float type1;
out vec3 velocity1;
out float age1;

uniform float uDeltaTime;
uniform float uTime;
uniform sampler1D uRandomTexture;
uniform float uLauncherLifetime;
uniform float uShellLifetime;
uniform float uSecondaryShellLifetime;
uniform vec3 uInitialLauncherPosition;

vec3 getRandomDirection(float pTexCoord)
{
    vec3 lDir = texture(uRandomTexture, pTexCoord).xyz;
    lDir -= vec3(0.5f, 0.5f, 0.5f);
    return lDir;
}

void main()
{
    float lAge = age0[0] + uDeltaTime;

    if (type0[0] == PARTICLE_TYPE_LAUNCHER)
    {
        if (lAge >= uLauncherLifetime)
        {
            type1 = PARTICLE_TYPE_SHELL;
            position1 = position0[0];
            vec3 lDirection = getRandomDirection(uTime / 1000.0f);
            lDirection.y = max(lDirection.y, 0.5f);
            velocity1 = normalize(lDirection);
            age1 = 0.0f;
            EmitVertex();
            EndPrimitive();
            lAge = 0.0f;
        }

        type1 = PARTICLE_TYPE_LAUNCHER;
        position1 = uInitialLauncherPosition;
        velocity1 = velocity0[0];
        age1 = lAge;
        EmitVertex();
        EndPrimitive();
    }
    else
    {
        float lDeltaTimeInSec = uDeltaTime / 1000.0f;
        vec3 lDeltaP = lDeltaTimeInSec * velocity0[0];
        vec3 lDeltaV = vec3(0.0f, lDeltaTimeInSec * -9.81f * 0.001, 0.0f);

        if (type0[0] == PARTICLE_TYPE_SHELL)
        {
            if (lAge < uShellLifetime)
            {
                type1 = PARTICLE_TYPE_SHELL;
                position1 = position0[0] + lDeltaP;
                velocity1 = velocity0[0] + lDeltaV;
                age1 = lAge;
                EmitVertex();
                EndPrimitive();
            }
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    type1 = PARTICLE_TYPE_SECONDARY_SHELL;
                    position1 = position0[0];
                    vec3 lDirection = getRandomDirection((uTime + i) / 1000.0f);
                    velocity1 = normalize(lDirection) / 2.0f;
                    age1 = 0.0f;
                    EmitVertex();
                    EndPrimitive();
                }
            }
        }
        else
        {
            if (lAge < uSecondaryShellLifetime)
            {
                type1 = PARTICLE_TYPE_SECONDARY_SHELL;
                position1 = position0[0] + lDeltaP;
                velocity1 = velocity0[0] + (lDeltaV * 10.0f);
                age1 = lAge;
                EmitVertex();
                EndPrimitive();
            }
        }

    }
}
