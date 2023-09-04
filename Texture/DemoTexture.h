// DemoTexture.h : Defines the CDemoTexture class.
//
// 
//

#ifndef _DEMOTEXTURE_H_
#define _DEMOTEXTURE_H_

#include "Dx11Base.h"


class CDemoTexture : public CDx11Base
{
    // Constructors
public:
    CDemoTexture();
    virtual ~CDemoTexture();

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

    //Terxture and Sampler
    ID3D11ShaderResourceView* m_ColorMap1;
    ID3D11ShaderResourceView* m_ColorMap2;
    ID3D11SamplerState* m_pSampler;
};


#endif // _DEMOTEXTURE_H_