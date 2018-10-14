#version 330

layout (triangles_adjacency) in; /* six vertices per triangle in */
layout (triangle_strip, max_vertices = 18) out; /* 4 vertices per quad * 3 triangle vertices + 6 vertices for near/far caps */

in vec3 pos0[]; /* an array of 6 vertices (triangle with adjacency) */

uniform vec3 uLightPos;
uniform mat4 uWVP;

float epsilon = 0.0001f;

/* Emit a quad using a triangle strip */
void emitQuad(vec3 pStartVertex, vec3 pEndVertex)
{
    /* Vertex 1: starting vertex (just a tiny bit below the original edge) */
    vec3 lLightDir = normalize(pStartVertex - uLightPos);
    gl_Position = uWVP * vec4((pStartVertex + lLightDir * epsilon), 1.0f);
    EmitVertex();

    /* Vertex 2: starting vertex projected to infinity */
    gl_Position = uWVP * vec4(lLightDir, 0.0f);
    EmitVertex();

    /* Vertex 3: ending vertex (just a tiny bit below the original edge) */
    lLightDir = normalize(pEndVertex - uLightPos);
    gl_Position = uWVP * vec4((pEndVertex + lLightDir * epsilon), 1.0f);
    EmitVertex();

    /* Vertex 4: ending vertex projected to infinity */
    gl_Position = uWVP * vec4(lLightDir, 0.0f);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 lEdge1 = pos0[2] - pos0[0];
    vec3 lEdge2 = pos0[4] - pos0[0];
    vec3 lEdge3 = pos0[1] - pos0[0];
    vec3 lEdge4 = pos0[3] - pos0[2];
    vec3 lEdge5 = pos0[4] - pos0[2];
    vec3 lEdge6 = pos0[5] - pos0[0];

    vec3 lNormal = normalize(cross(lEdge1, lEdge2));
    vec3 lLightDir = normalize(uLightPos - pos0[0]);

    /* Handle only light facing triangles */
    if (dot(lNormal, lLightDir) > 0.0f)
    {
        lNormal = cross(lEdge3, lEdge1);

        if (dot(lNormal, lLightDir) <= 0.0f)
            emitQuad(pos0[0], pos0[2]);


        lNormal = cross(lEdge4, lEdge5);
        lLightDir = uLightPos - pos0[2];

        if (dot(lNormal, lLightDir) <= 0.0f)
            emitQuad(pos0[2], pos0[4]);

        lNormal = cross(lEdge2, lEdge6);
        lLightDir = uLightPos - pos0[4];

        if (dot(lNormal, lLightDir) <= 0.0f)
            emitQuad(pos0[4], pos0[0]);

        /* Render the front cap */
        lLightDir = normalize(pos0[0] - uLightPos);
        gl_Position = uWVP * vec4((pos0[0] + lLightDir * epsilon), 1.0f);
        EmitVertex();

        lLightDir = normalize(pos0[2] - uLightPos);
        gl_Position = uWVP * vec4((pos0[2] + lLightDir * epsilon), 1.0f);
        EmitVertex();

        lLightDir = normalize(pos0[4] - uLightPos);
        gl_Position = uWVP * vec4((pos0[4] + lLightDir * epsilon), 1.0f);
        EmitVertex();

        EndPrimitive();

        /* Render the back cap */
        /*! \warning The order in which the vertices are emitted was not fully tested to handle all cases! A wrong order can result in undesired shadows!!!! */
        lLightDir = normalize(pos0[0] - uLightPos);
        gl_Position = uWVP * vec4(lLightDir, 0.0f);
        EmitVertex();

        lLightDir = normalize(pos0[4] - uLightPos);
        gl_Position = uWVP * vec4(lLightDir, 0.0f);
        EmitVertex();

        lLightDir = normalize(pos0[2] - uLightPos);
        gl_Position = uWVP * vec4(lLightDir, 0.0f);
        EmitVertex();

        EndPrimitive();
    }
}
