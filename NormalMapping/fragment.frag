#version 330 core

out vec4 FragColor;

in VS_OUT 
{
	vec3 TangentFragPos;
	vec3 TangentLightPos;
	vec3 TangentCamPos;
	vec2 TexCoord;
} fs_in;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

void main()
{	
    vec3 Normal = vec3(texture(NormalMap, fs_in.TexCoord));
    Normal = normalize(Normal * 2.0 - vec3(1.0));

    vec3 Color  = texture(DiffuseMap, fs_in.TexCoord).rgb;

    vec3 Ambient = 0.1 * Color;

    vec3 LightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float Diff = max(dot(LightDir, Normal), 0.0);
    vec3 Diffuse = Diff * Color;

    vec3 ViewDir = normalize(fs_in.TangentCamPos - fs_in.TangentFragPos);
    vec3 ReflectLightDir = reflect(-LightDir, Normal);
    float Spec = pow(max(dot(ViewDir, ReflectLightDir), 0.0), 32.0); 
    vec3 Specular = vec3(0.2) * Spec;

    FragColor = vec4(Ambient + Diffuse + Specular, 1.0);
}