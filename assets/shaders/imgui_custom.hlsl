struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

SamplerState sampler0 : register(s0);
Texture2D texture0 : register(t0);

float LinearizeDepth(float depth, float nearPlane, float farPlane)
{
    return (2.0 * nearPlane) / (farPlane + nearPlane - depth * (farPlane - nearPlane));
}

float4 PSMain(PS_INPUT input) : SV_Target
{
  float4 out_col = input.col * texture0.Sample(sampler0, input.uv);
  out_col.rgb = LinearizeDepth(out_col.r, 0.1f, 400);
  return out_col;
}