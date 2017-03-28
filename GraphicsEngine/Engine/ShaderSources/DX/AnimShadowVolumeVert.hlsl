struct VertexIn
{
	float3      position : POSITION;
	float3      normal   : NORMAL;
	float2      texCoord : TEXCOORD;
	float4      boneIndex : BLENDINDICES;
	float4      boneWeight : BLENDWEIGHT;
};

struct VertexOut
{
	float4      position : SV_POSITION;
};

static const int MAX_BONES = 64;

cbuffer PerEntity : register(b1)
{
	float4x4    pvmMatrix;
	float4x4    modelMatrix;
	float3      scaleVector;
	float       perEntitySpacing0;
};

cbuffer PerSkinning : register(b3)
{
	float4x4    bonesData[MAX_BONES];
}

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	float4 newPosition;

	newPosition  = mul(bonesData[vin.boneIndex.x], float4(vin.position, 1.0) * vin.boneWeight.x);
	newPosition += mul(bonesData[vin.boneIndex.y], float4(vin.position, 1.0) * vin.boneWeight.y);
	newPosition += mul(bonesData[vin.boneIndex.z], float4(vin.position, 1.0) * vin.boneWeight.z);
	newPosition += mul(bonesData[vin.boneIndex.w], float4(vin.position, 1.0) * vin.boneWeight.w);

	vout.position = mul(modelMatrix, float4(newPosition.xyz * scaleVector, 1.0f));

	return vout;
}