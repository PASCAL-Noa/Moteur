#ifndef NUM_DIR_LIGHTS
#define NUM_DIR_LIGHTS 2
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
#define NUM_SPOT_LIGHTS 0
#endif


//#include "LightingUtil.hlsl"

#define MaxLights 16


struct Light
{
    float3 Position;
    float3 Direction;
    float4 Color;
    float3 Strength;
    float FalloffStart;
    float FalloffEnd;
    float SpotPower;

};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    float denom = max(falloffEnd - falloffStart, 1e-5f); // Évite la division par zéro
    return saturate((falloffEnd - d) / denom);
}


float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));
    float f0 = 1.0f - cosIncidentAngle;
    return R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye)
{
    const float shininess = 10.0f;
    float3 halfVec = normalize(toEye + lightVec);
    float spec = pow(max(dot(halfVec, normal), 0.0f), shininess);
    return (lightStrength * 0.5f) + (lightStrength * spec);
}

float3 ComputeDirectionalLight(Light L, float3 normal, float3 toEye, float4 ambiantcolor)
{
    float3 lightVec = -L.Direction;
    float ndotl = max(dot(lightVec, normal), 0.0f);

    float3 lightStrength = L.Strength * ndotl + ambiantcolor.rgb * 0.6; 

    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}

float3 ComputePointLight(Light L, float3 pos, float3 normal, float3 toEye)
{
    float3 lightVec = L.Position - pos;
    float d = length(lightVec);
    if (d > L.FalloffEnd || d < 1e-5f)
        return 0.0f;

    lightVec /= d;
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl * CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}



float3 ComputeSpotLight(Light L, float3 pos, float3 normal, float3 toEye)
{
    float3 lightVec = L.Position - pos;
    float d = length(lightVec);
    if (d > L.FalloffEnd)
        return 0.0f;
    lightVec /= d;
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl * CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
    float spotFactor = pow(max(dot(-lightVec, L.Direction), 0.0f), L.SpotPower);
    lightStrength *= spotFactor;
    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}

float4 ComputeLighting(Light gLights[MaxLights],
                       float3 pos, float3 normal, float3 toEye,
                       float3 shadowFactor, float4 ambiantcolor)
{
    float3 result = 0.0f;

    int i = 0;

#if (NUM_DIR_LIGHTS > 0)
    for (i = 0; i < NUM_DIR_LIGHTS; ++i)
    {
        result += shadowFactor * ComputeDirectionalLight(gLights[i], normal, toEye, ambiantcolor);
    }
#endif

#if (NUM_POINT_LIGHTS > 0)
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
    {
        result += ComputePointLight(gLights[i], pos, normal, toEye);
    }
#endif

#if (NUM_SPOT_LIGHTS > 0)
    for(i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
    {
        result += ComputeSpotLight(gLights[i], pos, normal, toEye);
    }
#endif 

    return float4(result, 0.0f);
}


Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);


cbuffer ObjectConstants : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProjection;
};

cbuffer PassConstants : register(b1)
{
    float4 gAmbientColor;
    float3 gEyePosW;
    Light gLight[MaxLights];
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 uv : TEXCOORD;
};

float3x3 InverseTranspose(float4x4 m)
{
    float3x3 upper3x3 = (float3x3) m;
    return transpose(upper3x3);
}

VertexOut VS(VertexIn vin)
{
    VertexOut vout;


    vout.PosW = mul(float4(vin.PosL, 1.0), gWorld).xyz;

    vout.PosH = mul(float4(vin.PosL, 1.0), gWorldViewProj);

    float3x3 worldInverseTranspose = InverseTranspose(gWorld);
    vout.NormalW = normalize(mul(vin.NormalL, worldInverseTranspose));

    vout.uv = vin.uv;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float3 sampleColor = myTexture.Sample(mySampler, pin.uv).rgb;

    float3 normalW = normalize(pin.NormalW);
    float3 toEyeW = normalize(gEyePosW - pin.PosW);

    float3 shadowFactor = 1.0f; 

    float3 appliedLight = ComputeLighting(gLight, pin.PosW, normalW, toEyeW, shadowFactor, gAmbientColor).rgb;

    float3 finalColor = sampleColor * appliedLight;

    return float4(finalColor, 1.0f);
}