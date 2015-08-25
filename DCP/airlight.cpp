#include "dcp_core.h"
#include <malloc.h>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Pixel
{
	int value;
	int i, j;
};

void CalcAirlight(IplImage *darkchannel, IplImage *input, double A[])
{
	int height = input->height;
	int width = input->width;
	int widthstep = input->widthStep;
	int gwidthstep = darkchannel->widthStep;
	int nch = input->nChannels;

	struct Pixel *v_darkchannel = (struct Pixel *)malloc(sizeof(struct Pixel) * height * width);
	int count = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int value = *(uchar *)(darkchannel->imageData + i * gwidthstep + j);
			struct Pixel p = { value, i, j };
			v_darkchannel[count++] = p;
		}
	}
	sort(v_darkchannel, v_darkchannel + count, [](struct Pixel &a, struct Pixel &b){ return a.value > b.value; });

	
	IplImage *mask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	cvZero(mask);

	for (int i = 0; i < count * 0.001; i++)
	{
		struct Pixel p = v_darkchannel[i];
		*(uchar *)(mask->imageData + p.i * gwidthstep + p.j) = 255;
	}
	

	for (int k = 0; k < 3; k++)
	{
		struct Pixel *v_channel = (struct Pixel *)malloc(sizeof(struct Pixel) * height * width);
		int count = 0;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int flag = *(uchar *)(mask->imageData + i * gwidthstep + j);
				if (flag == 0)
					continue;

				int value = *(uchar *)(input->imageData + i * widthstep + j * nch + k);
				struct Pixel p = { value, i, j };
				v_channel[count++] = p;
			}
		}

		sort(v_channel, v_channel + count, [](struct Pixel &a, struct Pixel &b){ return a.value > b.value; });
		
		int channel_airlight = 0;
		for (int i = 0; i < count * 0.01; i++)
		{
			channel_airlight += v_channel[i].value;
		}
		channel_airlight /= (count * 0.01);
		A[k] = channel_airlight;

		free(v_channel);
	}
	free(v_darkchannel);
}