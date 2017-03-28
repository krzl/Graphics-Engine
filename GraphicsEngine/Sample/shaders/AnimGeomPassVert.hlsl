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
	float4      position        : SV_POSITION;
	float3      positionModel   : POSITION;
	float3      normal          : NORMAL;
	float2      texCoord        : TEXCOORD;
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
	float4 newNormal;

	newPosition  = mul(bonesData[vin.boneIndex.x], float4(vin.position, 1.0) * vin.boneWeight.x);
	newNormal    = mul(bonesData[vin.boneIndex.x], float4(vin.normal, 0.0)   * vin.boneWeight.x);
	newPosition += mul(bonesData[vin.boneIndex.y], float4(vin.position, 1.0) * vin.boneWeight.y);
	newNormal   += mul(bonesData[vin.boneIndex.y], float4(vin.normal, 0.0)   * vin.boneWeight.y);
	newPosition += mul(bonesData[vin.boneIndex.z], float4(vin.position, 1.0) * vin.boneWeight.z);
	newNormal   += mul(bonesData[vin.boneIndex.z], float4(vin.normal, 0.0)   * vin.boneWeight.z);
	newPosition += mul(bonesData[vin.boneIndex.w], float4(vin.position, 1.0) * vin.boneWeight.w);
	newNormal   += mul(bonesData[vin.boneIndex.w], float4(vin.normal, 0.0)   * vin.boneWeight.w);

	vout.position		= mul(pvmMatrix,	float4(newPosition.xyz * scaleVector, 1.0f));
	vout.normal			= mul(modelMatrix,	float4(newNormal.xyz, 0.0f)).xyz;
	vout.positionModel	= mul(modelMatrix,	float4(newPosition.xyz, 1.0f)).xyz;
	vout.texCoord		= vin.texCoord;

	return vout;
}