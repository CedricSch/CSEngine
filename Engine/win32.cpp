//#ifndef UNICODE
//#define UNICDOE
//#endif

#include "gl_context.h"
#include <windowsx.h>
#include <string>

static GLContext gContext{};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
	case WM_CREATE: {
		// Create OpenGL Context
		OutputDebugStringA("CREATE\n");
	} break;

	case WM_PAINT: {
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(gContext.mDeviceContext);
	} break;

	case WM_LBUTTONDOWN: {
		/*
			x Coords = lparam lower 16bit
			y Coords = lparam upper 16bit
			On 64 Bit machines: upper 32 bits are unused
		*/
		//int xCoord = GET_X_LPARAM(lParam);
		//int xCoord2 = lParam & 0xFFFF;

		//int yCoord = GET_Y_LPARAM(lParam);
		//int yCoord2 = (lParam >> 16) & 0xFFFF;
		////int yCoord = lParam & (16 << 16);

		//std::string s("" + xCoord);
		//OutputDebugStringA(s.c_str());
	} break;

	case WM_SIZE: {
		OutputDebugStringA("SIZE\n");
	} break;

	case WM_CLOSE: {
		// Destroy OpenGL Context
		OutputDebugStringA("CLOSE\n");
	} break;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow) {

	gContext.mAppInstance = hInstance;
	gContext.mClassName = L"CS_ENGINE_CLASS";
	gContext.mCmdShow = SW_NORMAL;
	gContext.mWidth = 680;
	gContext.mHeight = 680;
	gContext.mWindowProc = WindowProc;

	bool result = registerWindowClass(gContext);
	result = createOpenGLContext(gContext);

	ShowWindow(gContext.mWindowHandle, SW_NORMAL);

	MSG msg = {};

	// Message Pump
	while (1) {
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
