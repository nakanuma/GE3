#include "Toon.hlsli"

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // ライティングの計算
    float lightingFactor = 1.0f;
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        lightingFactor = saturate(pow(NdotL * 0.5f + 0.5f, 2.0f));
    }

    // トゥーンシェーディングの適用
    float toonStep = 4; // エッジの急峻さを調整するためのパラメータ
    float toon = round(lightingFactor * toonStep) / toonStep;

    // 最終的な出力色
    output.color = gMaterial.color * textureColor * toon;

    return output;
}