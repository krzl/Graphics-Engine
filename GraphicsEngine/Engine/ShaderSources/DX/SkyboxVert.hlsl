struct VertexIn
{
	float3		position : POSITION;
};

struct VertexOut
{
	float4		position : SV_POSITION;
	float3		texCoord : TEXCOORD;
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

	vout.position	= mul(pvmMatrix, float4(vin.position, 1.0f)).xyww;
	vout.texCoord	= vin.position;
	vout.texCoord.y = - vout.texCoord.y;

	return vout;
}