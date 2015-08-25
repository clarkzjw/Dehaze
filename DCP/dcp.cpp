#include "dcp.h"

#include <iostream>
using namespace std;

void dehaze(IplImage *recover, IplImage *input)
{
	int height = input->height;
	int width = input->width;
	
	IplImage *darkchannel = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *transmission = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *refine_transmission = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

	int darkchannelradius = MIN(width, height) * 0.02;

	

	double Airlight[3] = { 0.0, 0.0, 0.0 };
	CalcDarkChannel(darkchannel, input, darkchannelradius);
	
	CalcAirlight(darkchannel, input, Airlight);
	
	CalcTransmission(transmission, input, Airlight, darkchannelradius);
	GuidedFilterColor(refine_transmission, input, transmission, 1e-6, 60);
	CalcRecover(recover, input, refine_transmission, Airlight);

	
}
