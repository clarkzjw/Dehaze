#include "dcp_core.h"

void Recover(IplImage *Result, IplImage *InputImage, double A[])
{
	double t0 = 0.1;
	double t, tmp_res;

	int i, j, k;
	int e = 0, number = 0;
	double a = 0;

	for (i = 1; i <= height; i++)
	{
		for (j = 1; j <= width; j++)
		{
			t = real_trans[e++];
			t /= 255.0;
			for (k = 0; k < 3; k++)
			{
				a = *(uchar *)(InputImage->imageData + (i - 1) * widthstep + (j - 1) * nch + k);
				a /= 255;

				tmp_res = ((a - A[k]) / MaxTwo(t, t0)) + A[k];

				real_recover[number++] = tmp_res;

				if (tmp_res > 1)
					tmp_res = 1;
				else if (tmp_res < 0)
					tmp_res = 0;

				*(uchar *)(Result->imageData + (i - 1) * widthstep + (j - 1) * nch + k) = tmp_res * 255.0;
			}
		}
	}
}