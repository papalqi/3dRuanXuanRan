#pragma once
#include"geometry.h"
class objects
{
public:
	vertex *mesh;
	unsigned long mesh_num;
	int* material_ids;
	int texture_id;
	bool shadow;
	bool dirty;
	point pos;
	vector scale;
	vector axis;
	float theta;
	matrix matrixs;
	objects();
	~objects();
};

class box :objects
{
public:
	box()
	{
		this->pos = point (0, 2, -1, 1) ;
		this->scale = vector( 0.5, 0.5, 0.5, 0 );
		this->axis = vector(1, 0, 1, 1 );
		this->theta = 0.0f;
		//this->mesh = box_mesh;
		this->mesh_num = 36;
		this->material_ids = NULL;
		this->texture_id = 0;
		this->shadow = false;
		this->dirty = true;
	}

};

