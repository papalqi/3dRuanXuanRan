#include "device.h"

void device::device_init(int width, int height, void* fb)
{
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char* ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int j;
	assert(ptr);
	this->framebuffer = (unsigned int**)ptr;
	this->zbuffer = (float**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	this->texture = (unsigned int**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;
	if (fb != NULL) framebuf = (char*)fb;
	for (j = 0; j < height; j++)
	{
		this->framebuffer[j] = (unsigned int*)(framebuf + width * 4 * j);
		this->zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}
	this->texture[0] = (unsigned int*)ptr;
	this->texture[1] = (unsigned int*)(ptr + 16);
	memset(this->texture[0], 0, 64);
	this->tex_width = 2;
	this->tex_height = 2;
	this->max_u = 1.0f;
	this->max_v = 1.0f;
	this->width = width;
	this->height = height;
	this->background = 0xc0c0c0;
	this->foreground = 0;
	transforms.transform_init(width, height);
	this->render_state = RENDER_STATE_WIREFRAME;
}

void device::device_update()
{
	transforms.transform_update();
}

void device::device_destroy()
{
	if (this->framebuffer)
		free(this->framebuffer);
	this->framebuffer = NULL;
	this->zbuffer = NULL;
	this->texture = NULL;
}

void device::device_set_texture(void* bits, long pitch, int w, int h)
{
	char* ptr = (char*)bits;
	int j;
	assert(w <= 1024 && h <= 1024);
	for (j = 0; j < h; ptr += pitch, j++) // 重新计算每行纹理的指针
		this->texture[j] = (unsigned int*)ptr;
	this->tex_width = w;
	this->tex_height = h;
	this->max_u = (float)(w - 1);
	this->max_v = (float)(h - 1);
}

void device::device_clear(int mode)
{
	int y, x, height = this->height;
	for (y = 0; y < this->height; y++)
	{
		unsigned int* dst = this->framebuffer[y];
		unsigned int cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = this->background;
		for (x = this->width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < this->height; y++)
	{
		float* dst = this->zbuffer[y];
		for (x = this->width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}

void device::device_pixel(int x, int y, unsigned int color)
{
	if (((unsigned int)x) < (unsigned int)this->width && ((unsigned int)y) < (unsigned int)this->height)
	{
		this->framebuffer[y][x] = color;
	}
}

void device::device_draw_line(int x1, int y1, int x2, int y2, unsigned int c)
{
	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2)
	{
		device_pixel(x1, y1, c);
	}
	else if (x1 == x2)
	{
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) device_pixel(x1, y, c);
		device_pixel(x2, y2, c);
	}
	else if (y1 == y2)
	{
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc) device_pixel(x, y1, c);
		device_pixel(x2, y2, c);
	}
	else
	{
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy)
		{
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++)
			{
				device_pixel(x, y, c);
				rem += dy;
				if (rem >= dx)
				{
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					device_pixel(x, y, c);
				}
			}
			device_pixel(x2, y2, c);
		}
		else
		{
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++)
			{
				device_pixel(x, y, c);
				rem += dx;
				if (rem >= dy)
				{
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					device_pixel(x, y, c);
				}
			}
			device_pixel(x2, y2, c);
		}
	}
}

unsigned int device::device_texture_read(float u, float v)
{
	int x, y;
	u = u * this->max_u;
	v = v * this->max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = CMID(x, 0, this->tex_width - 1);
	y = CMID(y, 0, this->tex_height - 1);
	return this->texture[y][x];
	return 0;
}

void device::device_draw_scanline(scanline* scanline)
{
	unsigned int* framebuffer = this->framebuffer[scanline->y];
	float* zbuffer = this->zbuffer[scanline->y];
	int x = scanline->x;
	int w = scanline->w;
	int width = this->width;
	int render_state = this->render_state;
	for (; w > 0; x++, w--)
	{
		if (x >= 0 && x < width)
		{
			float rhw = scanline->v.rhw;
			if (rhw >= zbuffer[x])
			{
				float w = 1.0f / rhw;
				zbuffer[x] = rhw;
				if (render_state & RENDER_STATE_COLOR)
				{
					float r = scanline->v.color.r * w;
					float g = scanline->v.color.g * w;
					float b = scanline->v.color.b * w;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);
					framebuffer[x] = (R << 16) | (G << 8) | (B);
				}
				if (render_state & RENDER_STATE_TEXTURE)
				{
					float u = scanline->v.tc.u * w;
					float v = scanline->v.tc.v * w;
					unsigned int cc = device_texture_read(u, v);
					framebuffer[x] = cc;
				}
			}
		}
		scanline->v.vertex_add(&scanline->step);

		if (x >= width) break;
	}
}

void device::device_render_trap(trapezoid* trap)
{
	scanline scanlines;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (j = top; j < bottom; j++)
	{
		if (j >= 0 && j < this->height)
		{
			trap->trapezoid_edge_interp((float)j + 0.5f);
			scanlines.trapezoid_init_scan_line(trap, j);
			device_draw_scanline(&scanlines);
		}
		if (j >= this->height) break;
	}
}

//绘制
void device::device_draw_primitive(const vertex* v1, const vertex* v2, const vertex* v3)
{
	vector p1, p2, p3, c1, c2, c3;
	int render_state = this->render_state;

	//通过坐标变换进行变换到投影坐标上
	this->transforms.transform_apply(&c1, &v1->pos);
	this->transforms.transform_apply(&c2, &v2->pos);
	this->transforms.transform_apply(&c3, &v3->pos);
	//返回如果为0，则说明不在范围内，直接退出，也就是说，如果有一个点不再，则都不绘制
	// 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
	// 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
	if (transforms.transform_check_cvv(&c1) != 0) return;
	if (transforms.transform_check_cvv(&c2) != 0) return;
	if (transforms.transform_check_cvv(&c3) != 0) return;

	// 
	transforms.transform_homogenize(&p1, &c1);
	transforms.transform_homogenize(&p2, &c2);
	transforms.transform_homogenize(&p3, &c3);
	// 背面剔除
	vector t21, t32;
	vectorSub(&t21, &v2->pos, &v1->pos);
	vectorSub(&t32, &v3->pos, &v2->pos);
	if (t21.x * t32.y - t32.x * t21.y > 0) // 计算叉积
		return;


	// 纹理或者色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR))
	{
		vertex t1 = *v1, t2 = *v2, t3 = *v3;
		trapezoid traps[2];
		int n;

		t1.pos = p1;
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;

		t1.vertex_rhw_init(); // 初始化 w
		t2.vertex_rhw_init(); // 初始化 w
		t3.vertex_rhw_init(); // 初始化 w

		// 拆分三角形为0-2个梯形，并且返回可用梯形数量
		n = trapezoid_init_triangle(traps, &t1, &t2, &t3);

		if (n >= 1) device_render_trap(&traps[0]);
		if (n >= 2) device_render_trap(&traps[1]);
	}

	if (render_state & RENDER_STATE_WIREFRAME)
	{
		// 线框绘制
		device_draw_line((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, this->foreground);
		device_draw_line((int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, this->foreground);
		device_draw_line((int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, this->foreground);
	}
}
