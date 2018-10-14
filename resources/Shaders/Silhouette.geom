#version 330

layout (triangles_adjacency) in; /* six vertices per triangle in */
layout (line_strip, max_vertices = 6) out;

in vec3 worldPos0[]; /* an array of 6 vertices (triangle with adjacency) */

void emitLine(int pStartIndex, int pEndIndex)
{
    gl_Position = gl_in[pStartIndex].gl_Position;
    EmitVertex();

    gl_Position = gl_in[pEndIndex].gl_Position;
    EmitVertex();

    EndPrimitive();
}

uniform vec3 uLightPos;

void main()
{
    vec3 lEdge1 = worldPos0[2] - worldPos0[0];
    vec3 lEdge2 = worldPos0[4] - worldPos0[0];
    vec3 lEdge3 = worldPos0[1] - worldPos0[0];
    vec3 lEdge4 = worldPos0[3] - worldPos0[2];
    vec3 lEdge5 = worldPos0[4] - worldPos0[2];
    vec3 lEdge6 = worldPos0[5] - worldPos0[0];

	vec3 lNormal = cross(lEdge1, lEdge2);

	vec3 lLightDir = uLightPos - worldPos0[0];

	if (dot(lNormal, lLightDir) > 0.00001)
	{
		lNormal = cross(lEdge3, lEdge1);

		if (dot(lNormal, lLightDir) <= 0.0f)
			emitLine(0, 2);

		lNormal = cross(lEdge4, lEdge5);
		lLightDir = uLightPos - worldPos0[2];

		if (dot(lNormal, lLightDir) <= 0.0f)
			emitLine(2, 4);

		lNormal = cross(lEdge2, lEdge6);
		lLightDir = uLightPos - worldPos0[4];

		if (dot(lNormal, lLightDir) <= 0.0f)
			emitLine(4, 0);
	}
}

