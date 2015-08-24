#ifndef DCP_CORE_H
#define DCP_CORE_H

#include <opencv\cv.h>
#include <opencv\highgui.h>

void CalcDarkChannel(IplImage *darkchannel, IplImage *input, int radius);

#endif