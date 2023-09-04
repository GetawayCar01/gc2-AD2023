// Dx11Base.cpp : Implementando la clase CDx11Base.
//
// 
//

#include "Dx11Base.h"


CDx11Base::CDx11Base()
{
    m_hWnd = NULL;
    m_hInst = NULL;
    m_pD3DDevice = NULL;
    m_pD3DContext = NULL;
    m_pD3DRenderTargetView = NULL;
    m_pSwapChain = NULL;
}

CDx11Base::~CDx11Base()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides

bool CDx11Base::Initialize(HWND hWnd, HINSTANCE hInst)
{
    // Establecer atributos
    m_hWnd = hWnd;
    m_hInst = hInst;

    // Obtener el tamaño de la ventana
    RECT rc;
    ::GetClientRect(hWnd, &rc);
    UINT nWidth = rc.right - rc.left;
    UINT nHeight = rc.bottom - rc.top;

    // Estructura de la cadena de intercambio
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ::ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = nWidth;
    swapChainDesc.BufferDesc.Height = nHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = (HWND)hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = true;

    // Niveles de funciones admitidos
    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // Niveles de controlador admitidos
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // Banderas
    UINT flags = 0;
#ifdef _DEBUG
    flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Cree el dispositivo D3D y la cadena de intercambio
    HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
        NULL, 
        D3D_DRIVER_TYPE_HARDWARE, 
        NULL, 
        flags, 
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_pSwapChain, 
        &m_pD3DDevice, 
        &featureLevel,
        &m_pD3DContext
        );

    // Comprobar dispositivo
    /*
    if (FAILED(hr))	{
        ::MessageBox(hWnd, TEXT("A DX11 Video Card is Required"), TEXT("ERROR"), MB_OK);
        return false;
    }
    */

    // Obtener el búfer trasero de la cadena de intercambio
    ID3D11Texture2D *pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) {
        ::MessageBox(hWnd, TEXT("Unable to get back buffer"), TEXT("ERROR"), MB_OK);
        return false;
    }

    // Crear la vista de destino de renderizado
    hr = m_pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pD3DRenderTargetView);

    // Liberar el búfer trasero
    if (pBackBuffer != NULL)
        pBackBuffer->Release();

    // Verificar la vista de destino de renderizado
    if (FAILED(hr)) {
        ::MessageBox(hWnd, TEXT("Unable to create render target view"), TEXT("ERROR"), MB_OK);
        return false;
    }

    // Establecer el destino de procesamiento
    m_pD3DContext->OMSetRenderTargets(1, &m_pD3DRenderTargetView, NULL);

    // Establecer la ventana gráfica
    D3D11_VIEWPORT viewPort;
    viewPort.Width = (float)nWidth;
    viewPort.Height = (float)nHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    m_pD3DContext->RSSetViewports(1, &viewPort);

    // Cargar contenido
    return LoadContent();
}

void CDx11Base::Terminate()
{
    // Descargar contenido
    UnloadContent();

    // Limpiar
    if (m_pD3DRenderTargetView != NULL)
        m_pD3DRenderTargetView->Release();
    m_pD3DRenderTargetView= NULL;
    if (m_pSwapChain != NULL)
        m_pSwapChain->Release();
    m_pSwapChain= NULL;
    if (m_pD3DContext != NULL)
        m_pD3DContext->Release();
    m_pD3DContext= NULL;
    if (m_pD3DDevice != NULL)
        m_pD3DDevice->Release();
    m_pD3DDevice= NULL;
}

bool CDx11Base::CompileShader(LPCWSTR szFilePath, LPCSTR szFunc, LPCSTR szShaderModel, ID3DBlob** buffer)
{
    // Establecer banderas
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif
    
    // Compilar sombreador
    HRESULT hr;
    ID3DBlob* errBuffer = 0;
    hr = ::D3DX11CompileFromFile(
        szFilePath, 0, 0, szFunc, szShaderModel,
        flags, 0, 0, buffer, &errBuffer, 0);

    //Compruebe si hay errores
    if (FAILED(hr)) {
        if (errBuffer != NULL) {
            ::OutputDebugStringA((char*)errBuffer->GetBufferPointer());
            errBuffer->Release();
        }
        return false;
    }
    
    // Limpiar
    if (errBuffer != NULL)
        errBuffer->Release( );
    return true;
}
