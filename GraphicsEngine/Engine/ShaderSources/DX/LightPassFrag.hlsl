#define AMBIENT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 3

struct PixelIn
{
	float4      position  : SV_POSITION;
};

struct PixelOut
{
	float4      fragColor : SV_TARGET0;
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

Texture2D positionMap;
SamplerState positionSampler;

Texture2D colorMap;
SamplerState colorSampler;

Texture2D normalMap;
SamplerState normalSampler;

float4 CalcPointLight(float3 worldPos, float3 normal)
{
	float3 lightDir = worldPos - position;
	float distance  = length(lightDir);
	lightDir        = normalize(lightDir);

	float4 diffuseColor  = float4(0, 0, 0, 0);
	float4 specularColor = float4(0, 0, 0, 0);

	float diffuseFactor = dot(normal, -lightDir);

	if (diffuseFactor > 0.0)
	{
		diffuseColor = diffuse * diffuseFactor;

		float3 vertexToCam  = normalize(cameraPos - worldPos);
		float3 lightReflect = normalize(reflect(lightDir, normal));

		float specularFactor = dot(vertexToCam, lightReflect);

		if (specularFactor > 0.0f)
		{
			specularFactor = pow(abs(specularFactor), 20);
			specularColor  = specular * specularFactor;
		}
	}

	float4 color = diffuseColor + specularColor;
	float atten =	attenuation.x +
					attenuation.y * distance +
					attenuation.z * distance * distance;

	atten = max(1.0, atten);

	return color / atten;
}

float4 CalcDirectionalLight(float3 worldPos, float3 normal)
{
	float4 diffuseColor  = float4(0, 0, 0, 0);
	float4 specularColor = float4(0, 0, 0, 0);
	float diffuseFactor = dot(normal, -direction);

	if (diffuseFactor > 0.0f)
	{
		diffuseColor = diffuse * diffuseFactor;

		float3 vertexToEye  = normalize(cameraPos - worldPos);
		float3 lightReflect = normalize(reflect(direction, normal));

		float specularFactor = dot(vertexToEye, lightReflect);

		if (specularFactor > 0.0f)
		{
			specularFactor = pow(abs(specularFactor), 20);
			specularColor  = specular * specularFactor;
		}
	}

	return diffuseColor + specularColor;
}

float4 CalcSpotLight(float3 worldPos, float3 normal)
{
	float3 lightDir = worldPos - position;
	float distance  = length(lightDir);
	
	lightDir = normalize(lightDir);
	float cosLight = max(0.0f, dot(-lightDir, normalize(position - direction)));

	if (cosLight < cos(radians(cutoff)))
	{
		return float4(0, 0, 0, 0);
	}
	else
	{
		float4 diffuseColor  = float4(0, 0, 0, 0);
		float4 specularColor = float4(0, 0, 0, 0);

		float diffuseFactor = dot(normal, -lightDir);

		if (diffuseFactor > 0)
		{
			diffuseColor = diffuse * diffuseFactor;

			float3 vertexToEye   = normalize(cameraPos - worldPos);
			float3 lightReflect  = normalize(reflect(lightDir, normal));

			float specularFactor = dot(vertexToEye, lightReflect);
			specularFactor       = pow(abs(specularFactor), 20);

			if (specularFactor > 0)
			{
				specularColor = specular * specularFactor;
			}
		}

		float4 color = diffuseColor + specularColor;

		float atten =	attenuation.x +
						attenuation.y * distance +
						attenuation.z * distance * distance;

		atten = atten * pow(cosLight, spotExponent);
		atten = max(1.0f, atten);

		return color / atten;
	}
}

PixelOut main(PixelIn pin)
{
	PixelOut pout;

	float2 texCoord = pin.position.xy / screenSize;
	float3 worldPos = positionMap.Sample(positionSampler, texCoord).xyz;
	float3 color    = colorMap.   Sample(colorSampler,    texCoord).xyz;
	float3 normal   = normalMap.  Sample(normalSampler,   texCoord).xyz;

	normal = normalize(normal);

	if (type == AMBIENT_LIGHT)
	{
		pout.fragColor = float4(color, 1.0f) * CalcPointLight(worldPos, normal);
	}
	else if (type == DIRECTIONAL_LIGHT)
	{
		pout.fragColor = float4(color, 1.0f) * CalcDirectionalLight(worldPos, normal);
	}
	else if(type == POINT_LIGHT)
	{
		pout.fragColor = float4(color, 1.0f) * CalcPointLight(worldPos, normal);
	}
	else if(type == SPOT_LIGHT)
	{
		pout.fragColor = float4(color, 1.0f) * CalcSpotLight(worldPos, normal);
	}
	else{
		pout.fragColor = float4(0, 0, 0, 0);
	}
	
	return pout;
}