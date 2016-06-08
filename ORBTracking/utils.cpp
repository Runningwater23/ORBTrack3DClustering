/*
此文件中实现了图像的基本操作函数:
1、获取某位置的像素点
2、设置某位置的像素点（8位，32位和64位），
3、计算两点之间的距离的平方
4、在图片某一点画一个“X”
5、将两张图片合成为一个，高是二者之和，宽是二者的较大者。img1 在左上角，img2在右下角。
*/
#include "stdafx.h"
#include "utils.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <errno.h>
#include <string.h>
#include <stdlib.h>


/*************************** 函数定义 ****************************/


/*
输出形如 "Error: ..."的错误信息
*/
void fatal_error(char* format, ...)
{
	//va_list ap;

	fprintf(stderr, "Error: ");
	fprintf(stderr, "\n");
	abort();
}


char* replace_extension(const char* file, const char* extn)
{
	char* new_file, *lastdot;

	new_file = (char*)calloc(strlen(file) + strlen(extn) + 2, sizeof(char));
	strcpy(new_file, file);
	lastdot = strrchr(new_file, '.');
	if (lastdot)
		*(lastdot + 1) = '\0';
	else
		strcat(new_file, ".");
	strcat(new_file, extn);

	return new_file;
}

char* basename(const char* pathname)
{
	char* base, *last_slash;

	last_slash = (char*)strrchr(pathname, '/');//
	if (!last_slash)
	{
		base = (char*)calloc(strlen(pathname) + 1, sizeof(char));
		strcpy(base, pathname);
	}
	else
	{
		base = (char*)calloc(strlen(last_slash++), sizeof(char));
		strcpy(base, last_slash);
	}

	return base;
}


void progress(int done)
{
	char state[4] = { '|', '/', '-', '\\' };
	static int cur = -1;

	if (cur == -1)
		fprintf(stderr, "  ");

	if (done)
	{
		fprintf(stderr, "\b\bdone\n");
		cur = -1;
	}
	else
	{
		cur = (cur + 1) % 4;
		fprintf(stdout, "\b\b%c ", state[cur]);
		fflush(stderr);
	}
}



void erase_from_stream(FILE* stream, int n)
{
	int j;
	for (j = 0; j < n; j++)
		fprintf(stream, "\b");
	for (j = 0; j < n; j++)
		fprintf(stream, " ");
	for (j = 0; j < n; j++)
		fprintf(stream, "\b");
}



int array_double(void** array, int n, int size)
{
	void* tmp;

	tmp = realloc(*array, 2 * n * size);
	if (!tmp)
	{
		fprintf(stderr, "Warning: unable to allocate memory in array_double(),"
			" %s line %d\n", __FILE__, __LINE__);
		if (*array)
			free(*array);
		*array = NULL;
		return 0;
	}
	*array = tmp;
	return n * 2;
}


double dist_sq_2D(CvPoint2D64f p1, CvPoint2D64f p2)
{
	double x_diff = p1.x - p2.x;
	double y_diff = p1.y - p2.y;

	return x_diff * x_diff + y_diff * y_diff;
}

void draw_x(IplImage* img, CvPoint pt, int r, int w, CvScalar color)
{
	cvLine(img, pt, cvPoint(pt.x + r, pt.y + r), color, w, 8, 0);
	cvLine(img, pt, cvPoint(pt.x - r, pt.y + r), color, w, 8, 0);
	cvLine(img, pt, cvPoint(pt.x + r, pt.y - r), color, w, 8, 0);
	cvLine(img, pt, cvPoint(pt.x - r, pt.y - r), color, w, 8, 0);
}


/*将两张图像合成为一张，垂直排放
参数：img1：位于上方的图像的指针，img2：位于下方的图像的指针
返回值：合成图像
*/
extern IplImage* stack_imgs(IplImage* img1, IplImage* img2)
{
	//生成合成图像
	IplImage* stacked = cvCreateImage(cvSize(MAX(img1->width, img2->width),
		img1->height + img2->height),
		IPL_DEPTH_8U, 3);

	cvZero(stacked);//清零
	cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
	cvAdd(img1, stacked, stacked, NULL);//叠加第一张图像
	cvSetImageROI(stacked, cvRect(0, img1->height, img2->width, img2->height));
	cvAdd(img2, stacked, stacked, NULL);//叠加第二张图像
	cvResetImageROI(stacked);

	return stacked;
}

/*(自己写的函数)
将两张图像合成为一张，水平排放
参数：img1：位于左边的图像的指针，img2：位于右边的图像的指针
返回值：合成图像
*/
extern IplImage* stack_imgs_horizontal(IplImage* img1, IplImage* img2)///
{
	//生成合成图像
	IplImage * stacked = cvCreateImage(cvSize(img1->width + img2->width, MAX(img1->height, img2->height)),
		IPL_DEPTH_8U, 3);
	cvZero(stacked);//清零
	cvSetImageROI(stacked, cvRect(0, 0, img1->width, img1->height));
	cvAdd(img1, stacked, stacked, NULL);//叠加第一张图像
	cvSetImageROI(stacked, cvRect(img1->width, 0, img2->width, img2->height));
	cvAdd(img2, stacked, stacked, NULL);//叠加第二张图像
	cvResetImageROI(stacked);

	return stacked;
}



void vid_view(IplImage** imgs, int n, char* win_name)
{
	int k, i = 0, playing = 0;

	cvNamedWindow(win_name, 1);
	cvShowImage(win_name, imgs[i]);
	while (!win_closed(win_name))
	{
		if (playing)
		{
			i = MIN(i + 1, n - 1);
			cvNamedWindow(win_name, 1);
			cvShowImage(win_name, imgs[i]);
			k = cvWaitKey(33);
			if (k == ' ' || i == n - 1)
				playing = 0;
		}

		else
		{
			k = cvWaitKey(0);
			switch (k)
			{
				/* space */
			case ' ':
				playing = 1;
				break;

				/* esc */
			case 27:
			case 1048603:
				cvDestroyWindow(win_name);
				break;

				/* backspace */
			case '\b':
				i = 0;
				cvNamedWindow(win_name, 1);
				cvShowImage(win_name, imgs[i]);
				break;

				/* left arrow */
			case 65288:
			case 1113937:
				i = MAX(i - 1, 0);
				cvNamedWindow(win_name, 1);
				cvShowImage(win_name, imgs[i]);
				break;

				/* right arrow */
			case 65363:
			case 1113939:
				i = MIN(i + 1, n - 1);
				cvNamedWindow(win_name, 1);
				cvShowImage(win_name, imgs[i]);
				break;

				/* page up */
			case 65365:
			case 1113941:
				i = MAX(i - 10, 0);
				cvNamedWindow(win_name, 1);
				cvShowImage(win_name, imgs[i]);
				break;

				/* page down */
			case 65366:
			case 1113942:
				i = MIN(i + 10, n - 1);
				cvNamedWindow(win_name, 1);
				cvShowImage(win_name, imgs[i]);
				break;
			}
		}
	}
}


int win_closed(char* win_name)
{
	if (!cvGetWindowHandle(win_name))
		return 1;
	return 0;
}
