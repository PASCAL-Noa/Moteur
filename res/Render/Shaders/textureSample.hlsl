<!--#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 1
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
    #define NUM_SPOT_LIGHTS 0
#endif

// Include structures and functions for lighting.
#include "LightingUtil.hlsl"-->

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

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
    Light gLight;
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

    // Transforme la position en espace monde
    vout.PosW = mul(float4(vin.PosL, 1.0), gWorld).xyz;

    // Transforme la position en espace de clipping
    vout.PosH = mul(float4(vin.PosL, 1.0), gWorldViewProj);

    // Transforme la normale en espace monde (en utilisant la matrice inverse transposée)
    float3x3 worldInverseTranspose = InverseTranspose(gWorld);
    vout.NormalW = normalize(mul(vin.NormalL, worldInverseTranspose));

    // Passe les coordonnées de texture
    vout.uv = vin.uv;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    // Récupère la couleur de la texture
    float3 sampleColor = myTexture.Sample(mySampler, pin.uv).rgb;

    float3 normalW = pin.NormalW;

    // Calcul de la lumière ambiante
    float3 ambientLight = gAmbientColor.rgb * gLight.Strength;

    // Vecteur directionnel vers la lumière
    float3 vectorToLight = normalize(gLight.Position - pin.PosW);
    float3 toEyeW = normalize(gEyePosW - pin.PosW);


    // Calcul de l'intensité de la lumière diffuse (Lambert)
    float diffuseLightIntensity = max(dot(normalW, vectorToLight), 0.0f);

    // Calcul de la distance à la lumière (plus efficace que distance())
    float distanceToLight = length(gLight.Position - pin.PosW);

    // Calcul de l'atténuation
    float attenuationFactor = 1.0 / (1.0 + 0.1 * distanceToLight + 0.001 * pow(distanceToLight, 2));
    diffuseLightIntensity *= attenuationFactor;

    // Couleur de la lumière diffuse
    float3 diffuseLight = diffuseLightIntensity * gLight.Strength * gLight.Color.rgb;

    // Lumière totale appliquée
    float3 appliedLight = ambientLight + diffuseLight;

    // Applique la lumière à la texture
    float3 finalColor = sampleColor * appliedLight;

    // Retourne la couleur finale
    return float4(finalColor, 1.0f);
}
