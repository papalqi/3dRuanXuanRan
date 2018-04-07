#pragma once
#include "math.h"

class camera
{
public:
	point pos;//λ��
	point front;//��ǰ����
	point worldup;//z������
	matrix view;//�Ӿ�ת������
	matrix projection;//ͶӰ����
	matrix view_matrix_r;//�Ӿ������
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
	//ŷ����ʵ����ת
	void camera_init_by_euler( float yaw, float pitch);
	void camera_init_projection( );
	void camera_update( );

	void Gofront(float);
	void GoRight(float);
	void Goleft(float);
	void Goback(float);
};
