#pragma once

#include"math.h"
class transform {
public:

	matrix world;         // ��������任
	matrix view;          // ��Ӱ������任
	matrix projection;    // ͶӰ�任
	matrix transforms;     // transform = world * view * projection
	float w, h;             // ��Ļ��С

							// ������£����� transform = world * view * projection
	void transform_update();
	// ��ʼ����������Ļ����
	void transform_init(int width, int height);
	// ��ʸ�� x ���� project 
	void transform_apply(vector *y, const vector *x) {
		matrix_apply(y, x, &this->transforms);
	}
	// ����������ͬ cvv �ı߽�������׶�ü�
	int transform_check_cvv(const vector *v);
	// ��һ�����õ���Ļ����
	void transform_homogenize(vector *y, const vector *x);
};