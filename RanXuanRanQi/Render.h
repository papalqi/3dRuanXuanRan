#pragma once

#include<Windows.h>
#include"device.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "camera.h"
#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#endif

class Render
{
public:
	void Render_init(point, int w, int h, const TCHAR* title);
	void set_camera();
	camera Camera;
	int screen_init(int w, int h, const TCHAR* title); // 屏幕初始化
	int screen_close(void); // 关闭屏幕
	void draw_plane(int a, int b, int c, int d);
	void draw_box(float theta);
	void camera_at_zero(point s);
	void setState();
	void init_texture();
	void renderUpdate(point);
	device mDevice;
	void screen_update(void);
	void screen_dispatch(void);
private:
	int states[3] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	int nowState = 0;
};
