/*
思路：1、加载一幅图像 2、调整大小 3、灰度化->二值化->查找轮廓 4、遍历轮廓点缀 
特别的，本程序有对特定轮廓点缀以特定颜色 
*/ 
#include <cv.h>
#include <highgui.h>  
#include <stdlib.h>
#include <string.h>
#include "gif.h"

#define WIDTH	240
#define HEIGHT	240 //设定gif图的宽、高 
#define DELAY	23  // 单位是1/100秒钟 即100表示一秒
#define OUT_FILE "I:\\test2.gif" //git图输出全路径 
#define IMG_PATH "hua.jpg"

int main(int argc, char** argv)
{
	IplImage* img_src = NULL;
	IplImage* img_dst = NULL;
	IplImage* img_tmp = NULL;
	IplImage* img_gif = NULL;

//加载图像	
	img_src = cvLoadImage(IMG_PATH);
	if (NULL == img_src)
	{
		printf("load img fail.\n");
		return -1;
	}
	
	cvNamedWindow("img");	
	printf("img info, width:%d ; height:%d\n", img_src->width, img_src->height);
	
	img_tmp = cvCreateImage(cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U, 1);
	img_dst = cvCreateImage(cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U, 3);
	img_gif = cvCreateImage(cvSize(WIDTH, HEIGHT),IPL_DEPTH_8U, 4);

//调整大小	
	cvResize(img_src, img_dst);
	cvCvtColor(img_dst, img_tmp, CV_BGR2GRAY);//单通道，灰度化
	cvSet(img_dst, cvScalar(255,255,255)); 
	cvThreshold(img_tmp, img_tmp, 80, 255, CV_THRESH_BINARY_INV);//阈值，二值化 
//	cvDilate(img_tmp, img_tmp); //膨胀一下 
	cvShowImage("img", img_tmp);
	cvWaitKey(1000);
//查找轮廓 
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour_seq = NULL;
	cvFindContours(img_tmp, storage, &contour_seq, sizeof(CvContour), 
			CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); //提取出全部的轮廓 
	CvSeq* contour_h = contour_seq;
	CvSeq* contour_v = NULL;
	int contourCnt = 0;

//创建writer,准备写gif	
	GifWriter writer;
	if (!GifBegin(&writer, OUT_FILE, WIDTH, HEIGHT, DELAY))
	{
		return 1;
	}
//水平遍历轮廓 
	for (;contour_h!= NULL; contour_h = contour_h->h_next)//最后忘记赋值，刹不住车 
	{
		printf("Seq contourCnt is:%d\n", contourCnt);
		for (int i=0 ; i < contour_h->total; i++)
		{
			CvPoint* point = (CvPoint*)cvGetSeqElem(contour_h, i);
//仅对本图有效 
			if (contourCnt < 11 ) //画叶子 
			{
				cvCircle(img_dst,*point, 2, cvScalar(0,255,0), -1);		
			}else if(contourCnt < 36) //画花瓣 
			{
				cvCircle(img_dst,*point, 2, cvScalar(0,0,255), -1);		
			}else if(point->y > HEIGHT*0.66) //画柄 
			{
				cvCircle(img_dst,*point, 2, cvScalar(0,255,0), -1);
			}
			
		}	
		cvShowImage("img", img_dst);
		cvCvtColor(img_dst, img_gif, CV_BGR2RGBA);//会遍历所有点的 
//在最后一帧停留 2.5s 
		if (NULL != contour_h->h_next) 
		{ 
			GifWriteFrame(&writer, (const uint8_t*)img_gif->imageData, WIDTH, HEIGHT, DELAY);
		} 
		else
		{ 
			GifWriteFrame(&writer, (const uint8_t*)img_gif->imageData, WIDTH, HEIGHT, DELAY+250);
		} 
		if (27 == cvWaitKey(20))
		{
			break;
		}
		contourCnt++;
	}		
	printf("Seq ok\n");
	cvShowImage("img", img_dst);
	
	cvWaitKey(0);
	GifEnd(&writer);
	cvReleaseImage(&img_src);
	cvReleaseImage(&img_tmp);
	cvReleaseImage(&img_dst);
	cvReleaseImage(&img_gif);
	cvDestroyWindow("img");
	return 0;
}
