///////////////////////////
//Filename: modelclass.h
///////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

/////////////
//INCLUDES///
/////////////
#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;
/////////////////////
//MY CLASS INCLUDES//
/////////////////////
#include "textureclass.h"

/////////////////////
// Class name: ModelClass
/////////////////////
class ModelClass {

private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType{

		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();
	ID3D11ShaderResourceView* GetTexture4();
	ID3D11ShaderResourceView* GetTexture5();
	ID3D11ShaderResourceView* GetTexture6();
	ID3D11ShaderResourceView* GetTexture7();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	bool LoadTexture2(ID3D11Device*, WCHAR*);
	bool LoadTexture3(ID3D11Device*, WCHAR*);
	bool LoadTexture4(ID3D11Device*, WCHAR*);
	bool LoadTexture5(ID3D11Device*, WCHAR*);
	bool LoadTexture6(ID3D11Device*, WCHAR*);
	bool LoadTexture7(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	TextureClass* m_Texture2;
	TextureClass* m_Texture3;
	TextureClass* m_Texture4;
	TextureClass* m_Texture5;
	TextureClass* m_Texture6;
	TextureClass* m_Texture7;
	ModelType* m_model;
};

#endif