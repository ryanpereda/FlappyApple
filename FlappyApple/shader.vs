#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec4 vertex;


uniform mat4 vMovement;
uniform mat4 hMovement;
uniform mat4 projection;
uniform mat4 aRotate;
uniform int obj;

out vec2 TexCoords;
out vec3 ourColor;
void main()
{
	if (obj == 3) {
		gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
		TexCoords = vertex.zw;
	} else {
		if (obj == 0) {
			gl_Position =  vec4(aPos, 1.0);
		} else if (obj == 1) {
			gl_Position =  projection * hMovement * vec4(aPos, 1.0);
		} else if (obj == 2) {
			gl_Position =  projection * vMovement * aRotate * vec4(aPos, 1.0);
		} 

		ourColor = aColor;
	}
}