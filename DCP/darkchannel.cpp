#include "dcp_core.h"

static inline int minbgr(int b, int g, int r)
{
	b = b > g ? g : b;
	b = b > r ? r : b;
	return b;
}

void CalcDarkChannel(IplImage *darkchannel, IplImage *input, int radius)
{
	int height = input->height;
	int width = input->width;
	int widthstep = input->widthStep;
	int gwidthstep = darkchannel->widthStep;
	int nch = input->nChannels;

	int st_row, ed_row;
	int st_col, ed_col;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			st_row = i - radius, ed_row = i + radius;
			st_col = j - radius, ed_col = j + radius;

			if (st_row < 0)
				st_row = 0;
			if (ed_row >= height)
				ed_row = height - 1;
			if (st_col < 0)
				st_col = 0;
			if (ed_col >= width)
				ed_col = width - 1;

			int cur = 0;
			int min = 300;

			for (int m = st_row; m <= ed_row; m++)
			{
				for (int n = st_col; n <= ed_col; n++)
				{
					for (int k = 0; k < 3; k++)
					{
						cur = *(uchar *)(input->imageData + m * widthstep + n * nch + k);
						if (cur < min)
							min = cur;
					}
				}
			}
			*(uchar *)(darkchannel->imageData + i * gwidthstep + j) = min;
		}
	}
}
