#pragma once

#include"math.h"
class transform {
public:

	matrix world;         // 世界坐标变换
	matrix view;          // 摄影机坐标变换
	matrix projection;    // 投影变换
	matrix transforms;     // transform = world * view * projection
	float w, h;             // 屏幕大小

							// 矩阵更新，计算 transform = world * view * projection
	void transform_update();
	// 初始化，设置屏幕长宽
	void transform_init(int width, int height);
	// 将矢量 x 进行 project 
	void transform_apply(vector *y, const vector *x) {
		matrix_apply(y, x, &this->transforms);
	}
	// 检查齐次坐标同 cvv 的边界用于视锥裁剪
	int transform_check_cvv(const vector *v);
	// 归一化，得到屏幕坐标
	void transform_homogenize(vector *y, const vector *x);
};