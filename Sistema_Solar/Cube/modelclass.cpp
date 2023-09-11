//////////////////////////
//Filename: modelclass.cpp
//////////////////////////
#include "modelclass.h"

ModelClass::ModelClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_Texture2 = 0;
	m_Texture3 = 0;
	m_Texture4 = 0;
	m_Texture5 = 0;
	m_Texture6 = 0;
	m_Texture7 = 0;
	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other){
}

ModelClass::~ModelClass(){
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFileName, WCHAR* textureFilename, WCHAR* textureFilename2, WCHAR* textureFilename3, WCHAR* textureFilename4, WCHAR* textureFilename5, WCHAR* textureFilename6, WCHAR* textureFilename7)
{
	bool result;

	//Load the model data
	result = LoadModel(modelFileName);
	if(!result)
		return false;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
		return false;

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;
	result = LoadTexture2(device, textureFilename2);
	if(!result)
		return false;
	result = LoadTexture3(device, textureFilename3);
	if (!result)
		return false;
	result = LoadTexture4(device, textureFilename4);
	if (!result)
		return false;
	result = LoadTexture5(device, textureFilename5);
	if (!result)
		return false;
	result = LoadTexture6(device, textureFilename6);
	if (!result)
		return false;
	result = LoadTexture7(device, textureFilename7);
	if (!result)
		return false;

	return true;
}

void ModelClass::Shutdown(){

	//Release the model texture
	ReleaseTexture();

	//Release the vertex and index buffers;
	ShutdownBuffers();

	//Release the model data
	ReleaseModel();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext){

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount(){

	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture(){
	return m_Texture->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture2(){
	return m_Texture2->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture3(){
	return m_Texture3->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture4(){
	return m_Texture4->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture5(){
	return m_Texture5->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture6(){
	return m_Texture6->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTexture7(){
	return m_Texture7->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device){

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	int slices = 20;
	int stacks = 20;

	int numVerticesPerRow = slices + 1;
	int numVerticesPerColumn = stacks + 1;

	int numVertices = numVerticesPerRow * numVerticesPerColumn;

	int vertexStride = sizeof(VertexType)* numVertices; // 12 bytes
	int SizeOfVertexBufferInBytes = numVertices * vertexStride;

	int numIndices = slices * stacks * 6;
	m_vertexCount = numVertices;
	m_indexCount = 2 * numIndices;

	//Create the vertex array
	vertices = new VertexType[numVertices];
	if (!vertices)
		return false;

	////Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	unsigned int countv = 0;
	unsigned long counti = 0;

	float theta = 0.0f;
	float phi = 0.0f;
	float radius = 1;

	float PI = 3.1416;

	float verticalAngularStride = (float)PI / (float)stacks;
	float horizontalAngularStride = ((float)PI * 2) / (float)slices;

	for (int verticalIt = 0; verticalIt < numVerticesPerColumn; verticalIt++)
	{
		// beginning on top of the sphere:
		theta = ((float)PI / 2.0f) - verticalAngularStride * verticalIt;

		for (int horizontalIt = 0; horizontalIt < numVerticesPerRow; horizontalIt++)
		{
			phi = horizontalAngularStride * horizontalIt;

			// position
			float x = radius * (float)cos(theta) * (float)cos(phi);
			float y = radius * (float)cos(theta) * (float)sin(phi);
			float z = radius * (float)sin(theta);

			vertices[countv].position = D3DXVECTOR3(x, y, z);
			D3DXVec3Normalize(&vertices[countv].normal, &vertices[countv].position);
			//vertices[countv].texture = D3DXVECTOR2((1 - verticalIt / slices), (stacks * horizontalIt) / stacks);
			//vertices[countv].texture = D3DXVECTOR2(asin(vertices[countv].normal.x) / PI + 0.5, asin(vertices[countv].normal.y) / PI + 0.5);
			vertices[countv].texture = D3DXVECTOR2(asin(vertices[countv].normal.y) / PI + 0.5, asin(vertices[countv].normal.x) / PI + 0.5);
			vertices[countv].normal = D3DXVECTOR3(1 - x, 1 - y, 1 - z);
			countv++;
		}
	}

	for (int verticalIt = 0; verticalIt < stacks; verticalIt++)
	{
		for (int horizontalIt = 0; horizontalIt < slices; horizontalIt++)
		{
			short lt = (short)(horizontalIt + verticalIt * (numVerticesPerRow));
			short rt = (short)((horizontalIt + 1) + verticalIt * (numVerticesPerRow));

			short lb = (short)(horizontalIt + (verticalIt + 1) * (numVerticesPerRow));
			short rb = (short)((horizontalIt + 1) + (verticalIt + 1) * (numVerticesPerRow));

			indices[counti++] = lt;
			indices[counti++] = rt;
			indices[counti++] = lb;

			indices[counti++] = rt;
			indices[counti++] = rb;
			indices[counti++] = lb;
		}
	}

	//Set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch=0;
	vertexData.SysMemSlicePitch = 0;

	//Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc,&vertexData,&m_vertexBuffer);
	if(FAILED(result))
		return false;

	//Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch =0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
		return false;

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete []vertices;
	vertices = 0;

	delete []indices;
	indices = 0;

	return true;

}

void ModelClass::ShutdownBuffers(){

	//Release the index buffer
	if(m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//Release the vertex buffer
	if(m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;	
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext){

	unsigned int stride;
	unsigned int offset;

	//Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset=0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0,1,&m_vertexBuffer,&stride,&offset);

	//Set the index buffer to active in th input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer,DXGI_FORMAT_R32_UINT,0);

	//Set the tupe of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename){

	bool result;

	//Create the texture object
	m_Texture = new TextureClass;
	if(!m_Texture)
		return false;

	//Initialize the texture object
	result = m_Texture->Initialize(device,filename);
	if(!result)
		return false;

	return true;
}

bool ModelClass::LoadTexture2(ID3D11Device* device, WCHAR* filename){

	bool result;

	m_Texture2 = new TextureClass;
	if (!m_Texture2)
		return false;

	result = m_Texture2->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

bool ModelClass::LoadTexture3(ID3D11Device* device, WCHAR* filename){

	bool result;

	m_Texture3 = new TextureClass;
	if (!m_Texture3)
		return false;

	result = m_Texture3->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

bool ModelClass::LoadTexture4(ID3D11Device* device, WCHAR* filename){

	bool result;

	m_Texture4 = new TextureClass;
	if (!m_Texture4)
		return false;

	result = m_Texture4->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

bool ModelClass::LoadTexture5(ID3D11Device* device, WCHAR* filename){

	bool result;

	m_Texture5 = new TextureClass;
	if (!m_Texture5)
		return false;

	result = m_Texture5->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

bool ModelClass::LoadTexture6(ID3D11Device* device, WCHAR* filename){

	bool result;

	m_Texture6 = new TextureClass;
	if (!m_Texture6)
		return false;

	result = m_Texture6->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

bool ModelClass::LoadTexture7(ID3D11Device* device, WCHAR* filename){

	bool result;

	m_Texture7 = new TextureClass;
	if (!m_Texture7)
		return false;

	result = m_Texture7->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

bool ModelClass::LoadModel(char* filename){
	ifstream fin;
	char input;
	int i;

	//Open the model file
	fin.open(filename);

	//If it couldn't open then exit
	if(fin.fail())
		return false;

	//Read up to the value of vertex count
	fin.get(input);
	while(input != ':')
		fin.get(input);

	//Read in the vertex count
	fin>>m_vertexCount;

	//Set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	//create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
		return false;

	//Read up the beginning of the data
	fin.get(input);
	while(input != ':')
		fin.get(input);
	fin.get(input);
	fin.get(input);

	//read in the vertex data
	for(i=0;i<m_vertexCount;i++){

		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

		
	}

	//Close the model file
	fin.close();

	return true;

}

void ModelClass::ReleaseTexture(){

	if(m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	if (m_Texture2){
		m_Texture2->Shutdown();
		delete m_Texture2;
		m_Texture2 = 0;
	}

	if (m_Texture3){
		m_Texture3->Shutdown();
		delete m_Texture3;
		m_Texture3 = 0;
	}

	if (m_Texture4){
		m_Texture4->Shutdown();
		delete m_Texture4;
		m_Texture4 = 0;
	}

	if (m_Texture5){
		m_Texture5->Shutdown();
		delete m_Texture5;
		m_Texture5 = 0;
	}

	if (m_Texture6){
		m_Texture6->Shutdown();
		delete m_Texture6;
		m_Texture6 = 0;
	}

	if (m_Texture7){
		m_Texture7->Shutdown();
		delete m_Texture7;
		m_Texture7 = 0;
	}

	return;
}

void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}
