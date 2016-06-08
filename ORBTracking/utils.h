/*
  ���ļ���������ͼ��Ļ�����������:
1����ȡĳλ�õ����ص�
2������ĳλ�õ����ص㣨8λ��32λ��64λ����
3����������֮��ľ����ƽ��
4����ͼƬĳһ�㻭һ����X��
5��������ͼƬ�ϳ�Ϊһ�������Ƕ���֮�ͣ����Ƕ��ߵĽϴ��ߡ�img1 �����Ͻǣ�img2�����½ǡ�
*/

#pragma once

#include "cxcore.h"

#include <stdio.h>

//��x�ľ���ֵ
#ifndef ABS
#define ABS(x) ( ( (x) < 0 )? -(x) : (x) )
#endif

//������������ȡ�����ø�������ͼ�������ֵ

static __inline int pixval8( IplImage* img, int r, int c )
{
	return (int)( ( (uchar*)(img->imageData + img->widthStep*r) )[c] );
}


static __inline void setpix8( IplImage* img, int r, int c, uchar val)
{
	( (uchar*)(img->imageData + img->widthStep*r) )[c] = val;
}


/*��32λ�����͵�ͨ��ͼ���л�ȡָ�����������ֵ����������
������
img������ͼ��
r��������
c��������
����ֵ������(c,r)��(r��c��)������ֵ
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



//������
extern void fatal_error( char* format, ... );


//��ȡһ���ļ�ȫ�������ļ�������չ�����ӵ�һ��
extern char* replace_extension( const char* file, const char* extn );

//�ļ�����ȥ��·��c:\\traffic.jpg => traffic.jpg
extern char* basename( const char* pathname );

//��ʾ�������
extern void progress( int done );

extern void erase_from_stream( FILE* stream, int n );

//���鳤�ȼӱ�
extern int array_double( void** array, int n, int size );

//��������ĶԽ��߾���
extern double dist_sq_2D( CvPoint2D64f p1, CvPoint2D64f p2 );

//�ڵ�pt�����棬���ʾ������Ǹ��㻭������
extern void draw_x( IplImage* img, CvPoint pt, int r, int w, CvScalar color );


/*������ͼ��ϳ�Ϊһ�ţ���ֱ�ŷ�,���Ƕ���֮�ͣ����Ƕ��ߵĽϴ���
������img1��λ���Ϸ���ͼ���ָ�룬img2��λ���·���ͼ���ָ��
����ֵ���ϳ�ͼ��
*/
extern IplImage* stack_imgs( IplImage* img1, IplImage* img2 );

/*������ͼ��ϳ�Ϊһ�ţ�ˮƽ�ŷ�
������img1��λ����ߵ�ͼ���ָ�룬img2��λ���ұߵ�ͼ���ָ��
����ֵ���ϳ�ͼ��
*/
extern IplImage* stack_imgs_horizontal( IplImage* img1, IplImage* img2 );

extern void vid_view( IplImage** imgs, int n, char* win_name );

//�鿴ĳ�������Ƿ��Ѿ��ر�
extern int win_closed( char* name );
