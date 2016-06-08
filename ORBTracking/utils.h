/*
  此文件中声明了图像的基本操作函数:
1、获取某位置的像素点
2、设置某位置的像素点（8位，32位和64位），
3、计算两点之间的距离的平方
4、在图片某一点画一个“X”
5、将两张图片合成为一个，高是二者之和，宽是二者的较大者。img1 在左上角，img2在右下角。
*/

#pragma once

#include "cxcore.h"

#include <stdio.h>

//求x的绝对值
#ifndef ABS
#define ABS(x) ( ( (x) < 0 )? -(x) : (x) )
#endif

//内联函数，读取和设置各种类型图像的像素值

static __inline int pixval8( IplImage* img, int r, int c )
{
	return (int)( ( (uchar*)(img->imageData + img->widthStep*r) )[c] );
}


static __inline void setpix8( IplImage* img, int r, int c, uchar val)
{
	( (uchar*)(img->imageData + img->widthStep*r) )[c] = val;
}


/*从32位浮点型单通道图像中获取指定坐标的像素值，内联函数
参数：
img：输入图像
r：行坐标
c：列坐标
返回值：坐标(c,r)处(r行c列)的像素值
*/
static __inline float pixval32f( IplImage* img, int r, int c )
{
	return ( (float*)(img->imageData + img->widthStep*r) )[c];
}


static __inline void setpix32f( IplImage* img, int r, int c, float val )
{
	( (float*)(img->imageData + img->widthStep*r) )[c] = val;
}


static __inline double pixval64f( IplImage* img, int r, int c )
{
	return (double)( ( (double*)(img->imageData + img->widthStep*r) )[c] );
}


static __inline void setpix64f( IplImage* img, int r, int c, double val )
{
	( (double*)(img->imageData + img->widthStep*r) )[c] = val;
}



//错误处理
extern void fatal_error( char* format, ... );


//获取一个文件全名，将文件名和扩展名连接到一起
extern char* replace_extension( const char* file, const char* extn );

//文件名中去掉路径c:\\traffic.jpg => traffic.jpg
extern char* basename( const char* pathname );

//显示程序进度
extern void progress( int done );

extern void erase_from_stream( FILE* stream, int n );

//数组长度加倍
extern int array_double( void** array, int n, int size );

//计算两点的对角线距离
extern double dist_sq_2D( CvPoint2D64f p1, CvPoint2D64f p2 );

//在点pt画个叉，本质就是在那个点画四条线
extern void draw_x( IplImage* img, CvPoint pt, int r, int w, CvScalar color );


/*将两张图像合成为一张，垂直排放,高是二者之和，宽是二者的较大者
参数：img1：位于上方的图像的指针，img2：位于下方的图像的指针
返回值：合成图像
*/
extern IplImage* stack_imgs( IplImage* img1, IplImage* img2 );

/*将两张图像合成为一张，水平排放
参数：img1：位于左边的图像的指针，img2：位于右边的图像的指针
返回值：合成图像
*/
extern IplImage* stack_imgs_horizontal( IplImage* img1, IplImage* img2 );

extern void vid_view( IplImage** imgs, int n, char* win_name );

//查看某个窗口是否已经关闭
extern int win_closed( char* name );
