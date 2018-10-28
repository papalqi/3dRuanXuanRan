#pragma once
#include <cmath>
#define PI 3.141592653
#define angle_to_radian(X) ((X)/180*PI)
#define radian_to_angle(X) ((X)/PI*180)

//定义四维矩阵
struct matrix
{
	float m[4][4];
};

//定义四维向量
struct vector
{
	float x, y, z, w;
};

static int CMID(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }

// 计算插值：t 为 [0, 1] 之间的数值
static float interp(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }

// 计算向量长度
static float vector_length(const vector* v)
{
	float sq = v->x * v->x + v->y * v->y + v->z * v->z;
	return (float)sqrt(sq);
}

static void vector_scale(vector* v, float k)
{
	v->x *= k;
	v->y *= k;
	v->z *= k;
	v->w *= k;
}

// z = x + y
static void vectorAdd(vector* z, const vector* x, const vector* y)
{
	z->x = x->x + y->x;
	z->y = x->y + y->y;
	z->z = x->z + y->z;
	z->w = 1.0;
}

// z = x - y
static void vectorSub(vector* z, const vector* x, const vector* y)
{
	z->x = x->x - y->x;
	z->y = x->y - y->y;
	z->z = x->z - y->z;
	z->w = 1.0;
}

// 矢量点乘
static float vector_dotproduct(const vector* x, const vector* y)
{
	return x->x * y->x + x->y * y->y + x->z * y->z;
}

// 矢量叉乘
static void vector_crossproduct(vector* z, const vector* x, const vector* y)
{
	float m1, m2, m3;
	m1 = x->y * y->z - x->z * y->y;
	m2 = x->z * y->x - x->x * y->z;
	m3 = x->x * y->y - x->y * y->x;
	z->x = m1;
	z->y = m2;
	z->z = m3;
	z->w = 1.0f;
}

// 矢量插值，t取值 [0, 1]
static void vector_interp(vector* z, const vector* x1, const vector* x2, float t)
{
	z->x = interp(x1->x, x2->x, t);
	z->y = interp(x1->y, x2->y, t);
	z->z = interp(x1->z, x2->z, t);
	z->w = 1.0f;
}

// 矢量归一化
static void vector_normalize(vector* v)
{
	float length = vector_length(v);
	if (length != 0.0f)
	{
		float inv = 1.0f / length;
		v->x *= inv;
		v->y *= inv;
		v->z *= inv;
	}
}

// c = a + b
static void matrix_add(matrix* c, const matrix* a, const matrix* b)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] + b->m[i][j];
	}
}

// c = a - b
static void matrix_sub(matrix* c, const matrix* a, const matrix* b)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] - b->m[i][j];
	}
}

// c = a * b
static void matrix_mul(matrix* c, const matrix* a, const matrix* b)
{
	matrix z;
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
				(a->m[j][1] * b->m[1][i]) +
				(a->m[j][2] * b->m[2][i]) +
				(a->m[j][3] * b->m[3][i]);
		}
	}
	c[0] = z;
}

// c = a * f
static void matrix_scale(matrix* c, const matrix* a, float f)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] * f;
	}
}

// y = x * m
static void matrix_apply(vector* y, const vector* x, const matrix* m)
{
	float X = x->x, Y = x->y, Z = x->z, W = x->w;
	y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
	y->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
	y->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
	y->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
}

static void matrix_set_identity(matrix* m)
{
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}

static void matrix_set_zero(matrix* m)
{
	m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
}

// 平移变换
static void matrix_set_translate(matrix* m, float x, float y, float z)
{
	matrix_set_identity(m);
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}

// 缩放变换
static void matrix_set_scale(matrix* m, float x, float y, float z)
{
	matrix_set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}

// 旋转矩阵
static void matrix_set_rotate(matrix* m, float x, float y, float z, float theta)
{
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	vector vec = { x, y, z, 1.0f };
	float w = qcos;
	vector_normalize(&vec);
	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m->m[1][0] = 2 * x * y - 2 * w * z;
	m->m[2][0] = 2 * x * z + 2 * w * y;
	m->m[0][1] = 2 * x * y + 2 * w * z;
	m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m->m[2][1] = 2 * y * z - 2 * w * x;
	m->m[0][2] = 2 * x * z - 2 * w * y;
	m->m[1][2] = 2 * y * z + 2 * w * x;
	m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
	m->m[3][3] = 1.0f;
}

// 设置摄像机
static void matrix_set_lookat(matrix* m, const vector* eye, const vector* at, const vector* up)
{
	vector xaxis, yaxis, zaxis;

	vectorSub(&zaxis, at, eye);
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);
	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);

	m->m[0][0] = xaxis.x;
	m->m[1][0] = xaxis.y;
	m->m[2][0] = xaxis.z;
	m->m[3][0] = -vector_dotproduct(&xaxis, eye);

	m->m[0][1] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[2][1] = yaxis.z;
	m->m[3][1] = -vector_dotproduct(&yaxis, eye);

	m->m[0][2] = zaxis.x;
	m->m[1][2] = zaxis.y;
	m->m[2][2] = zaxis.z;
	m->m[3][2] = -vector_dotproduct(&zaxis, eye);

	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}

// D3DXMatrixPerspectiveFovLH
static void matrix_set_perspective(matrix* m, float fovy, float aspect, float zn, float zf)
{
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	matrix_set_zero(m);
	m->m[0][0] = (float)(fax / aspect);
	m->m[1][1] = (float)(fax);
	m->m[2][2] = zf / (zf - zn);
	m->m[3][2] = -zn * zf / (zf - zn);
	m->m[2][3] = 1;
}

typedef vector point;

//设置旋转轴
static void matrix_set_axis(matrix* m, const vector* xaxis, const vector* yaxis, const vector* zaxis, const point* pos)
{
	m->m[0][0] = xaxis->x;
	m->m[0][1] = xaxis->y;
	m->m[0][2] = xaxis->z;
	m->m[1][0] = yaxis->x;
	m->m[1][1] = yaxis->y;
	m->m[1][2] = yaxis->z;
	m->m[2][0] = zaxis->x;
	m->m[2][1] = zaxis->y;
	m->m[2][2] = zaxis->z;
	m->m[3][0] = pos->x;
	m->m[3][1] = pos->y;
	m->m[3][2] = pos->z;
	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}

//TODO::所有的矩阵计算进行封装，调用Eigen库