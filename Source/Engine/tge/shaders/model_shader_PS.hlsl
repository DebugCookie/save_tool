#include "common.hlsli"

#define DIFFUSE_MAP 0
#define NORMAL_MAP 1

SamplerState SampleType;
Texture2D shaderTextures[2] : register( t1 );

float4 main(ModelVertexToPixel input) : SV_TARGET
{
	float4 Diffuse = shaderTextures[DIFFUSE_MAP].Sample(SampleType, input.texCoord0) * input.vertexColor0;
	
	float3 AmbientColor = Diffuse.xyz * AmbientLightColorAndIntensity.xyz;
	float3 DirectColor = float3(0, 0, 0);
	
	if (Diffuse.w <= alphaTestThreshold)
	{
		discard;
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return float4(AmbientColor + DirectColor, Diffuse.w);
}