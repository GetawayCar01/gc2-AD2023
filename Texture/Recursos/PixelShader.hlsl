//PixelShader.hlsl



//Constantes Globales para la Textura
Texture2D colorMap1 : register(t0);
Texture2D colorMap2 : register(t1);


//TODO: poner otra textura y subir o bajar colores 
SamplerState colorSampler : register(s0);


//Estructura de Entrada para el Shader
struct PS_Input {//ahora se combierte en entrada del pixel shader
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};


float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float color1 = colorMap1.Sample(colorSampler, frag.uv);
    float color2 = colorMap2.Sample(colorSampler, frag.uv);

    //----Modificar el brillo y el contraste de la primera textura-----
    float brightness = 0.0; // Factor de brillo
    float contrast = 1.0; // Factor de contraste
    color1 = (color1 - 0.5) * contrast + 0.5 + brightness;


    //----Invierte los colores de la segunda textura
    color2 = 0.1 + color2;

    float4 colorFinal = color1 + color2;

    return colorFinal;
}
