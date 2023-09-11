/////////////////////
//Filename: light.vs
/////////////////////

///////////
//GLOBALS//
///////////
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix traMatrix;
	matrix pading;
	matrix pading2;

};

////////////
//TYPEDEFS//
////////////
struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float2 tex2 : TEXCOORD0;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float2 tex2 : TEXCOORD1;
};

/////////////////
//Vertex Shader
////////////////
PixelInputType LightVertexShader(VertexInputType input){
	PixelInputType output;

	//Change the position of the vector to be 4 units for proper matrix calculations
	input.position.w = 1.0f;

	//calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Store the texture coordinates for the pixel shader
	output.tex = mul(input.tex, traMatrix);
	output.tex2 = mul(input.tex2, traMatrix);

	//Calculate the normal vector against the world matrix only
	//output.normal =  mul(input.normal,(float3x3)worldMatrix);
	output.normal =  mul(input.normal,(float3x3)traMatrix);

	//Normalize the normal vector
	output.normal = normalize(output.normal);

	return output;
};

