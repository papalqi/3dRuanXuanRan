#include "camera.h"


camera::camera()
{
}

void camera::camera_init(int width, int height)
{
	this->pos = vector{0.0f, -2.0f, -2.5f, 1.0f};
	this->front = vector{0.0f, 0.0f, 1.0f, 0.0f};
	this->worldup = vector{0.0f, 1.0f, 0.0f, 0.0f};
	this->fovy = 3.1415926 * 0.5f;
	this->zn = 0.1f;
	this->zf = 500.0f;
	this->width = width;
	this->height = height;
	this->aspect = (float)width / (float)height;
	//this->projection = perspective;
	this->left = -1.0f;
	this->right = 1.0f;
	this->bottom = -1.0f;
	this->top = 1.0f;
	camera_init_projection();
	//this->dirty = true;
	//this_count++;
}


camera::~camera()
{
}

void camera::camera_init_by_euler(float yaw, float pitch)
{
	this->front.x = sin(angle_to_radian(yaw)) * cos(angle_to_radian(pitch));
	this->front.y = -sin(angle_to_radian(pitch));
	this->front.z = cos(angle_to_radian(yaw)) * cos(angle_to_radian(pitch));
	vector_normalize(&this->front);
}

void camera::Gofront(float velocity)
{
	vector temp = this->front;
	vector_scale(&temp, velocity);
	vectorAdd(&this->pos, &this->pos, &temp);
}

void camera::GoRight(float velocity)
{
	vector temp = this->front;
	vector_crossproduct(&temp, &this->front, &this->worldup);
	vector_normalize(&temp);
	vector_scale(&temp, velocity);
	vectorAdd(&this->pos, &this->pos, &temp);
}

void camera::Goleft(float velocity)
{
	vector temp = this->front;
	vector_crossproduct(&temp, &this->front, &this->worldup);
	vector_normalize(&temp);
	vector_scale(&temp, velocity);
	vectorSub(&this->pos, &this->pos, &temp);
}

void camera::Goback(float velocity)
{
	vector temp = this->front;
	vector_scale(&temp, velocity);
	vectorSub(&this->pos, &this->pos, &temp);
}


void camera::camera_init_projection()
{
	matrix_set_perspective(&this->projection, this->fovy, this->aspect, this->zn, this->zf);
}

void camera::camera_update()
{
	vector right, at, up, front = this->front;
	vector_crossproduct(&right, &this->worldup, &this->front);
	vector_normalize(&right);
	vector_crossproduct(&up, &this->front, &right);
	vector_normalize(&up);
	vectorAdd(&at, &this->pos, &this->front);
	matrix_set_lookat(&this->view, &this->pos, &at, &up);
	vector_normalize(&front);
	matrix_set_axis(&this->view_matrix_r, &right, &up, &front, &this->pos);
}
