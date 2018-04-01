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
//			return 1; /* 返回值 */
//		}
//
//		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
//		info_ptr = png_create_info_struct(png_ptr);
//
//		setjmp(png_jmpbuf(png_ptr));
//		/* 读取PNG_BYTES_TO_CHECK个字节的数据 */
//		temp = (int)fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
//		/* 若读到的数据并没有PNG_BYTES_TO_CHECK个字节 */
//		if (temp < PNG_BYTES_TO_CHECK) {
//			fclose(fp);
//			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//			return 2;/* 返回值 */
//		}
//		/* 检测数据是否为PNG的签名 */
//		temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
//		/* 如果不是PNG的签名，则说明该文件不是PNG文件 */
//		if (temp != 0) {
//			fclose(fp);
//			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//			return 3;/* 返回值 */
//		}
//
//		/* 复位文件指针 */
//		rewind(fp);
//		/* 开始读文件 */
//		png_init_io(png_ptr, fp);
//		/* 读取PNG图片信息 */
//		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
//		/* 获取图像的色彩类型 */
//		color_type = png_get_color_type(png_ptr, info_ptr);
//		/* 获取图像的宽高 */
//		w = png_get_image_width(png_ptr, info_ptr);
//		h = png_get_image_height(png_ptr, info_ptr);
//
//		*bits = (unsigned int*)malloc(sizeof(unsigned int) * w * h);
//
//		/* 获取图像的所有行像素数据，row_pointers里边就是rgba数据 */
//		row_pointers = png_get_rows(png_ptr, info_ptr);
//		/* 根据不同的色彩类型进行相应处理 */
//		switch (color_type) {
//		case PNG_COLOR_TYPE_RGB_ALPHA:
//			for (y = 0; y<h; ++y) {
//				for (x = 0; x < w; ++x) {
//					(*bits)[y*w + x] = 0;
//					/* 以下是RGBA数据，需要自己补充代码，保存RGBA数据 */
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
//			/* 其它色彩类型的图像就不读了 */
//		default:
//			fclose(fp);
//			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
//			return 4/* 返回值 */;
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
