#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 Projection = mat4(1.0);
uniform mat4 View = mat4(1.0);

void main()
{
	TexCoord = aPos;
	vec4 Position = Projection * View * vec4(aPos, 1.0);
	gl_Position = Position.xyww;
}