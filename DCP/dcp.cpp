#include "dcp_core.h"

void dehaze(IplImage *recover, IplImage *input)
{
	int height = input->height;
	int width = input->width;
	
	IplImage *darkchannel = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *transmission = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *refine_transmission = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

	int darkchannelradius = cvRound(MIN(width, height) * 0.02);
	double Airlight[3] = { 0.0, 0.0, 0.0 };

	TicToc t;
	t.tic();
	printf("CalcDarkChannel...");
	CalcDarkChannel(darkchannel, input, darkchannelradius);
	t.toc();

	t.tic();
	printf("CalcAirlight...");
	CalcAirlight(darkchannel, input, Airlight);
	t.toc();

	t.tic();
	printf("CalcTransmission...");
	CalcTransmission(transmission, input, Airlight, darkchannelradius);
	t.toc();

	t.tic();
	printf("GuidedFilterColor...");
	GuidedFilterColor(refine_transmission, input, transmission, 1e-6, 60);
	t.toc();

	t.tic();
	printf("CalcRecover...");
	CalcRecover(recover, input, refine_transmission, Airlight);	
	t.toc();

	cvReleaseImage(&refine_transmission);
	cvReleaseImage(&transmission);
	cvReleaseImage(&darkchannel);
}
