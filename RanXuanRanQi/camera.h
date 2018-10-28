#pragma once
#include "math.h"

class camera
{
public:
	point pos; //位置
	point front; //面前方向
	point worldup; //z正方形
	matrix view; //视觉转换矩阵
	matrix projection; //投影矩阵
	matrix view_matrix_r; //视觉逆矩阵
	int width;
	int height;
	float fovy;
	float zn;
	float zf;

	float left;
	float right;
	float bottom;
	float top;
	float aspect;

public:
	camera();
	void camera_init(int width, int height);

	//	void camera_init();
	~camera();

	//欧拉角实现旋转
	void camera_init_by_euler(float yaw, float pitch);
	void camera_init_projection();
	void camera_update();

	void Gofront(float);
	void GoRight(float);
	void Goleft(float);
	void Goback(float);
};
