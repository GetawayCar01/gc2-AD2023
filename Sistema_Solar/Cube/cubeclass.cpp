//////////////////////////
//Filename: Cubeclass.cpp
//////////////////////////
#include "Cubeclass.h"

CubeClass::CubeClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_Cube = 0;
}

CubeClass::CubeClass(const CubeClass& other){
}

CubeClass::~CubeClass(){
}

bool CubeClass::Initialize(ID3D11Device* device,char* CubeFileName, WCHAR* textureFilename)
{
	bool result;

	//Load the Cube data
	result = LoadCube(CubeFileName);
	if(!result)
		return false;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
		return false;

	// Load the texture for this Cube.
	result = LoadTexture(device, textureFilename);
	if(!result)
		return false;

	return true;
}

void CubeClass::Shutdown(){

	//Release the Cube texture
	ReleaseTexture();

	//Release the vertex and index buffers;
	ShutdownBuffers();

	//Release the Cube data
	ReleaseCube();
	return;
}

void CubeClass::Render(ID3D11DeviceContext* deviceContext){

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return;
}

int CubeClass::GetIndexCount(){

	return m_indexCount;
}

ID3D11ShaderResourceView* CubeClass::GetTexture(){
	return m_Texture->GetTexture();
}

bool CubeClass::InitializeBuffers(ID3D11Device* device){

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if(!indices)
		return false;

	//Load the vertex and index array with data
	for(i=0;i<m_vertexCount;i++){

		vertices[i].position = D3DXVECTOR3(m_Cube[i].x,m_Cube[i].y,m_Cube[i].z);
		vertices[i].texture = D3DXVECTOR2(m_Cube[i].tu,m_Cube[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_Cube[i].nx,m_Cube[i].ny,m_Cube[i].nz);

		indices[i] = i;
	}
	m_vertexCount = 36;
	m_indexCount = 36;
	//CARGAR CUBO
	vertices[0].position = D3DXVECTOR3(-1.0,1.0,-1.0);
	vertices[0].texture = D3DXVECTOR2(0.0,0.0);
	vertices[0].normal = D3DXVECTOR3(-1.0,0.0,-1.0);
	vertices[0].position = D3DXVECTOR3(0.0,0.0,0.0);
	vertices[0].texture = D3DXVECTOR2(1.0,0.0);
	vertices[0].normal = D3DXVECTOR3(0.0,0.0,-1.0);
	vertices[0].position = D3DXVECTOR3(0.0,0.0,0.0);
	vertices[0].texture = D3DXVECTOR2(0.0,1.0);
	vertices[0].normal = D3DXVECTOR3(0.0,0.0,-1.0);
	vertices[0].position = D3DXVECTOR3(0.0,0.0,0.0);
	vertices[0].texture = D3DXVECTOR2(0.0,1.0);
	vertices[0].normal = D3DXVECTOR3(0.0,0.0,-1.0);
	vertices[0].position = D3DXVECTOR3(0.0,0.0,0.0);
	vertices[0].texture = D3DXVECTOR2(0.0,0.0);
	vertices[0].normal = D3DXVECTOR3(0.0,0.0,-1.0);
	vertices[0].position = D3DXVECTOR3(0.0,0.0,0.0);
	vertices[0].texture = D3DXVECTOR2(0.0,0.0);
	vertices[0].normal = D3DXVECTOR3(0.0,0.0,-1.0);

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

void CubeClass::ShutdownBuffers(){

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

void CubeClass::RenderBuffers(ID3D11DeviceContext* deviceContext){

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

bool CubeClass::LoadTexture(ID3D11Device* device, WCHAR* filename){

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

bool CubeClass::LoadCube(char* filename){
	ifstream fin;
	char input;
	int i;

	//Open the Cube file
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

	//create the Cube using the vertex count that was read in
	m_Cube = new CubeType[m_vertexCount];
	if(!m_Cube)
		return false;

	//Read up the beginning of the data
	fin.get(input);
	while(input != ':')
		fin.get(input);
	fin.get(input);
	fin.get(input);

	//read in the vertex data
	for(i=0;i<m_vertexCount;i++){

		fin >> m_Cube[i].x >> m_Cube[i].y >> m_Cube[i].z;
		fin >> m_Cube[i].tu >> m_Cube[i].tv;
		fin >> m_Cube[i].nx >> m_Cube[i].ny >> m_Cube[i].nz;

		
	}

	//Close the Cube file
	fin.close();

	return true;

}

void CubeClass::ReleaseTexture(){

	if(m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void CubeClass::ReleaseCube()
{
	if(m_Cube)
	{
		delete [] m_Cube;
		m_Cube = 0;
	}

	return;
}
