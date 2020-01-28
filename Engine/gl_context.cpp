#include "OpenGLContext.h"


ATOM registerWindowClass(const GLContext& context) {
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = context.mWindowProc;
	wc.hInstance = context.mAppInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = context.mClassName;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	return RegisterClassEx(&wc);
}

bool createOpenGLContext(GLContext& context) {
	HWND tempWindow = CreateWindowEx(NULL, context.mClassName, L"TemporaryWindow", WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, context.mWidth, context.mHeight, NULL, NULL, context.mAppInstance, NULL);

	if (tempWindow == NULL) {
		return false;
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
		return false;
	}


	if (!SetPixelFormat(deviceContext, tempPixelFormatID, &tempPixelFormat)) {
		OutputDebugStringA("SetPixelFormat failed");
		return false;
	}

	HGLRC fakeRC = wglCreateContext(deviceContext);
	if (!fakeRC) {
		OutputDebugStringA("wglCreateContext failed");
		return false;
	}

	if (!wglMakeCurrent(deviceContext, fakeRC)) {
		OutputDebugStringA("wglMakeCurrent failed");
		return false;
	}

	if (!gladLoadWGL(deviceContext)) {
		OutputDebugStringA("gladLoadWGL failed");
		return false;
	}

	if (!gladLoadGL()) {
		OutputDebugStringA("gladLoadGL failed");
		return false;
	}


	context.mWindowHandle = CreateWindowEx(NULL, context.mClassName, L"Engine", WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, context.mWidth, context.mHeight, NULL, NULL, context.mAppInstance, NULL);
	context.mDeviceContext = GetDC(context.mWindowHandle);

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

	bool status = wglChoosePixelFormatARB(context.mDeviceContext, attribList, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0) {
		OutputDebugStringA("wglChoosePixelFormatARB() failed.");
		return false;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(context.mDeviceContext, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(context.mDeviceContext, pixelFormatID, &PFD);


	auto fakeContext = wglCreateContext(context.mDeviceContext);
	if (fakeContext == NULL) {
		OutputDebugStringA("wglCreateContextAttribsARB() failed.");
		return false;
	}

	const int major_min = 3, minor_min = 3;
	int  contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	context.mRenderContext = wglCreateContextAttribsARB(context.mDeviceContext, 0, contextAttribs);
	if (context.mRenderContext == NULL) {
		OutputDebugStringA("wglCreateContextAttribsARB() failed.");
		return false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(tempWindow, deviceContext);
	DestroyWindow(tempWindow);

	if (!wglMakeCurrent(context.mDeviceContext, context.mRenderContext)) {
		OutputDebugStringA("wglMakeCurrent() failed.");
		return false;
	}
	else {
		glClearColor(0.8, 0.8, 0.8, 1.0);
	}

	return true;
}

bool destroyOpenGLContext(const GLContext& context) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(context.mRenderContext);
	ReleaseDC(context.mWindowHandle, context.mDeviceContext);
	return DestroyWindow(context.mWindowHandle);
}
