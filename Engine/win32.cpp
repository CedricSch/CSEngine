//#ifndef UNICODE
//#define UNICDOE
//#endif

//#include <windows.h>
//#include "glad.h"
#include "glad_wgl.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static void setupOpenGL();


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow) {

	// Datenstruktur welche intern beim OS benutzt wird
	const wchar_t ENGINE_CLASS_NAME[] = L"CSENGINE";
	static HINSTANCE APP_INSTANCE = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	// Handle zur Applikations Instance
	wc.hInstance = APP_INSTANCE;
	// Name für die Window Klasse
	wc.lpszClassName = ENGINE_CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClass(&wc);

	HWND tempWindow = CreateWindow(ENGINE_CLASS_NAME, L"FakeWnd", WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 1, 1, NULL, NULL, APP_INSTANCE, NULL);

	if (tempWindow == NULL) {
		return 0;
	}

	HDC deviceContext = GetDC(tempWindow);

	PIXELFORMATDESCRIPTOR tempPixelFormat = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // FLags
		PFD_TYPE_RGBA, // type of framebuffer
		32, // number of bits for color
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, // number of bits for depthbuffer
		8, // number of bits for stencil buffer
		0, // number of Aux buffers in the framebuffer
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	// Get an PixelFormatID if wantedPixelFormat matches something we can use
	int tempPixelFormatID = ChoosePixelFormat(deviceContext, &tempPixelFormat);

	if (!tempPixelFormatID) {
		OutputDebugStringA("ChoosePixelFormat failed");
		return 1;
	}


	if (!SetPixelFormat(deviceContext, tempPixelFormatID, &tempPixelFormat)) {
		OutputDebugStringA("SetPixelFormat failed");
		return 1;
	}

	HGLRC fakeRC = wglCreateContext(deviceContext);
	if (!fakeRC) {
		OutputDebugStringA("wglCreateContext failed");
		return 1;
	}

	if (!wglMakeCurrent(deviceContext, fakeRC)) {
		OutputDebugStringA("wglMakeCurrent failed");
		return 1;
	}

	if (!gladLoadWGL(deviceContext)) {
		OutputDebugStringA("gladLoadWGL failed");
	}

	if (!gladLoadGL()) {
		OutputDebugStringA("gladLoadGL failed");
	}


	HWND realWindow = CreateWindow(ENGINE_CLASS_NAME, L"OpenGL", WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, APP_INSTANCE, NULL);
	HDC realDC = GetDC(realWindow);

	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0 // End
	};

	int pixelFormatID; UINT numFormats;

	bool status = wglChoosePixelFormatARB(realDC, attribList, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0) {
		OutputDebugStringA("wglChoosePixelFormatARB() failed.");
		return 1;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(realDC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(realDC, pixelFormatID, &PFD);


	HGLRC RC = wglCreateContext(realDC);
	if (RC == NULL) {
		OutputDebugStringA("wglCreateContextAttribsARB() failed.");
		return 1;
	}

	const int major_min = 3, minor_min = 3;
	int  contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	RC = wglCreateContextAttribsARB(realDC, 0, contextAttribs);
	if (RC == NULL) {
		OutputDebugStringA("wglCreateContextAttribsARB() failed.");
		return 1;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(tempWindow, deviceContext);
	DestroyWindow(tempWindow);
	if (!wglMakeCurrent(realDC, RC)) {
		OutputDebugStringA("wglMakeCurrent() failed.");
		return 1;
	}

	SetWindowTextA(realWindow, (LPCSTR)glGetString(GL_VERSION));
	ShowWindow(realWindow, SW_MAXIMIZE);

	glClearColor(0.129f, 0.586f, 0.949f, 1.0f); // rgb(33,150,243)
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(realDC);


	// Message Loop

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void setupOpenGL() {

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CREATE: {

		} break;
		case WM_SIZE: {
			OutputDebugStringA("SIZE\n");
		} break;

		case WM_CLOSE: {
			//PostQuitMessage(0);
		} break;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);;
};