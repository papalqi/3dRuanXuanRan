#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include"transform.h"
#include"geometry.h"
#include"math.h"
#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色
#define IUINT32

class device
{
public:

	transform transforms; // 坐标变换器
	int width; // 窗口宽度
	int height; // 窗口高度
	unsigned int** framebuffer; // 像素缓存：framebuffer[y] 代表第 y行
	float** zbuffer; // 深度缓存：zbuffer[y] 为第 y行指针
	unsigned int** texture; // 纹理：同样是每行索引
	int tex_width; // 纹理宽度
	int tex_height; // 纹理高度
	float max_u; // 纹理最大宽度：tex_width - 1
	float max_v; // 纹理最大高度：tex_height - 1
	int render_state; // 渲染状态
	unsigned int background; // 背景颜色
	unsigned int foreground; // 线框颜色

	void device_init(int width, int height, void* fb);
	void device_destroy();
	void device_set_texture(void* bits, long pitch, int w, int h);
	void device_clear(int mode);
	void device_pixel(int x, int y, unsigned int color);
	void device_draw_line(int x1, int y1, int x2, int y2, unsigned int c);
	unsigned int device_texture_read(float u, float v);
	void device_draw_scanline(scanline* scanline);
	void device_render_trap(trapezoid* trap);
	void device_draw_primitive(const vertex* v1,
		const vertex* v2, const vertex* v3);
};
