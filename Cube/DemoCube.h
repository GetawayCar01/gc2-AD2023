// DemoCube.h : Defines the CDemoCube class.
//
//
//

#ifndef _DEMOCUBE_H_
#define _DEMOCUBE_H_

#include "Dx11Base.h"


class CDemoCube : public CDx11Base
{
// Constructors
public:
    CDemoCube();
    virtual ~CDemoCube();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Members
protected:
    ID3D11VertexShader* m_pVS;
    ID3D11PixelShader* m_pPS;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;
    ID3D11Buffer* m_pIndexBuffer;
    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;

    // Buffers world - projection - view
    ID3D11Buffer* m_pWorldCB;
    ID3D11Buffer* m_pProjCB;
    ID3D11Buffer* m_pViewCB;
    //Matrices de transformacion
    XMMATRIX m_viewMatrix;
    XMMATRIX m_projMatrix;
};


#endif // _DEMOCUBE_H_
