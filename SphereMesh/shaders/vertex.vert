#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);
uniform mat4 Model = mat4(1.0);

out vec3 PosWorld;

void main()
{
	vec4 Temp = Model * vec4(aPosition, 1.0);
	PosWorld = vec3(Temp.xyz);
	gl_Position = Projection *  View *  Model * vec4(aPosition,  1.0);
}