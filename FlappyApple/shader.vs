#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 vMovement;
uniform mat4 hMovement;
uniform mat4 projection;
uniform mat4 aRotate;
uniform int obj;

out vec3 ourColor;
void main()
{
	if (obj == 0) {
		gl_Position =  vec4(aPos, 1.0);
	} else if (obj == 1) {
		gl_Position =  projection * hMovement * vec4(aPos, 1.0);
	} else {
		gl_Position =  projection * vMovement * aRotate * vec4(aPos, 1.0);
	}
	
	ourColor = aColor;
}