#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include"transform.h"
#include"geometry.h"
#include"math.h"
#define RENDER_STATE_WIREFRAME      1		// ��Ⱦ�߿�
#define RENDER_STATE_TEXTURE        2		// ��Ⱦ����
#define RENDER_STATE_COLOR          4		// ��Ⱦ��ɫ
#define IUINT32 
class device {
public:

	transform transforms;      // ����任��
	int width;                  // ���ڿ��
	int height;                 // ���ڸ߶�
	unsigned int **framebuffer;      // ���ػ��棺framebuffer[y] ����� y��
	float **zbuffer;            // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��
	unsigned int **texture;          // ����ͬ����ÿ������
	int tex_width;              // ������
	int tex_height;             // ����߶�
	float max_u;                // ��������ȣ�tex_width - 1
	float max_v;                // �������߶ȣ�tex_height - 1
	int render_state;           // ��Ⱦ״̬
	unsigned int background;         // ������ɫ
	unsigned int foreground;         // �߿���ɫ



	void device_init(int width, int height, void *fb);
	void device_destroy();
	void device_set_texture(void *bits, long pitch, int w, int h);
	void device_clear(int mode);
	void device_pixel(int x, int y, unsigned int color);
	void device_draw_line(int x1, int y1, int x2, int y2, unsigned int c);
	unsigned int device_texture_read(float u, float v);
	void device_draw_scanline(scanline *scanline);
	void device_render_trap(trapezoid *trap);
	void device_draw_primitive(const vertex *v1,
		const vertex *v2, const vertex *v3);

};
