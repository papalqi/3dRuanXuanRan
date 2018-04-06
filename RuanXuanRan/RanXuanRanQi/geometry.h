#pragma once

#include"math.h"

//定点颜色
class color
{
public:
	float r, g, b;
};

//？？
class texcoord
{
public:
	float u, v;
};

//顶点
class vertex
{
public:
	vector pos;
	texcoord tc;
	color color;
	float rhw;

	void vertex_rhw_init()
	{
		float rhw = 1.0f / this->pos.w;
		this->rhw = rhw;
		this->tc.u *= rhw;
		this->tc.v *= rhw;
		this->color.r *= rhw;
		this->color.g *= rhw;
		this->color.b *= rhw;
	}

	void vertex_interp(const vertex* x1, const vertex* x2, float t)
	{
		vector_interp(&this->pos, &x1->pos, &x2->pos, t);
		this->tc.u = interp(x1->tc.u, x2->tc.u, t);
		this->tc.v = interp(x1->tc.v, x2->tc.v, t);
		this->color.r = interp(x1->color.r, x2->color.r, t);
		this->color.g = interp(x1->color.g, x2->color.g, t);
		this->color.b = interp(x1->color.b, x2->color.b, t);
		this->rhw = interp(x1->rhw, x2->rhw, t);
	}

	void vertex_division(const vertex* x1, const vertex* x2, float w)
	{
		float inv = 1.0f / w;
		this->pos.x = (x2->pos.x - x1->pos.x) * inv;
		this->pos.y = (x2->pos.y - x1->pos.y) * inv;
		this->pos.z = (x2->pos.z - x1->pos.z) * inv;
		this->pos.w = (x2->pos.w - x1->pos.w) * inv;
		this->tc.u = (x2->tc.u - x1->tc.u) * inv;
		this->tc.v = (x2->tc.v - x1->tc.v) * inv;
		this->color.r = (x2->color.r - x1->color.r) * inv;
		this->color.g = (x2->color.g - x1->color.g) * inv;
		this->color.b = (x2->color.b - x1->color.b) * inv;
		this->rhw = (x2->rhw - x1->rhw) * inv;
	}

	void vertex_add(const vertex* x)
	{
		this->pos.x += x->pos.x;
		this->pos.y += x->pos.y;
		this->pos.z += x->pos.z;
		this->pos.w += x->pos.w;
		this->rhw += x->rhw;
		this->tc.u += x->tc.u;
		this->tc.v += x->tc.v;
		this->color.r += x->color.r;
		this->color.g += x->color.g;
		this->color.b += x->color.b;
	}
};


//边
class edge
{
public:
	vertex v, v1, v2;
};


//四边形
class trapezoid
{
public:

	float top, bottom;
	edge left, right;


	void trapezoid_edge_interp(float y)
	{
		float s1 = this->left.v2.pos.y - this->left.v1.pos.y;
		float s2 = this->right.v2.pos.y - this->right.v1.pos.y;
		float t1 = (y - this->left.v1.pos.y) / s1;
		float t2 = (y - this->right.v1.pos.y) / s2;
		this->left.v.vertex_interp(&this->left.v1, &this->left.v2, t1);
		this->right.v.vertex_interp(&this->right.v1, &this->right.v2, t2);
	}
};

//扫描线
class scanline
{
public:
	vertex v, step;
	int x, y, w;

	void trapezoid_init_scan_line(const trapezoid* trap, int y)
	{
		float width = trap->right.v.pos.x - trap->left.v.pos.x;
		this->x = (int)(trap->left.v.pos.x + 0.5f);
		this->w = (int)(trap->right.v.pos.x + 0.5f) - this->x;
		this->y = y;
		this->v = trap->left.v;
		if (trap->left.v.pos.x >= trap->right.v.pos.x) this->w = 0;
		this->step.vertex_division(&trap->left.v, &trap->right.v, width);
	}
};

static int trapezoid_init_triangle(trapezoid* trap, const vertex* p1,
                                   const vertex* p2, const vertex* p3)
{
	const vertex* p;
	float k, x;

	if (p1->pos.y > p2->pos.y) p = p1, p1 = p2, p2 = p;
	if (p1->pos.y > p3->pos.y) p = p1, p1 = p3, p3 = p;
	if (p2->pos.y > p3->pos.y) p = p2, p2 = p3, p3 = p;
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

	if (p1->pos.y == p2->pos.y)
	{
		// triangle down
		if (p1->pos.x > p2->pos.x) p = p1, p1 = p2, p2 = p;
		trap[0].top = p1->pos.y;
		trap[0].bottom = p3->pos.y;
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p3;
		trap[0].right.v1 = *p2;
		trap[0].right.v2 = *p3;
		return (trap[0].top < trap[0].bottom) ? 1 : 0;
	}

	if (p2->pos.y == p3->pos.y)
	{
		// triangle up
		if (p2->pos.x > p3->pos.x) p = p2, p2 = p3, p3 = p;
		trap[0].top = p1->pos.y;
		trap[0].bottom = p3->pos.y;
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;
		return (trap[0].top < trap[0].bottom) ? 1 : 0;
	}

	trap[0].top = p1->pos.y;
	trap[0].bottom = p2->pos.y;
	trap[1].top = p2->pos.y;
	trap[1].bottom = p3->pos.y;

	k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
	x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

	if (x <= p3->pos.x)
	{
		// triangle left
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;
		trap[1].left.v1 = *p2;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p1;
		trap[1].right.v2 = *p3;
	}
	else
	{
		// triangle right
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p3;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p2;
		trap[1].left.v1 = *p1;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p2;
		trap[1].right.v2 = *p3;
	}

	return 2;
}
