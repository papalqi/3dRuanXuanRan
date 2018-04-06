#pragma once

typedef unsigned int IUINT32;

class texture
{
public:
	IUINT32** datas; // data
	IUINT32 datas_len;
	bool use_mipmap; // able mipmap
	IUINT32 width;
	IUINT32 height;
	texture();
	~texture();
};
