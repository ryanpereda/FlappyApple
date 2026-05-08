#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 projection;
uniform int obj;

out vec3 ourColor;
void main()
{
	if (obj == 1) {
		gl_Position =  projection * view * vec4(aPos, 1.0);
	} else {
		gl_Position =  vec4(aPos, 1.0);
	}
	
	ourColor = aColor;
}