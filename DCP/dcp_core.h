#ifndef DCP_CORE_H
#define DCP_CORE_H

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>

#include <cstdio>
#include <malloc.h>

#include "utility.h"

void CalcDarkChannel(IplImage *darkchannel, IplImage *input, int radius);
void CalcAirlight(IplImage *darkchannel, IplImage *input, double A[]);
void CalcTransmission(IplImage *transmission, IplImage *input, double A[], int radius);
void CalcRecover(IplImage *result, IplImage *input, IplImage *transmission, double A[]);
void GuidedFilterColor(IplImage *q, IplImage *II, IplImage *p, double eps, int r);

#endif