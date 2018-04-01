#pragma once

#include "geometry.h"


class pointlight {
public:
	point pos;
	float constant;
	float linear;
	float quadratic;
	color ambi;
	color diff;
	color spec;
	bool shadow;
} ;

