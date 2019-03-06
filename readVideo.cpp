#include <stdio.h>
#include "opencv2/opencv.hpp"

int main(int argc, char** argv)
{
	CvCapture* capture = NULL;
	IplImage* frame = NULL;
	char key = 0;
	capture = cvCreateCameraCapture(-1); //������ͷ 
	//capture = cvCreateFileCapture("f://");
	if (NULL == capture)
	{
		printf("Read Video fail!\n");
		return -1;
	}
	cvNamedWindow("video");
	
	while(1)
	{
		frame = cvQueryFrame(capture);
		if(NULL == frame)
		{
			break;
		}
		
		cvShowImage("video", frame);
		key = cvWaitKey(33);
		if(27 == key)
		{
			break;
		}
	}
	
	
	
	cvReleaseCapture(&capture); //capture������һ��ָ�� 
	cvDestroyWindow("video");
	return 0; 
}
