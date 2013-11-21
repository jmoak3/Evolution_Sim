
uniform extern float4x4 gWVP;

uniform extern texture gTex;
uniform extern texture gTex2;
uniform extern texture gTex3;


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


struct EFOutputVS
{
    float4 posH    : POSITION0;
    float2 tex0    : TEXCOORD0;
};


EFOutputVS EFEvolutionVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0)
{			
    // Zero out our output.
	EFOutputVS outVS = (EFOutputVS)0;
		
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;
	
	// Done--return the output.
    return outVS;
}

float4 EFEvolutionPS(float2 tex0 : TEXCOORD0) : COLOR
{
	float4 texColor = tex2D(TexS, tex0);
	return float4(texColor);
}


technique EFEvolutionShader
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 EFEvolutionVS();
        pixelShader  = compile ps_2_0 EFEvolutionPS();
    }
}


struct LOutputVS
{
    float4 posH    : POSITION0;
    float2 tex0    : TEXCOORD0;
	float2 whichTex: TEXCOORD1;
};


LOutputVS LEvolutionVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0, 
					 float2 TexStuff : TEXCOORD1) 
{			
    // Zero out our output.
	LOutputVS outVS = (LOutputVS)0;
		
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;


	outVS.whichTex = TexStuff;
	
	
	// Done--return the output.
    return outVS;
}

float4 LEvolutionPS(float2 tex0 : TEXCOORD0, float2 whichTex : TEXCOORD1) : COLOR
{
	float4 texColor;
	texColor = tex2D(TexS, tex0);
	//src color * src alpha + dest color * (1-src alpha)
		
	float cAlp = whichTex.x; 
	float sAlp = whichTex.y;

	if (cAlp > 1.0f)
		cAlp = 1.0f;
	if (cAlp < 0.0f)
		cAlp = 0.0f;

	if (sAlp > 1.0f)
		sAlp = 1.0f;
	if (sAlp < 0.0f)
		sAlp = 0.0f;

	//float totalInverse = 1.0f / (sAlp + cAlp);
	float4 carn = tex2D(TexS2, tex0).rgba;
	float4 sight = tex2D(TexS3, tex0).rgba;
	


	//carn.rgba *= cAlp * totalInverse;
	//sight.rgba *= sAlp * totalInverse;

	//texColor.rgb = lerp(texColor.rgb, sight.rgb, sAlp) + lerp(texColor.rgb, carn.rgb, cAlp);
	//texColor.a = texColor.a + sight.a + carn.a;
	
	float3 cLerp = lerp(1.0f, carn.rgb, cAlp);
	float3 sLerp = lerp(1.0f, sight.rgb, sAlp);

	texColor.a = texColor.a + sAlp*sight.a + cAlp*carn.a;

	if (sight.a > 0.0f && carn.a > 0.0f)
	{
		texColor.rgb = sLerp + cLerp + texColor.rgb;
	}
	else if (sight.a > 0.0f)
	{
		texColor.rgb = sLerp + texColor.rgb;
	}
	else if (carn.a > 0.0f)
	{
		texColor.rgb = cLerp + texColor.rgb;			
	}		
	return float4(texColor);
}

technique LEvolutionShader
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 LEvolutionVS();
        pixelShader  = compile ps_2_0 LEvolutionPS();
    }
}

