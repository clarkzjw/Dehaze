#include "dcp_core.h"

void CalcTransmission(IplImage *transmission, IplImage *input, double A[], int radius)
{
	int width = input->width;
	int height = input->height;
	int widthstep = input->widthStep;
	int gwidthstep = transmission->widthStep;
	int nch = input->nChannels;

	double w = 0.95;
	
	IplImage *normalized_input = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	for (int k = 0; k < 3; k++)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				double tmp = *(uchar *)(input->imageData + i * widthstep + j * nch + k);
				tmp = tmp / A[k] * 255.0;

				tmp = tmp > 255 ? 255 : tmp;
				*(uchar *)(normalized_input->imageData + i * widthstep + j * nch + k) = tmp;
			}
		}
	}
	CalcDarkChannel(transmission, normalized_input, radius);

	for (int i = 0; i < height; i++)
	{	
		for (int j = 0; j < width; j++)
		{
			double tran = *(uchar *)(transmission->imageData + i * gwidthstep + j);

			tran = 1 - w * (tran / 255.0);

			tran = tran > 1 ? 1 : tran;
			tran = tran < 0 ? 0 : tran;

			*(uchar *)(transmission->imageData + i * gwidthstep + j) = tran * 255;
		}
	}
	cvReleaseImage(&normalized_input);
}
