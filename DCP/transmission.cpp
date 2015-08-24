#include "dcp_core.h"

void Estimate_transmission(IplImage *Transmission_maps, IplImage *InputImage, double A[], double heap[], double real_trans[])
{
	double w = 0.95;
	double tmp;
	double tran;
	int i, j, k;
	int e = 0;

	IplImage *tmp_Trans = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	for (k = 0; k < 3; k++)
	{
		for (i = 1; i <= height; i++)
		{
			for (j = 1; j <= width; j++)
			{
				tmp = *(uchar *)(InputImage->imageData + (i - 1) * widthstep + (j - 1) * nch + k);
				tmp /= A[k];

				if (tmp > 255)
					tmp = 255;
				*(uchar *)(tmp_Trans->imageData + (i - 1) * widthstep + (j - 1) * nch + k) = tmp;
			}
		}
	}
	Cal_DarkChannel(Transmission_maps, tmp_Trans, nInteger, heap);

	for (i = 1; i <= height; i++)
	{
		for (j = 1; j <= width; j++)
		{
			tran = *(uchar *)(Transmission_maps->imageData + (i - 1) * gwidthstep + (j - 1));

			tran /= 255.0;
			tran = 1 - w * tran;

			real_trans[e++] = tran * 255.0;

			if (tran > 1)
				tran = 1;
			else if (tran < 0)
				tran = 0;

			*(uchar *)(Transmission_maps->imageData + (i - 1) * gwidthstep + (j - 1)) = tran * 255;
		}
	}
	//cvSaveImage("Estimated-Trans.bmp", Transmission_maps);
	cvReleaseImage(&tmp_Trans);
}
