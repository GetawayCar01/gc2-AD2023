// DemoCube.cpp : Implements the CDemoCube class.
//
//
//

#include "DemoCube.h"

// Vertex struct
struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};

//Variables de transformación
float rotationx;
float rotationz;

float traslationx;
float traslationy;

bool positivex;
bool positivey;


//////////////////////////////////////////////////////////////////////
// Constructors

CDemoCube::CDemoCube()
{
    m_pVS = NULL;
    m_pPS = NULL;
    m_pInputLayout = NULL;
    m_pVertexBuffer = NULL;
    m_pIndexBuffer = NULL;
    m_pColorMap = NULL;
    m_pColorMapSampler = NULL;
    m_pViewCB = NULL;
    m_pProjCB = NULL;
    m_pWorldCB = NULL;
}

CDemoCube::~CDemoCube()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides

bool CDemoCube::LoadContent()
{
    // Compile vertex shader
    ID3DBlob* pVSBuffer = NULL;
    bool res = CompileShader(L"Recursos/VertexShader.hlsl", "VS_Main", "vs_4_0", &pVSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load vertex shader", L"ERROR", MB_OK);
        return false;
    }

    // Create vertex shader
    HRESULT hr;
    hr = m_pD3DDevice->CreateVertexShader(
        pVSBuffer->GetBufferPointer(),
        pVSBuffer->GetBufferSize(),
        0, &m_pVS);
    if (FAILED(hr)) {
        if (pVSBuffer)
            pVSBuffer->Release();
        return false;
    }

    // Define input layout
    D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

    // Create input layout
    hr = m_pD3DDevice->CreateInputLayout(
        shaderInputLayout, numLayoutElements,
        pVSBuffer->GetBufferPointer(), 
        pVSBuffer->GetBufferSize(), 
        &m_pInputLayout);
    if (FAILED(hr)) {
        return false;
    }

    // Release VS buffer
    pVSBuffer->Release();
    pVSBuffer = NULL;

    // Compile pixel shader
    ID3DBlob* pPSBuffer = NULL;
    res = CompileShader(L"Recursos/PixelShader.hlsl", "PS_Main", "ps_4_0", &pPSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load pixel shader", L"ERROR", MB_OK);
        return false;
    }

    // Create pixel shader
    hr = m_pD3DDevice->CreatePixelShader(
        pPSBuffer->GetBufferPointer(),
        pPSBuffer->GetBufferSize(), 
        0, &m_pPS);
    if (FAILED(hr)) {
        return false;
    }

    // Cleanup PS buffer
    pPSBuffer->Release();
    pPSBuffer = NULL;

    // Define triangles
    Vertex vertices[] =
    {
        { XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
    };

    // Vertex description
    D3D11_BUFFER_DESC vertexDesc;
    ::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(Vertex) * 24;

    // Resource data
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));
    resourceData.pSysMem = vertices;

    // Create vertex buffer
    hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Index buffer data
    WORD indices[] =
    {
        3,   1,  0,  2,  1,  3,
        6,   4,  5,  7,  4,  6,
        11,  9,  8, 10,  9, 11,
        14, 12, 13, 15, 12, 14,
        19, 17, 16, 18, 17, 19,
        22, 20, 21, 23, 20, 22
    };

    // Create index buffer
    D3D11_BUFFER_DESC indexBufferDesc;
    ::ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof( WORD ) * 36;
    indexBufferDesc.CPUAccessFlags = 0;
    resourceData.pSysMem = indices;
    hr = m_pD3DDevice->CreateBuffer(&indexBufferDesc, &resourceData, &m_pIndexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Load texture
    hr = ::D3DX11CreateShaderResourceViewFromFile(
        m_pD3DDevice, L"Recursos/uv_checker.jpg", 0, 0, &m_pColorMap, 0);

    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to load texture", L"ERROR", MB_OK);
        return false;
    }

    // Texture sampler
    D3D11_SAMPLER_DESC textureDesc;
    ::ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    textureDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    textureDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = m_pD3DDevice->CreateSamplerState(&textureDesc, &m_pColorMapSampler);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to create texture sampler state", L"ERROR", MB_OK);
        return false;
    }

    // Create constant buffers (see hlsl-file)
    D3D11_BUFFER_DESC constBufferDesc;
    ::ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
    constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constBufferDesc.ByteWidth = sizeof(XMMATRIX);
    constBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = m_pD3DDevice->CreateBuffer(&constBufferDesc, 0, &m_pViewCB);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Error al Crear Buffer View", L"ERROR", MB_OK);
        return false;
    }
    hr = m_pD3DDevice->CreateBuffer(&constBufferDesc, 0, &m_pProjCB);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Error al Crear Buffer Projection", L"ERROR", MB_OK);
        return false;
    }
    hr = m_pD3DDevice->CreateBuffer(&constBufferDesc, 0, &m_pWorldCB);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Error al Crear Buffer World", L"ERROR", MB_OK);
        return false;
    }

    // Initialize matrixes
    m_viewMatrix = XMMatrixIdentity();
    m_projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 640.0f / 480.0f, 0.01f, 100.0f);
    XMMATRIX t = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
    m_viewMatrix = XMMatrixTranspose(m_viewMatrix * t);
    m_projMatrix = XMMatrixTranspose(m_projMatrix);

    //Set Transformations
    rotationx = 0.0;
    rotationz = 0.0;
    traslationx = 0.0;
    traslationy = 0.0;
    positivex = true;
    positivey = true;

    return true;
}

void CDemoCube::UnloadContent()
{
    // Cleanup
    if (m_pColorMap)
        m_pColorMap->Release();
    m_pColorMap = NULL;
    if (m_pColorMapSampler)
        m_pColorMapSampler->Release();
    m_pColorMapSampler = NULL;
    if (m_pVS)
        m_pVS->Release();
    m_pVS = NULL;
    if (m_pPS)
        m_pPS->Release();
    m_pPS = NULL;
    if (m_pInputLayout)
        m_pInputLayout->Release();
    m_pInputLayout = NULL;
    if (m_pVertexBuffer)
        m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
    if (m_pIndexBuffer)
        m_pIndexBuffer->Release();
    m_pIndexBuffer = NULL;

    if (m_pViewCB)
        m_pViewCB->Release();
    m_pViewCB = NULL;
    if (m_pProjCB)
        m_pProjCB->Release();
    m_pProjCB = NULL;
    if (m_pWorldCB)
        m_pWorldCB->Release();
    m_pWorldCB = NULL;
}

void CDemoCube::Update()
{
}

void CDemoCube::Render()
{

    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);
    m_pD3DContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Set vertex buffer
    m_pD3DContext->IASetInputLayout(m_pInputLayout);
    m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pD3DContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set shaders
    m_pD3DContext->VSSetShader(m_pVS, 0, 0);
    m_pD3DContext->PSSetShader(m_pPS, 0, 0);
    m_pD3DContext->PSSetShaderResources(0, 1, &m_pColorMap);
    m_pD3DContext->PSSetSamplers(0, 1, &m_pColorMapSampler);

    // Position cube in the world
    XMMATRIX r = XMMatrixRotationRollPitchYaw(rotationx, 0.0f, rotationz);
    XMMATRIX t = XMMatrixTranslation(traslationx, traslationy, 5.0f);
    XMMATRIX s = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    XMMATRIX w = XMMatrixTranspose(r * t * s);

    // Update constant buffers
    m_pD3DContext->UpdateSubresource(m_pWorldCB, 0, 0, &w, 0, 0);
    m_pD3DContext->UpdateSubresource(m_pViewCB, 0, 0, &m_viewMatrix, 0, 0);
    m_pD3DContext->UpdateSubresource(m_pProjCB, 0, 0, &m_projMatrix, 0, 0);

    // Upload constant buffers to GPU
    m_pD3DContext->VSSetConstantBuffers(0, 1, &m_pWorldCB);
    m_pD3DContext->VSSetConstantBuffers(1, 1, &m_pViewCB);
    m_pD3DContext->VSSetConstantBuffers(2, 1, &m_pProjCB);

    // Draw triangles
    m_pD3DContext->DrawIndexed(36, 0, 0);

    // Present back buffer to display
    m_pSwapChain->Present(0, 0);

    //Transformations
    rotationx += 0.0005;
    rotationz += 0.0005;

    if (positivex) {
        traslationx += .006;
        if (traslationx >= 6.5) {
            positivex = false;
        }
    }
    else {
        traslationx -= .006;
        if (traslationx <= -6.5) {
            positivex = true;
        }
    }

    if (positivey) {
        traslationy += .008;
        if (traslationy >= 4.5) {
            positivey = false;
        }
    }
    else {
        traslationy -= .008;
        if (traslationy <= -4.5) {
            positivey = true;
        }
    }
}
