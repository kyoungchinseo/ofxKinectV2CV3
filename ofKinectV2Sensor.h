#pragma once

#include "ofMain.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "ofxOpenCv.h"

#include <Kinect.h>
#include <Windows.h>

using namespace cv;
using namespace std;


class ofKinectV2Sensor
{
public:
	ofKinectV2Sensor(void);
	~ofKinectV2Sensor(void);

	bool initializeKinectv2();
	void update();
	void draw();
public:

	IKinectSensor*	pSensor;
	IDepthFrameSource* pDepthSource;
	IColorFrameSource* pColorSource;
	IBodyFrameSource* pBodySource;

	IColorFrameReader* pColorReader;
	IBodyFrameReader* pBodyReader;
	IDepthFrameReader* pDepthReader;

	IFrameDescription* pDepthDescription;
	IFrameDescription* pColorDescription;

	ICoordinateMapper* pCoordinateMapper;
		

	// buffer
	ofxCvGrayscaleImage grayscaleImage;
	ofxCvColorImage colorscaleImage;

	// 
	int depthWidth, depthHeight;
	unsigned int depthBufferSize;

	int colorWidth, colorHeight;
	unsigned int colorBufferSize;
};

