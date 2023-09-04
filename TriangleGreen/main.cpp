// main.cpp : Define el punto de entrada para la aplicación..
//
//
//

#include "windows.h"
#include "DemoTriangleGreen.h"

// Variable Globales
HINSTANCE g_hInst;	// instancia actual
HWND g_hWnd;		// handle dela ventana principal

// Declaraciones de funciones (incluidas en este archivo C ++)
ATOM				MyRegisterClass(HINSTANCE);
bool				CreateMainWnd(int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


// Función de punto de entrada principal
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Inicializamos
    g_hInst = hInstance;
    g_hWnd = NULL;
    MyRegisterClass(hInstance);

    // Crear ventana principal
    if (!CreateMainWnd(nCmdShow))
        return -1;

    // Inicializamos el demo
    CDemoTriangleGreen demo;
    if (!demo.Initialize(g_hWnd, g_hInst))
        return -1;

    // Bucle de mensaje principal
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        // Actualizar y renderizar
        demo.Update();
        demo.Render();
    }

    // Terminamos el demo
    demo.Terminate();

    return (int)msg.wParam;
}


// Registra la clase de ventana
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= L"TriangleGreenClass";
    wcex.hIconSm		= NULL;

    return RegisterClassEx(&wcex);
}


// Crea la ventana principal
bool CreateMainWnd(int nCmdShow)
{
    // Calcular el tamaño de la ventana principal
    RECT rc = { 0, 0, 1200, 720 };
    ::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // Crea la ventana principal
    g_hWnd = ::CreateWindow(L"TriangleGreenClass", L"Creando un Triangulo", 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        rc.right - rc.left, rc.bottom - rc.top, 
        NULL, NULL, g_hInst, NULL);
    
    // Compruebe la manija de la ventana
    if (g_hWnd == NULL)
        return false;

    ::ShowWindow(g_hWnd, nCmdShow);
    ::UpdateWindow(g_hWnd);

    return true;
}


// Procesa mensajes para la ventana principal
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = ::BeginPaint(hWnd, &ps);
        ::EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
