#version 330 core

out vec4 FragColor;

in vec3 Normal;

struct SHCoeffs 
{ 
	vec3 L00, L1m1, L10, L11, L2m2, L2m1, L20, L21, L22;
};

SHCoeffs Beach = SHCoeffs(
    vec3( 0.6841148,  0.6929004,  0.7069543),
    vec3( 0.3173355,  0.3694407,  0.4406839),
    vec3(-0.1747193, -0.1737154, -0.1657420),
    vec3(-0.4496467, -0.4155184, -0.3416573),
    vec3(-0.1690202, -0.1703022, -0.1525870),
    vec3(-0.0837808, -0.0940454, -0.1027518),
    vec3(-0.0319670, -0.0214051, -0.0147691),
    vec3( 0.1641816,  0.1377558,  0.1010403),
    vec3( 0.3697189,  0.3097930,  0.2029923)
);

vec3 Lookup_Light(vec3 Normal, SHCoeffs L)
{
    float X = Normal.x;
    float Y = Normal.y; 
    float Z = Normal.z; 

    const float C1 = 0.429043;
    const float C2 = 0.511664;
    const float C3 = 0.743125;
    const float C4 = 0.886227;
    const float C5 = 0.247708;

	vec3 Result = C1 * L.L22 * (X * X - Y * Y) +
        		  C3 * L.L20 * Z * Z +
				  C4 * L.L00 -
				  C5 * L.L20 +
				  2.0 * C1 * L.L2m2 * X * Y +
				  2.0 * C1 * L.L21  * X * Z +
				  2.0 * C1 * L.L2m1 * Y * Z +
				  2.0 * C2 * L.L11  * X +
				  2.0 * C2 * L.L1m1 * Y +
				  2.0 * C2 * L.L10  * Z;
    
    return (Result);
}

vec3 GammaCorrection(vec3 Color)
{
    vec3 Result = pow(Color, vec3(1.0/2.0));
    return(Result);
}

void main()
{	
	vec3 Result = Lookup_Light(Normal, Beach) * 0.5;

    Result = GammaCorrection(Result);
	FragColor = vec4(Result, 1.0);
}