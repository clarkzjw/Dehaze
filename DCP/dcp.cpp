#include "dcp.h"

void dehaze(IplImage *recover, IplImage *input)
{
	int height = input->height;
	int width = input->width;
	
	IplImage *darkchannel = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *transmission = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *refine_transmission = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

	int darkchannelradius = MIN(width, height) * 0.02;

	CalcDarkChannel(darkchannel, input, darkchannelradius);



}
