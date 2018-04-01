#pragma once

#include<Windows.h>
#include"device.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif


class Render
{
public:
	void Render_init();

	int screen_init(int w, int h, const TCHAR *title);// ÆÁÄ»³õÊ¼»¯
	int screen_close(void);								// ¹Ø±ÕÆÁÄ»
	void draw_plane(int a, int b, int c, int d);
	void draw_box(float theta);
	void camera_at_zero(float x, float y, float z);

	void init_texture();
	void clear();
	device mDevice;
	void screen_update(void);
	void screen_dispatch(void);



};
