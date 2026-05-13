//libpng
#ifndef __LIBPNG__
#define __LIBPNG__

#ifdef __cplusplus
	extern "C" {
#endif
/////////////////////////////////////////////////////////////////////////////
extern int initpng(void);
extern int BmptoPng(char * bmp_file_name,int width,int height,int m_color,char * png_file_name);
extern int writePng( char* png_file_name, unsigned char*  pixels , int width, int height, int bit_depth);

#ifdef __cplusplus
}
#endif 

#endif  


