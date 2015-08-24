#include "dcp_core.h"

#include <iostream>
#include <vector>

using namespace std;

void GuidedFilterColor(IplImage *q, IplImage *II, IplImage *p, double eps, int r)
{
	int height = q->height;
	int width = q->width;
	int widthstep = II->widthStep;
	int gwidthstep = p->widthStep;
	int nch = II->nChannels;

	int i, j;
	int m, n;
	int w;
	int e = 0;
	int st_row, ed_row;
	int st_col, ed_col;

	double sum_Ir, sum_Ig, sum_Ib;
	double sum_Ir_square, sum_Ig_square, sum_Ib_square;
	double sum_IrIg, sum_IgIb, sum_IrIb;
	double sum_PiIr, sum_PiIg, sum_PiIb;
	double sum_Pi;

	double A, B, C, D, E, F, G, H, I, J, K, L;
	double X, Y, Z;
	double ak_r, ak_g, ak_b;
	double bk;
	double det;

	double tmp_Ir, tmp_Ig, tmp_Ib;
	double tmp_p, tmp_q;

	vector<double> v_ak_r;
	vector<double> v_ak_g;
	vector<double> v_ak_b;
	vector<double> v_bk;

	for (i = 1; i <= height; i++)
	{
		if (i % 10 == 0)
			printf("%d\n", i);

		for (j = 1; j <= width; j++)
		{
			st_row = i - r, ed_row = i + r;
			st_col = j - r, ed_col = j + r;

			if (st_row <= 0)
				st_row = 1;
			if (ed_row > height)
				ed_row = height;
			if (st_col <= 0)
				st_col = 1;
			if (ed_col > width)
				ed_col = width;

			sum_Ir = sum_Ig = sum_Ib = 0;
			sum_Ir_square = sum_Ig_square = sum_Ib_square = 0;
			sum_IrIg = sum_IgIb = sum_IrIb = 0;
			sum_PiIr = sum_PiIg = sum_PiIb = 0;
			sum_Pi = 0;
			w = 0;

			for (m = st_row; m <= ed_row; m++)
			{
				for (n = st_col; n <= ed_col; n++)
				{
					tmp_Ib = *(uchar *)(II->imageData + (m - 1) * widthstep + (n - 1) * nch);
					tmp_Ig = *(uchar *)(II->imageData + (m - 1) * widthstep + (n - 1) * nch + 1);
					tmp_Ir = *(uchar *)(II->imageData + (m - 1) * widthstep + (n - 1) * nch + 2);

					tmp_p = *(uchar *)(p->imageData + (m - 1) * gwidthstep + (n - 1));

					sum_Ib += tmp_Ib;
					sum_Ig += tmp_Ig;
					sum_Ir += tmp_Ir;

					sum_Ib_square += tmp_Ib * tmp_Ib;
					sum_Ig_square += tmp_Ig * tmp_Ig;
					sum_Ir_square += tmp_Ir * tmp_Ir;

					sum_IrIg += tmp_Ir * tmp_Ig;
					sum_IgIb += tmp_Ig * tmp_Ib;
					sum_IrIb += tmp_Ir * tmp_Ib;

					sum_Pi += tmp_p;
					sum_PiIb += tmp_p * tmp_Ib;
					sum_PiIg += tmp_p * tmp_Ig;
					sum_PiIr += tmp_p * tmp_Ir;

					w++;
				}
			}

			A = (sum_Ir_square + w * eps) * sum_Ig - sum_Ir * sum_IrIg;
			B = sum_IrIg * sum_Ig - sum_Ir * (sum_Ig_square + w * eps);
			C = sum_IrIb * sum_Ig - sum_Ir * sum_IgIb;
			D = sum_PiIr * sum_Ig - sum_PiIg * sum_Ir;
			E = (sum_Ir_square + w * eps) * sum_Ib - sum_IrIb * sum_Ir;
			F = sum_IrIg * sum_Ib - sum_IgIb * sum_Ir;
			G = sum_IrIb * sum_Ib - (sum_Ib_square + w * eps) * sum_Ir;
			H = sum_PiIr * sum_Ib - sum_PiIb * sum_Ir;
			I = (sum_Ir_square + w * eps) * w - sum_Ir * sum_Ir;
			J = sum_IrIg * w - sum_Ig * sum_Ir;
			K = sum_IrIb * w - sum_Ib * sum_Ir;
			L = sum_PiIr * w - sum_Pi * sum_Ir;

			det = A * F * K + B * G * I + C * E * J - C * F * I - A * G * J - B * E * K;
			X = D * F * K + B * G * L + C * H * J - C * F * L - D * G * J - B * H * K;
			Y = A * H * K + D * G * I + C * E * L - C * H * I - D * E * K - A * G * L;
			Z = A * F * L + B * H * I + D * J * E - D * F * I - B * E * L - A * H * J;

			ak_r = X / det;
			ak_g = Y / det;
			ak_b = Z / det;

			bk = (sum_PiIg - sum_IrIg * ak_r - (sum_Ig_square + w * eps) * ak_g - sum_IgIb * ak_b) / sum_Ig;

			tmp_Ib = *(uchar *)(II->imageData + (i - 1) * widthstep + (j - 1) * nch);
			tmp_Ig = *(uchar *)(II->imageData + (i - 1) * widthstep + (j - 1) * nch + 1);
			tmp_Ir = *(uchar *)(II->imageData + (i - 1) * widthstep + (j - 1) * nch + 2);

			tmp_q = ak_b * tmp_Ib + ak_g * tmp_Ig + ak_r * tmp_Ir + bk;

			if (tmp_q > 255)
				tmp_q = 255;
			else if (tmp_q < 0)
				tmp_q = 0;

			*(uchar *)(q->imageData + (i - 1) * gwidthstep + (j - 1)) = tmp_q;

			v_ak_b.push_back(ak_b);
			v_ak_g.push_back(ak_g);
			v_ak_r.push_back(ak_r);
			v_bk.push_back(bk);
		}
	}

	for (int i = 1; i <= height; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			st_row = i - r, ed_row = i + r;
			st_col = j - r, ed_col = j + r;

			if (st_row <= 0)
				st_row = 1;
			if (ed_row > height)
				ed_row = height;
			if (st_col <= 0)
				st_col = 1;
			if (ed_col > width)
				ed_col = width;

			double ak_r, ak_g, ak_b, bk;
			ak_r = ak_g = ak_b = bk = 0;

			int number = 0;
			for (int m = st_row; m <= ed_row; m++)
			{
				for (int n = st_col; n <= ed_col; n++)
				{
					ak_r += v_ak_r[(m - 1) * width + n - 1];
					ak_g += v_ak_g[(m - 1) * width + n - 1];
					ak_b += v_ak_b[(m - 1) * width + n - 1];
					bk += v_bk[(m - 1) * width + n - 1];
					number++;
				}
			}

			ak_r /= number;
			ak_g /= number;
			ak_b /= number;
			bk /= number;

			tmp_Ib = *(uchar *)(II->imageData + (i - 1) * widthstep + (j - 1) * nch);
			tmp_Ig = *(uchar *)(II->imageData + (i - 1) * widthstep + (j - 1) * nch + 1);
			tmp_Ir = *(uchar *)(II->imageData + (i - 1) * widthstep + (j - 1) * nch + 2);

			tmp_q = ak_b * tmp_Ib + ak_g * tmp_Ig + ak_r * tmp_Ir + bk;

			if (tmp_q > 255)
				tmp_q = 255;
			else if (tmp_q < 0)
				tmp_q = 0;

			*(uchar *)(q->imageData + (i - 1) * gwidthstep + (j - 1)) = tmp_q;
		}
	}
}
