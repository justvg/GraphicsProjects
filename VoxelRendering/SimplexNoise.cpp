namespace core
{

	vec3 Gradients[] = {
		{1.0,1.0,0.0},{-1.0,1.0,0.0},{1.0,-1.0,0.0},{-1.0,-1.0,0.0},
		{1.0,0.0,1.0},{-1.0,0.0,1.0},{1.0,0.0,-1.0},{-1.0,0.0,-1.0},
		{0.0,1.0,1.0},{0.0,-1.0,1.0},{0.0,1.0,-1.0},{0.0,-1.0,-1.0}
	};

	int Permutation[512] = {
			151,160,137,91,90,15, 131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
			8,99,37,240,21,10,23, 190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
			35,11,32,57,177,33, 88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180, 151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 
	};

	float Dot(vec3 Grad, float X, float Y, float Z)
	{
		float Result = Grad.x * X + Grad.y * Y + Grad.z * Z;
		return(Result);
	}

	int FastFloor(float X)
	{
		int Result = X >= 0 ? (int)X : (int)X-1;
		return(Result);
	}

	float Noise(float XIn, float YIn, float ZIn)
	{
		float N0, N1, N2, N3; 

		// Skew the input space to determine which simplex cell we're in
		float F3 = 1.0f/3.0f;
		float S = (XIn + YIn + ZIn)*F3;
		int I = FastFloor(XIn + S);
		int J = FastFloor(YIn + S);
		int K = FastFloor(ZIn + S);

		// Unskew the cell origin back to (x,y,z) space
		float G3 = 1.0f/6.0f;
		float t = (I + J + K)*G3;
		float X0 = I - t;
		float Y0 = J - t;
		float Z0 = K - t;
		float x0 = XIn - X0;
		float y0 = YIn - Y0;
		float z0 = ZIn - Z0;

		// Determine which simplex we are in.
		int I1, J1, K1; // Offsets for second corner of simplex in (i,j,k) coords
		int I2, J2, K2; // Offsets for third corner of simplex in (i,j,k) coords
		if(x0 >= y0)
		{
			if(y0 >= z0)
			{
				I1 = 1; J1 = 0; K1 = 0; I2 = 1; J2 = 1; K2 = 0;
			}
			else if (x0 >= z0)
			{
				I1 = 1; J1 = 0; K1 = 0; I2 = 1; J2 = 0; K2 = 1; 
			}
			else 
			{
				I1 = 0; J1 = 0; K1 = 1; I2 = 1; J2 = 0; K2 = 1;
			}
		}
		else 
		{
			if(y0 < z0)
			{
				I1 = 0; J1 = 0; K1 = 1; I2 = 0; J2 = 1; K2 = 1;
			}
			else if(x0 < z0)
			{
				I1 = 0; J1 = 1; K1 = 0; I2 = 0; J2 = 1; K2 = 1;
			}
			else
			{
				I1 = 0; J1 = 1; K1 = 0; I2 = 1; J2 = 1; K2 = 0;
			}
		}

		float x1 = x0 - I1 + G3;
		float y1 = y0 - J1 + G3;
		float z1 = z0 - K1 + G3;
		float x2 = x0 - I2 + 2.0f*G3;
		float y2 = y0 - J2 + 2.0f*G3;
		float z2 = z0 - K2 + 2.0f*G3;
		float x3 = x0 - 1.0f + 3.0f*G3;
		float y3 = y0 - 1.0f + 3.0f*G3;
		float z3 = z0 - 1.0f + 3.0f*G3;

		int II = I & 255;
		int JJ = J & 255;
		int KK = K & 255;
		int GI0 = Permutation[II+Permutation[JJ+Permutation[KK]]] % 12;
		int GI1 = Permutation[II+I1+Permutation[JJ+J1+Permutation[KK+K1]]] % 12;
		int GI2 = Permutation[II+I2+Permutation[JJ+J2+Permutation[KK+K2]]] % 12;
		int GI3 = Permutation[II+1+Permutation[JJ+1+Permutation[KK+1]]] % 12;

		// Calculate the contribution from the four corners
		float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
		if(t0 < 0.0f) 
		{
			N0 = 0.0f;
		}
		else
		{
			t0 *= t0;
			N0 = t0 * t0 * Dot(Gradients[GI0], x0, y0, z0);
		}

		float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
		if(t1 < 0.0f) 
		{
			N1 = 0.0f;
		}
		else
		{
			t1 *= t1;
			N1 = t1 * t1 * Dot(Gradients[GI1], x1, y1, z1);
		}

		float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
		if(t2 < 0.0f) 
		{
			N2 = 0.0f;
		}
		else
		{
			t2 *= t2;
			N2 = t2 * t2 * Dot(Gradients[GI2], x2, y2, z2);
		}

		float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
		if(t3 < 0.0f) 
		{
			N3 = 0.0f;
		}
		else
		{
			t3 *= t3;
			N3 = t3 * t3 * Dot(Gradients[GI3], x3, y3, z3);
		}

		float Result = 16.0f*(N0 + N1 + N2 + N3) + 1.0f;
		return(Result);
	}

	float SimplexNoise(int Octaves, float X, float Y, float Z)
	{
		float Result = 0.0f;
		for(int I=0; I < Octaves; I++)
		{
			Result += Noise(X*pow(2, I), Y*pow(2, I), Z*pow(2, I));
		}
		return(Result);
	}
	
}