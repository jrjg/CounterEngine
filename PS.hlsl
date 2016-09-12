cbuffer cbObjectColors : register(b0)
{
	float4		g_vDiffuseObjectColor	: packoffset(c0);
	float4		g_vAmbientObjectColor	: packoffset(c1);
	bool		g_bHasTexture : packoffset(c2.x);
};


Texture2D	g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

struct PS_INPUT
{
	float4 vDiffuse		: COLOR0;
	float2 vTexcoord	: TEXCOORD0;
};


float4 PSMain(PS_INPUT Input) : SV_TARGET
{
	float4 vOutputColor;

	if (g_bHasTexture)
	vOutputColor = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord) * Input.vDiffuse;
	else
	vOutputColor = Input.vDiffuse;

	return vOutputColor;
}