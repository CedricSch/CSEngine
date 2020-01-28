#pragma once
#include "glad_wgl.h"


struct GLContext
{
	HINSTANCE mAppInstance;
	HGLRC mRenderContext;
	HDC mDeviceContext;
	HWND mWindowHandle;
	LPCWSTR mClassName;
	int mCmdShow;
	int mWidth;
	int mHeight;
	WNDPROC mWindowProc;
};

ATOM registerWindowClass(const GLContext&);
bool createOpenGLContext(GLContext&);
bool destroyOpenGLContext(const GLContext&);

