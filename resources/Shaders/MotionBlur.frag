#version 330

in vec2 textureCoords0;

uniform sampler2D uColorTexture;
uniform sampler2D uMotionTexture;

out vec4 fragColor;

void main()
{
	vec2 lMotionVector = texture(uMotionTexture, textureCoords0).xy * 0.01;
	vec4 lColor = vec4(0.0f);
	vec2 lTextureCoords = textureCoords0;

	lColor += texture(uColorTexture, lTextureCoords) * 0.4f;
	lTextureCoords -= lMotionVector;
	lColor += texture(uColorTexture, lTextureCoords) * 0.3f;
	lTextureCoords -= lMotionVector;
	lColor += texture(uColorTexture, lTextureCoords) * 0.2f;
	lTextureCoords -= lMotionVector;
	lColor += texture(uColorTexture, lTextureCoords) * 0.1f;

    fragColor = lColor;
}
