/*
˼·��1������һ��ͼ�� 2��������С 3���ҶȻ�->��ֵ��->�������� 4������������׺ 
�ر�ģ��������ж��ض�������׺���ض���ɫ 
*/ 
#include <cv.h>
#include <highgui.h>  
#include <stdlib.h>
#include <string.h>
#include "gif.h"

#define WIDTH	240
#define HEIGHT	240 //�趨gifͼ�Ŀ��� 
#define DELAY	23  // ��λ��1/100���� ��100��ʾһ��
#define OUT_FILE "I:\\test2.gif" //gitͼ���ȫ·�� 
#define IMG_PATH "hua.jpg"

int main(int argc, char** argv)
{
	IplImage* img_src = NULL;
	IplImage* img_dst = NULL;
	IplImage* img_tmp = NULL;
	IplImage* img_gif = NULL;

//����ͼ��	
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

//������С	
	cvResize(img_src, img_dst);
	cvCvtColor(img_dst, img_tmp, CV_BGR2GRAY);//��ͨ�����ҶȻ�
	cvSet(img_dst, cvScalar(255,255,255)); 
	cvThreshold(img_tmp, img_tmp, 80, 255, CV_THRESH_BINARY_INV);//��ֵ����ֵ�� 
//	cvDilate(img_tmp, img_tmp); //����һ�� 
	cvShowImage("img", img_tmp);
	cvWaitKey(1000);
//�������� 
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour_seq = NULL;
	cvFindContours(img_tmp, storage, &contour_seq, sizeof(CvContour), 
			CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); //��ȡ��ȫ�������� 
	CvSeq* contour_h = contour_seq;
	CvSeq* contour_v = NULL;
	int contourCnt = 0;

//����writer,׼��дgif	
	GifWriter writer;
	if (!GifBegin(&writer, OUT_FILE, WIDTH, HEIGHT, DELAY))
	{
		return 1;
	}
//ˮƽ�������� 
	for (;contour_h!= NULL; contour_h = contour_h->h_next)//������Ǹ�ֵ��ɲ��ס�� 
	{
		printf("Seq contourCnt is:%d\n", contourCnt);
		for (int i=0 ; i < contour_h->total; i++)
		{
			CvPoint* point = (CvPoint*)cvGetSeqElem(contour_h, i);
//���Ա�ͼ��Ч 
			if (contourCnt < 11 ) //��Ҷ�� 
			{
				cvCircle(img_dst,*point, 2, cvScalar(0,255,0), -1);		
			}else if(contourCnt < 36) //������ 
			{
				cvCircle(img_dst,*point, 2, cvScalar(0,0,255), -1);		
			}else if(point->y > HEIGHT*0.66) //���� 
			{
				cvCircle(img_dst,*point, 2, cvScalar(0,255,0), -1);
			}
			
		}	
		cvShowImage("img", img_dst);
		cvCvtColor(img_dst, img_gif, CV_BGR2RGBA);//��������е�� 
//�����һ֡ͣ�� 2.5s 
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
