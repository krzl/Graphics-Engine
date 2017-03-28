struct VertexIn
{
	float3      position : POSITION;
	float3      normal   : NORMAL;
	float2      texCoord : TEXCOORD;
};

struct VertexOut
{
	float4      position        : SV_POSITION;
	float3      positionModel   : POSITION;
	float3      normal          : NORMAL;
	float2      texCoord        : TEXCOORD;
};

cbuffer PerEntity : register(b1)
{
	float4x4    pvmMatrix;
	float4x4    modelMatrix;
	float3      scaleVector;
	float       perEntitySpacing0;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	float3 pos = vin.position * scaleVector;

	vout.normal = mul(modelMatrix, float4(vin.normal, 0.0f)).xyz;
	vout.positionModel = mul(modelMatrix, float4(pos.xyz, 1.0f)).xyz;
	vout.texCoord = vout.positionModel.xz;
	vout.position = mul(pvmMatrix, float4(pos, 1.0f));

	return vout;
}