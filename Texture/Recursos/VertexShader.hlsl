//VertexShader.hlsl
//
//
//

//Estructura de Entrada y de Salida para el Shader
struct VS_Input {
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;//una sola uv con TEXCOORD
};

//salida del vertex se combierte en la entrada del pixel
struct PS_Output {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

//cada ves que mandamos a renderizar nuestra escena 
PS_Output VS_Main(VS_Input vertices)
{
    PS_Output psOut = (PS_Output)0;

    psOut.pos = vertices.pos;
    psOut.uv = vertices.uv;

    return psOut;
}