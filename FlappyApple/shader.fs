#version 330 core
out vec4 FragColor;

uniform int obj;
uniform sampler2D text;
uniform vec3 textColor;

in vec2 TexCoords;
in vec3 ourColor;
void main()
{
	if (obj != 3) {
		FragColor = vec4(ourColor, 1.0f);
	} else {
		vec4 sampled = vec4 (1.0, 1.0, 1.0, texture(text, TexCoords).r);
		FragColor = vec4(textColor, 1.0) * sampled;
	}
   
}