#version 330 core

out vec4 FragColor;

in vec3 FragmentPosWorld;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 CamPosWorld;
uniform samplerCube Skybox;

vec3 GammaCorrection(vec3 Color)
{
    vec3 Result = pow(Color, vec3(1.0/2.0));
    return(Result);
}

void main()
{	
    vec3 ViewVector = normalize(FragmentPosWorld - CamPosWorld);
    vec3 Reflection = reflect(ViewVector, Normal);
	FragColor = texture(Skybox, Reflection);
}