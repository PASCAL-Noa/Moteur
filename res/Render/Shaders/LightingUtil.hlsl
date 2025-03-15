//***************************************************************************************
// LightingUtil.hlsl (adapté sans Material)
//***************************************************************************************

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
    const float shininess = 32.0f;
    float3 halfVec = normalize(toEye + lightVec);
    float spec = pow(max(dot(halfVec, normal), 0.0f), shininess);
    return (lightStrength * 0.5f) + (lightStrength * spec);
}

float3 ComputeDirectionalLight(Light L, float3 normal, float3 toEye)
{
    float3 lightVec = -L.Direction;
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;
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
                       float3 shadowFactor)
{
    float3 result = 0.0f;

    int i = 0;

#if (NUM_DIR_LIGHTS > 0)
    for(i = 0; i < NUM_DIR_LIGHTS; ++i)
    {
        result += shadowFactor * ComputeDirectionalLight(gLights[i], normal, toEye);
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
