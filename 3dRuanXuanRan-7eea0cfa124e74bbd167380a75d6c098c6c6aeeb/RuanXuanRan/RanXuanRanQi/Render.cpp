#include "Render.h"

vertex mesh[8] = {
	{{1, -1, 1, 1}, {0, 0}, {1.0f, 0.2f, 0.2f}, 1},
	{{-1, -1, 1, 1}, {0, 1}, {0.2f, 1.0f, 0.2f}, 1},
	{{-1, 1, 1, 1}, {1, 1}, {0.2f, 0.2f, 1.0f}, 1},
	{{1, 1, 1, 1}, {1, 0}, {1.0f, 0.2f, 1.0f}, 1},
	{{1, -1, -1, 1}, {0, 0}, {1.0f, 1.0f, 0.2f}, 1},
	{{-1, -1, -1, 1}, {0, 1}, {0.2f, 1.0f, 1.0f}, 1},
	{{-1, 1, -1, 1}, {1, 1}, {1.0f, 0.3f, 0.3f}, 1},
	{{1, 1, -1, 1}, {1, 0}, {0.2f, 1.0f, 0.3f}, 1},
};
static HWND screen_handle = NULL; // 主窗口 HWND
static HDC screen_dc = NULL; // 配套的 HDC
static HBITMAP screen_hb = NULL; // DIB
static HBITMAP screen_ob = NULL; // 老的 BITMAP
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512]; // 当前键盘按下状态
unsigned char* screen_fb = NULL; // frame buffer
long screen_pitch = 0;

static LRESULT screen_events(HWND hWnd, UINT msg,
                             WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE: screen_exit = 1;
		break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1;
		break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0;
		break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Render::Render_init(point s, int w, int h, const TCHAR* title)
{
	this->screen_init(800, 600, title);

	{
		mDevice.device_init(800, 600, screen_fb);
		Camera.camera_init(w, h);
		init_texture();
		mDevice.render_state = RENDER_STATE_TEXTURE;
	}
}

void Render::set_camera()
{
	Camera.camera_update();
	mDevice.transforms.view = Camera.view;
	mDevice.transforms.view_r = Camera.view_matrix_r;
	mDevice.transforms.projection = Camera.projection;
}

int Render::screen_init(int w, int h, const TCHAR* title)
{
	WNDCLASS wc = {
		CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN3.1415926")
	};
	BITMAPINFO bi = {
		{
			sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
			w * h * 4, 0, 0, 0, 0
		}
	};
	RECT rect = {0, 0, w, h};
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screen_handle = CreateWindow(_T("SCREEN3.1415926"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, w * h * 4);

	return 0;
}

int Render::screen_close(void)
{
	if (screen_dc)
	{
		if (screen_ob)
		{
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb)
	{
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle)
	{
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

void Render::renderUpdate(point pos)
{
	this->screen_update();
	this->screen_dispatch();
	this->mDevice.device_clear(1);
	set_camera();
}

void Render::draw_plane(int a, int b, int c, int d)
{
	{
		vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
		p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
		p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
		mDevice.device_draw_primitive(&p1, &p2, &p3);
		mDevice.device_draw_primitive(&p3, &p4, &p1);
	}
}

void Render::draw_box(float theta)
{
	{
		matrix m;
		matrix_set_rotate(&m, -1, -0.5, 1, theta);
		mDevice.transforms.world = m;
		mDevice.transforms.transform_update();
		draw_plane(0, 1, 2, 3);
		draw_plane(4, 5, 6, 7);
		draw_plane(0, 4, 5, 1);
		draw_plane(1, 5, 6, 2);
		draw_plane(2, 6, 7, 3);
		draw_plane(3, 7, 4, 0);
	}
}

void Render::camera_at_zero(point s)
{
	{
		vector eye = {s.x, s.y, s.z, 1}, at = {0, 0, 0, 1}, up = {0, 0, 1, 1};
		matrix_set_lookat(&mDevice.transforms.view, &eye, &at, &up);
		mDevice.transforms.transform_update();
	}
}

void Render::setState()
{
	++nowState;
	nowState = nowState / 3;
	this->mDevice.render_state = states[nowState];
}

void Render::init_texture()
{
	{
		static unsigned int texture[256][256];
		int i, j;
		for (j = 0; j < 256; j++)
		{
			for (i = 0; i < 256; i++)
			{
				int x = i / 32, y = j / 32;
				texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
			}
		}
		mDevice.device_set_texture(texture, 256 * 4, 256, 256);
	}
}

void Render::screen_update(void)
{
	{
		HDC hDC = GetDC(screen_handle);
		BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
		ReleaseDC(screen_handle, hDC);
		screen_dispatch();
	}
}

void Render::screen_dispatch(void)
{
	MSG msg;
	while (1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}
