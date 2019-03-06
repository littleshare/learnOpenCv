#include <stdio.h>
#include "opencv2/opencv.hpp"

int main(int argc, char** argv)
{
	IplImage *pic = NULL;
	
	pic = cvLoadImage("lena.jpg");
	if (NULL == pic)
	{
		printf("Load image fail!\n");
		return -1;
	}
	cvNamedWindow("pic");
	cvShowImage("pic", pic);
	cvWaitKey(0);
	
	cvReleaseImage(&pic);
	cvDestroyWindow("pic");
	return 0; 
} 
