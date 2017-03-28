#define AMBIENT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 3
#define EPSILON 0.1

struct GeometryInput
{
	float4 position : SV_POSITION;
};

struct GeometryOutput
{
	float4 position : SV_POSITION;
};

cbuffer PerFrame : register(b0)
{
	float4x4    pvMatrix;
	float3      cameraPos;
	float       perFrameSpacing0;
	float3      cameraUp;
	float       perFrameSpacing1;
	float2      screenSize;
	float       deltaTime;
	float       perFrameSpacing2;
};

cbuffer PerLight : register(b2)
{
	float3      position;
	int         type;
	float4      ambient;
	float4      diffuse;
	float4      specular;
	float3      direction;
	float       cutoff;
	float3      attenuation;
	int         spotExponent;
};

void EmitQuad(float3 startVertex, float3 endVertex, 
	inout TriangleStream<GeometryOutput> output)
{
	float3 lightDir = float3(1.0f, 0.0f, 0.0f);

	if (type != DIRECTIONAL_LIGHT)
	{
		lightDir = normalize(startVertex - position);
	}
	else
	{
		lightDir = direction;
	}

	GeometryOutput gout1;
	gout1.position = mul(pvMatrix, float4(startVertex + lightDir * EPSILON, 1.0f));
	output.Append(gout1);

	GeometryOutput gout2;
	gout2.position = mul(pvMatrix, float4(lightDir, 0.0f));
	output.Append(gout2);

	if (type != DIRECTIONAL_LIGHT)
	{
		lightDir = normalize(endVertex - position);
	}

	GeometryOutput gout3;
	gout3.position = mul(pvMatrix, float4(endVertex + lightDir * EPSILON, 1.0f));
	output.Append(gout3);

	GeometryOutput gout4;
	gout4.position = mul(pvMatrix, float4(lightDir, 0.0f));
	output.Append(gout4);

	output.RestartStrip();
}

[maxvertexcount(18)]
void main(triangleadj GeometryInput input[6], inout TriangleStream<GeometryOutput> output)
{
	float3 e1 = input[2].position.xyz - input[0].position.xyz;
	float3 e2 = input[4].position.xyz - input[0].position.xyz;
	float3 e3 = input[1].position.xyz - input[0].position.xyz;
	float3 e4 = input[3].position.xyz - input[2].position.xyz;
	float3 e5 = input[4].position.xyz - input[2].position.xyz;
	float3 e6 = input[5].position.xyz - input[0].position.xyz;

	float3 normal = normalize(cross(e1, e2));
	float3 lightDir = float3(1.0f, 0.0f, 0.0f);

	if (type != DIRECTIONAL_LIGHT)
	{
		lightDir = normalize(position - input[0].position.xyz);
	}
	else
	{
		lightDir = direction;
	}

	if (dot(normal, lightDir) > 0)
	{
		normal = cross(e3, e1);

		if (dot(normal, lightDir) <= 0)
		{
			float3 startVertex = input[0].position.xyz;
			float3 endVertex   = input[2].position.xyz;
			EmitQuad(startVertex, endVertex, output);
		}

		normal = cross(e4, e5);
		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(position - input[2].position.xyz);
		}

		if (dot(normal, lightDir) <= 0)
		{
			float3 startVertex = input[2].position.xyz;
			float3 endVertex   = input[4].position.xyz;
			EmitQuad(startVertex, endVertex, output);
		}

		normal = cross(e2, e6);
		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(position - input[4].position.xyz);
		}

		if (dot(normal, lightDir) <= 0)
		{
			float3 startVertex = input[4].position.xyz;
			float3 endVertex   = input[0].position.xyz;
			EmitQuad(startVertex, endVertex, output);
		}

		// front cap
		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(input[0].position.xyz - position);
		}
		GeometryOutput gout1;
		gout1.position = mul(pvMatrix, float4(input[0].position.xyz + lightDir * EPSILON, 1.0f));
		output.Append(gout1);

		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(input[2].position.xyz - position);
		}
		GeometryOutput gout2;
		gout2.position = mul(pvMatrix, float4(input[2].position.xyz + lightDir * EPSILON, 1.0f));
		output.Append(gout2);

		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(input[4].position.xyz - position);
		}
		GeometryOutput gout3;
		gout3.position = mul(pvMatrix, float4(input[4].position.xyz + lightDir * EPSILON, 1.0f));
		output.Append(gout3);

		output.RestartStrip();

		// back cap
		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(input[0].position.xyz - position);
		}
		GeometryOutput gout4;
		gout4.position = mul(pvMatrix, float4(lightDir, 0.0f));
		output.Append(gout4);

		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(input[4].position.xyz - position);
		}
		GeometryOutput gout5;
		gout5.position = mul(pvMatrix, float4(lightDir, 0.0f));
		output.Append(gout5);

		if (type != DIRECTIONAL_LIGHT)
		{
			lightDir = normalize(input[2].position.xyz - position);
		}
		GeometryOutput gout6;
		gout6.position = mul(pvMatrix, float4(lightDir, 0.0f));
		output.Append(gout6);

		output.RestartStrip();
	}
}