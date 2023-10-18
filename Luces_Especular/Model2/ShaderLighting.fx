// ShaderLighting.fx : Implements the shader.
//
//
//


Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);


cbuffer cbChangesEveryFrame : register(b0)
{
    matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
    matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
    matrix projMatrix;
};

cbuffer cbCameraPos : register(b3)
{
    float3 cameraPos;
};

cbuffer cbColorDifuso : register(b4)
{
    float3 colorDifuso1;
};

cbuffer cbMagnitudEspecular : register(b0)
{
    float magnitudEspecular;
};

cbuffer cbMagnitudEspecular2 : register(b1)
{
    float magnitudEspecular2;
};


struct VS_Input
{
    float4 pos  : POSITION;
    float2 tex0 : TEXCOORD0;
    float3 norm : NORMAL;
};


struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float3 norm : NORMAL;
    float3 lightVec : TEXCOORD1;
    float3 cameraVec : TEXCOORD2;
    float3 colorDifuso : TEXCOORD3;
    
    float3 lightVec2 : TEXCOORD4;
};


PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input) 0;

    // Initial transformations
    float4 worldPos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(worldPos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.tex0 = vertex.tex0;
    
    vsOut.norm = mul(vertex.norm, (float3x3)worldMatrix);
    vsOut.norm = normalize(vsOut.norm);
    
    // Set light position
    float3 lightPos = float3(10.0f, 100.0f, -100.0f); //luz estatica
    float3 lightPos2 = cameraPos; //luz sigue camara

    // Calculate light vector
    vsOut.lightVec = normalize(lightPos - worldPos);
    vsOut.lightVec2 = normalize(lightPos2 - worldPos);
    
    vsOut.colorDifuso = normalize(colorDifuso1);
    
    
    
    // Calculate camera vector
    vsOut.cameraVec = normalize(cameraPos - worldPos);

    return vsOut;
}


float4 PS_Main(PS_Input frag) : SV_TARGET
{
    // Light colors
    
    
    float3 iluminacionAmbiental = float3(0.2f, 0.2f, 0.2f);
    float3 colorDifuso = float3(0.2f, 0.8f, 0.6f);
    float3 colorDifuso2 = normalize(frag.colorDifuso);
    
    float3 colorEspecular = float3(0.5f, 0.5f, 0.5f);
    float3 colorEspecular2 = float3(0.5f, 0.5f, 0.5f);

    // Get parameters
    float3 normal = normalize(frag.norm);
    float3 lightVec = normalize(frag.lightVec);
    float3 cameraVec = normalize(frag.cameraVec);
    
    float3 lightVec2 = normalize(frag.lightVec2);

    // Calculate diffuse lighting
    float intensidadDifusa = saturate(dot(normal, lightVec));
    float3 iluminacionDifusa = colorDifuso * intensidadDifusa;
    
    float intensidadDifusa2 = saturate(dot(normal, lightVec2));
    float3 iluminacionDifusa2 = colorDifuso2 * intensidadDifusa2;
    
    //Calcular la intensidad de la reflexion especular
    float3 R = normalize(lightVec + cameraVec);
    float intensidadEspecular = pow(saturate(dot(normal, R)), magnitudEspecular);
    
    float3 R2 = normalize(lightVec2 + cameraVec);
    float intensidadEspecular2 = pow(saturate(dot(normal, R2)), magnitudEspecular2);

    // Calculate final color
    float3 iluminacionFinal = iluminacionAmbiental + (colorDifuso * intensidadDifusa) + (colorEspecular * intensidadEspecular) + (colorDifuso2 * intensidadDifusa2) + (colorEspecular2 * intensidadEspecular2);
    
    //float3 iluminacionFinal = iluminacionAmbiental + iluminacionDifusa;
    
    float4 textura = colorMap.Sample(colorSampler, frag.tex0);

    // Without texture
    return float4(iluminacionFinal, 1.0f);

    // With texture
    return colorMap.Sample(colorSampler, frag.tex0) * float4(iluminacionFinal, 1.0f);
}
