//-----------------------------------------------------------------------------
// File: XuiEffect.hlsl
//
// Desc: HLSL file for the XuiEffect sample. 
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Vertex shader output structures
//-----------------------------------------------------------------------------
struct VSOUT
{
    float4 Position : POSITION;
    float2 uvTex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex shader constants
//--------------------------------------------------------------------------------------
uniform float4x4 WorldViewProj : register(c0);

sampler2D TextureSampler : register(s0);
sampler2D TextureSampler1 : register(s1);

//--------------------------------------------------------------------------------------
// Pixel shader constants
//--------------------------------------------------------------------------------------
uniform float4 ColorFactor : register(c0);  // color to modulate with
uniform float4 DisplacementFactor : register(c1);
uniform float4 Displacement : register(c2);


//--------------------------------------------------------------------------------------
// Name: ShadeVertex
// Desc: Vertex shader
//--------------------------------------------------------------------------------------
VSOUT ShadeVertex
(
    const float2 Position0 : POSITION0,
    const float2 uv : TEXCOORD0
)
{
    VSOUT Output;
    float4 pos; // position in screen space
    pos = mul(float4(Position0.x, Position0.y, 0, 1), WorldViewProj);
    pos.x -= .5;
    pos.y -= .5;
    pos.z = 0;

    Output.Position=pos;

    Output.uvTex.x = uv.x;
    Output.uvTex.y = uv.y;

    return Output;
}


//--------------------------------------------------------------------------------------
// Name: ShadePixel
// Desc: Pixel shader for a simple 2d effect
//--------------------------------------------------------------------------------------
float4 ShadePixel
(
    VSOUT Input
) : COLOR
{
    float2 displaceUV = Input.uvTex;
    displaceUV.x += Displacement.x;
    
    float4 clr = tex2D(TextureSampler1, displaceUV);
    
    float2 uv = Input.uvTex;
    uv.x += (.15 - clr.y/2)*DisplacementFactor.y;
    uv.y += (.15 - clr.y/2)*DisplacementFactor.y;

    return tex2D(TextureSampler, uv)*ColorFactor;
}
