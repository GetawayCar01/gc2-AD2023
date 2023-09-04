// DemoTriangleGreen.h :Defieniedo la clase CDemoTriangleGreen.
//
// 
//

#ifndef _DEMOTRIANGLEGREEN_H_
#define _DEMOTRIANGLEGREEN_H_

#include "Dx11Base.h"


class CDemoTriangleGreen : public CDx11Base
{
// Constructors
public:
    CDemoTriangleGreen();
    virtual ~CDemoTriangleGreen();

// Overrides
public:
    virtual bool LoadContent();
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

// Members
protected:
    ID3D11VertexShader* m_pVSGreenColor;
    ID3D11PixelShader* m_pPSGreenColor;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;
};


#endif // _DEMOTRIANGLEGREEN_H_
