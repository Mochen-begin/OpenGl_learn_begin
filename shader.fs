#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTexCoord;
#ifdef	USE_TEXTURE
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
#endif
void main()
{
	#ifdef USE_TEXTURE
	FragColor = mix(texture(texture1,ourTexCoord),texture(texture2,ourTexCoord),mixValue);
	#else
	FragColor = vec4(ourColor,1.0);
	#endif
}