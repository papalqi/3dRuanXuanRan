#include "transform.h"

void transform::transform_update()
{
	matrix m;
	matrix_mul(&m, &this->world, &this->view);
	matrix_mul(&this->transforms, &m, &this->projection);
	matrix_mul(&this->mv, &this->world, &this->view);
	matrix_mul(&this->vp, &this->view, &this->projection);
	matrix_mul(&this->mvp, &this->mv, &this->projection);
}

void transform::transform_init(int width, int height)
{
	float aspect = (float)width / ((float)height);
	matrix_set_identity(&this->world);
	matrix_set_identity(&this->view);
	matrix_set_perspective(&this->projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	this->w = (float)width;
	this->h = (float)height;
	transform_update();
}

int transform::transform_check_cvv(const vector* v)
{
	float w = v->w;
	int check = 0;
	if (v->z < 0.0f) check |= 1;
	if (v->z > w) check |= 2;
	if (v->x < -w) check |= 4;
	if (v->x > w) check |= 8;
	if (v->y < -w) check |= 16;
	if (v->y > w) check |= 32;
	return check;
}

void transform::transform_homogenize(vector* y, const vector* x)
{
	float rhw = 1.0f / x->w;
	y->x = (x->x * rhw + 1.0f) * this->w * 0.5f;
	y->y = (1.0f - x->y * rhw) * this->h * 0.5f;
	y->z = x->z * rhw;
	y->w = 1.0f;
}
