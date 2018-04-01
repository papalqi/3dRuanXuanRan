//#include "image.h"
//
//
//
//image::image()
//{
//}
//
//int image::make_texture_by_png(const char * name, bool mipmap)
//{
//	
//	IUINT32 *data = NULL;
//	texture *texture = &texture[texture_count];
//	char trueName[100];
//	char *findPos = NULL;
//	if ((findPos = strrchr(name, '.')) != NULL) {
//		long len = findPos - name;
//		strncpy(trueName, name, len);
//		trueName[len] = '\0';
//	}
//	int res = load_png_image(findPos == NULL ? name : trueName, &data, &texture->width, &texture->height);
//	if (res == 0) {
//		texture->datas = (IUINT32**)malloc(1 * sizeof(IUINT32*));
//		texture->datas[0] = data;
//		if (mipmap) {
//			texture->use_mipmap = true;
//			generate_mipmaps(texture, 1.01);
//		}
//		texture_count++;
//		return texture_count - 1;
//	}
//	return -1;
//}
//
//int image::load_png_image(const char * name, unsigned int ** bits, unsigned int * width, unsigned int * height)
//{
//	
//		FILE *fp;
//		png_structp png_ptr;
//		png_infop info_ptr;
//		png_bytep* row_pointers;
//		char buf[PNG_BYTES_TO_CHECK];
//		int w, h, x, y, temp, color_type;
//
//		fp = fopen(getFilePath(name, "png"), "rb");
//		if (fp == NULL) {
//			return 1; /* ����ֵ */
//		}
//
//		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
//		info_ptr = png_create_info_struct(png_ptr);
//
//		setjmp(png_jmpbuf(png_ptr));
//		/* ��ȡPNG_BYTES_TO_CHECK���ֽڵ����� */
//		temp = (int)fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
//		/* �����������ݲ�û��PNG_BYTES_TO_CHECK���ֽ� */
//		if (temp < PNG_BYTES_TO_CHECK) {
//			fclose(fp);
//			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//			return 2;/* ����ֵ */
//		}
//		/* ��������Ƿ�ΪPNG��ǩ�� */
//		temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
//		/* �������PNG��ǩ������˵�����ļ�����PNG�ļ� */
//		if (temp != 0) {
//			fclose(fp);
//			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//			return 3;/* ����ֵ */
//		}
//
//		/* ��λ�ļ�ָ�� */
//		rewind(fp);
//		/* ��ʼ���ļ� */
//		png_init_io(png_ptr, fp);
//		/* ��ȡPNGͼƬ��Ϣ */
//		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
//		/* ��ȡͼ���ɫ������ */
//		color_type = png_get_color_type(png_ptr, info_ptr);
//		/* ��ȡͼ��Ŀ�� */
//		w = png_get_image_width(png_ptr, info_ptr);
//		h = png_get_image_height(png_ptr, info_ptr);
//
//		*bits = (unsigned int*)malloc(sizeof(unsigned int) * w * h);
//
//		/* ��ȡͼ����������������ݣ�row_pointers��߾���rgba���� */
//		row_pointers = png_get_rows(png_ptr, info_ptr);
//		/* ���ݲ�ͬ��ɫ�����ͽ�����Ӧ���� */
//		switch (color_type) {
//		case PNG_COLOR_TYPE_RGB_ALPHA:
//			for (y = 0; y<h; ++y) {
//				for (x = 0; x < w; ++x) {
//					(*bits)[y*w + x] = 0;
//					/* ������RGBA���ݣ���Ҫ�Լ�������룬����RGBA���� */
//					(*bits)[y*w + x] |= row_pointers[y][4 * x + 0] << 16; // red
//					(*bits)[y*w + x] |= row_pointers[y][4 * x + 1] << 8; // green
//					(*bits)[y*w + x] |= row_pointers[y][4 * x + 2]; // blue
//					(*bits)[y*w + x] |= row_pointers[y][4 * x + 3] << 24; // alpha
//				}
//			}
//			break;
//
//		case PNG_COLOR_TYPE_RGB:
//			for (y = 0; y<h; ++y) {
//				for (x = 0; x<w; ++x) {
//					(*bits)[y*w + x] = 0xff000000;
//					(*bits)[y*w + x] |= row_pointers[y][3 * x + 0] << 16; // red
//					(*bits)[y*w + x] |= row_pointers[y][3 * x + 1] << 8; // green
//					(*bits)[y*w + x] |= row_pointers[y][3 * x + 2]; // blue
//				}
//			}
//			break;
//			/* ����ɫ�����͵�ͼ��Ͳ����� */
//		default:
//			fclose(fp);
//			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//			return 4/* ����ֵ */;
//		}
//		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//
//		*width = w;
//		*height = h;
//
//		return 0;
//	
//
//
//
//
//}
//
//
//
