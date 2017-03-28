struct PixelIn
{
	float4      position        : SV_POSITION;
	float3      positionModel   : POSITION;
	float3      normal          : NORMAL;
	float2      texCoord        : TEXCOORD;
};

struct PixelOut
{
	float4      positionModel   : SV_TARGET0;
	float4      diffuse         : SV_TARGET1;
	float4      normal          : SV_TARGET2;
	float4      entityID        : SV_TARGET3;
	//float4      texCoord      : SV_TARGET4; // currently unused
};

Texture2D diffuseTexture;
SamplerState diffuseSampler
{
	AddressU = REPEAT;
	AddressV = REPEAT;
};

cbuffer PerEntity : register(b1)
{
	float4x4    pvmMatrix;
	float4x4    modelMatrix;
	float3      scaleVector;
	int         entityID;
};

PixelOut main(PixelIn pin)
{
	PixelOut pout;

	pout.positionModel  = float4(pin.positionModel, 1.f);
	pout.diffuse        = diffuseTexture.Sample(diffuseSampler, pin.texCoord);
	pout.normal         = float4(normalize(pin.normal), 0.0f);
	pout.entityID       = entityID;
	//pout.texCoord       = float4(pin.texCoord, 0.f, 0.f);

	return pout;
}