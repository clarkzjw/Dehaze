#include "dcp_core.h"

void CalcRecover(IplImage *result, IplImage *input, IplImage *transmission, double A[])
{
	int height = input->height;
	int width = input->width;
	int widthstep = input->widthStep;
	int gwidthstep = transmission->widthStep;
	int nch = input->nChannels;

	double t0 = 0.1;
	double t, tmp_res;

	int i, j, k;
	int e = 0, number = 0;
	double a = 0;

	A[0] /= 255.0;
	A[1] /= 255.0;
	A[2] /= 255.0;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			t = *(uchar *)(transmission->imageData + i * gwidthstep + j);
			t /= 255.0;
			for (k = 0; k < 3; k++)
			{
				a = *(uchar *)(input->imageData + i * widthstep + j * nch + k);
				a /= 255.0;

				tmp_res = ((a - A[k]) / MAX(t, t0)) + A[k];
				tmp_res *= 255.0;
				tmp_res = tmp_res > 255 ? 255 : (tmp_res < 0 ? 0 : tmp_res);

				*(uchar *)(result->imageData + i * widthstep + j * nch + k) = cvRound(tmp_res);
			}
		}
	}
}