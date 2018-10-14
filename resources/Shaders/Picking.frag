#version 330

uniform uint uDrawIndex;
uniform uint uObjectIndex;

out vec3 fragColor;

void main()
{
    /* gl_PrimitiveID can only be used in the GS and FS. If the GS is enabled and the FS wants to use gl_PrimitiveID, the GS must write gl_PrimitiveID into one of its output variables and the FS must declare a variable by the same name for input. In our case we have no GS so we can simply use gl_PrimitiveID. */
    fragColor = vec3(float(uObjectIndex), float(uDrawIndex), float(gl_PrimitiveID + 1));
}
