//#pragma once
//#include<png.h>
//#include <stdio.h>
//#include <string>
//#include"texture.h"
//#define PNG_BYTES_TO_CHECK 4
//const char* getFilePath(const char* name, const char* type) {
//	static char path[100];
//	if (strcmp(type, "png") == 0)
//		sprintf(path, "../../../resources/image/%s.%s", name, type);
//	else if (strcmp(type, "obj") == 0)
//		sprintf(path, "../../../resources/obj/%s.%s", name, type);
//	return path;
//}
//class image
//{
//public:
//	image();
//	int generate_mipmaps(texture *texture, float gamma)
//	{
//		IUINT32 **mipmaps = NULL;
//		int num_mip_levels = logbase2ofx(texture->width) + 1;
//		texture->datas_len = num_mip_levels;
//		mipmaps = (IUINT32**)malloc(num_mip_levels * sizeof(IUINT32*));
//		mipmaps[0] = texture->datas[0];
//		int mip_width = texture->width;
//		int mip_height = texture->height;
//		for (int mip_level = 1; mip_level < num_mip_levels; mip_level++) {
//			mip_width = mip_width >> 1;
//			mip_height = mip_height >> 1;
//			mipmaps[mip_level] = (IUINT32*)malloc(mip_width * mip_height * sizeof(IUINT32));
//			IUINT32 *src_buffer = mipmaps[mip_level - 1];
//			IUINT32 *dest_buffer = mipmaps[mip_level];
//			for (int x = 0; x < mip_width; x++)
//			{
//				for (int y = 0; y < mip_height; y++)
//				{
//					float r0, g0, b0, a0,
//						r1, g1, b1, a1,
//						r2, g2, b2, a2,
//						r3, g3, b3, a3;
//					int r_avg, g_avg, b_avg, a_avg;
//
//					IUINT32 c = src_buffer[(x * 2 + 0) + (y * 2 + 0)*mip_width * 2];
//					b0 = c & 0xff;
//					g0 = (c >> 8) & 0xff;
//					r0 = (c >> 16) & 0xff;
//					a0 = (c >> 24) & 0xff;
//
//					c = src_buffer[(x * 2 + 1) + (y * 2 + 0)*mip_width * 2];
//					b1 = c & 0xff;
//					g1 = (c >> 8) & 0xff;
//					r1 = (c >> 16) & 0xff;
//					a1 = (c >> 24) & 0xff;
//
//					c = src_buffer[(x * 2 + 0) + (y * 2 + 1)*mip_width * 2];
//					b2 = c & 0xff;
//					g2 = (c >> 8) & 0xff;
//					r2 = (c >> 16) & 0xff;
//					a2 = (c >> 24) & 0xff;
//
//					c = src_buffer[(x * 2 + 1) + (y * 2 + 1)*mip_width * 2];
//					b3 = c & 0xff;
//					g3 = (c >> 8) & 0xff;
//					r3 = (c >> 16) & 0xff;
//					a3 = (c >> 24) & 0xff;
//
//					r_avg = (IUINT32)(0.5f + gamma*(r0 + r1 + r2 + r3) / 4);
//					g_avg = (IUINT32)(0.5f + gamma*(g0 + g1 + g2 + g3) / 4);
//					b_avg = (IUINT32)(0.5f + gamma*(b0 + b1 + b2 + b3) / 4);
//					a_avg = (IUINT32)(0.5f + gamma*(b0 + b1 + b2 + b3) / 4);
//
//					int R = CMID(r_avg, 0, 255);
//					int G = CMID(g_avg, 0, 255);
//					int B = CMID(b_avg, 0, 255);
//					int A = CMID(a_avg, 0, 255);
//
//					dest_buffer[x + y*mip_width] = (A << 24) | (R << 16) | (G << 8) | B;
//				}
//			}
//		}
//		free(texture->datas);
//		texture->datas = mipmaps;
//		return num_mip_levels;
//	}
//	int make_texture_by_png(const char *name, bool mipmap);
//	int load_png_image(const char *name, unsigned int **bits, unsigned int *width, unsigned int *height);
//};