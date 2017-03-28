struct PixelIn
{
	float4		position : SV_POSITION;
	float3		texCoord : TEXCOORD;
};

struct PixelOut
{
	float4		fragColor : SV_TARGET;
};

TextureCube skyboxTexture;
SamplerState skyboxSampler;

PixelOut main(PixelIn pin)
{
	PixelOut pout;

	pout.fragColor = skyboxTexture.Sample(skyboxSampler, pin.texCoord);

	return pout;
}