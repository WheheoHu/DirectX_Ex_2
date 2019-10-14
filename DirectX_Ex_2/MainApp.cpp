#include "MyD3D.h"

MyD3D _device;

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!_device.CreateDevice(hinstance, 480, 480))
	{
		MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!_device.Initialize())
	{
		MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			_device.FrameMove(timeDelta);
			_device.Render();

			lastTime = currTime;
		}
	}

	_device.Release();

	return 0;
}


int main() {
	WinMain(NULL, NULL, NULL, 1);
	return 0;
}