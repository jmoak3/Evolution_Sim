
uniform extern float4x4 gWVP;

uniform extern texture gTex;
uniform extern texture gTex2;
uniform extern texture gTex3;
uniform extern texture gTex4;
uniform extern texture gTex5;


sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler TexS2 = sampler_state
{
	Texture = <gTex2>;
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
 
sampler TexS3 = sampler_state
{
	Texture = <gTex3>;
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler TexS4 = sampler_state
{
	Texture = <gTex4>;
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler TexS5 = sampler_state
{
	Texture = <gTex5>;
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};


struct OutputVS
{
    float4 posH    : POSITION0;
    float2 tex0    : TEXCOORD0;		
	float3 whichTex : TEXCOORD1;
};

OutputVS EvolutionVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0, 
					 float4x4 trans : TEXCOORD1, float3 TexStuff : TEXCOORD5) 
{			
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
		
	outVS.posH = mul( mul(float4(posL, 1.0f), transpose(trans)),  gWVP);

	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;


	outVS.whichTex = TexStuff;
	
	
	// Done--return the output.
    return outVS;
}

float4 EvolutionPS(float2 tex0 : TEXCOORD0, float3 whichTex : TEXCOORD1) : COLOR
{
	float4 texColor;
	if (whichTex.x < 0.2f)
	{
		texColor = tex2D(TexS, tex0);
	}
	else if (whichTex.x > 0.9f && whichTex.x < 1.1f)
	{
		texColor = tex2D(TexS2, tex0);
	}
	else if (whichTex.x > 1.9f)
	{
		texColor = tex2D(TexS3, tex0);
		//src color * src alpha + dest color * (1-src alpha)
		
		float cAlp = whichTex.y; 
		float sAlp = whichTex.z;

		if (cAlp > 1.0f)
			cAlp = 1.0f;
		if (cAlp < 0.0f)
			cAlp = 0.0f;

		if (sAlp > 1.0f)
			sAlp = 1.0f;
		if (sAlp < 0.0f)
			sAlp = 0.0f;

		float totalInverse = 1.0f / (sAlp + cAlp);
		float3 carn = tex2D(TexS5, tex0).rgb;
		float3 sight = tex2D(TexS4, tex0).rgb;
		//carn.rgb *= whichTex.y;
		//sight.rgb *= whichTex.z;
		
		carn *= cAlp * totalInverse;
		sight *= sAlp * totalInverse;

		texColor.rgb = lerp(texColor.rgb, sight.rgb, sAlp) + lerp(texColor.rgb, carn.rgb, cAlp);
	}
	
	return float4(texColor);
}

technique EvolutionShader
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_3_0 EvolutionVS();
        pixelShader  = compile ps_3_0 EvolutionPS();
    }
}
