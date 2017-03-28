struct VertexIn
{
	float3		position : POSITION;
	float3		normal   : NORMAL;
	float2		texCoord : TEXCOORD;
};

struct VertexOut
{
	float4		position : SV_POSITION;
};

cbuffer PerEntity : register(b1)
{
	float4x4	pvmMatrix;
	float4x4	modelMatrix;
	float3		scaleVector;
	float		perEntitySpacing0;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	vout.position = mul(modelMatrix, float4(vin.position * scaleVector, 1.0f));

	return vout;
}