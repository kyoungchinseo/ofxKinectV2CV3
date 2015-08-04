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

	bool init();
	bool init(bool colorMode, bool depthMode, bool bodyMode, bool bodyIndexMode, bool audioMode);
	void update();
	void draw();
public:

	IKinectSensor*		pSensor;

	IColorFrameSource*	pColorSource;
	IDepthFrameSource*	pDepthSource;
	IBodyFrameSource*	pBodySource;
	IBodyIndexFrameSource *pBodyIndexSource;
	IAudioSource*		pAudioSource;


	IColorFrameReader*	pColorReader;
	IDepthFrameReader*	pDepthReader;
	IBodyFrameReader*	pBodyReader;
	IBodyIndexFrameReader *pBodyIndexReader;
	IAudioBeamFrameReader *pAudioReader;

	IFrameDescription*	pDepthDescription;
	IFrameDescription*	pColorDescription;

	ICoordinateMapper*	pCoordinateMapper;
		
	// flags
	bool bColorMode;
	bool bDepthMode;
	bool bBodyIndexMode;
	bool bBodyMode;
	bool bAudioMode;


	// buffer
	ofxCvGrayscaleImage grayscaleImage;
	ofxCvColorImage colorscaleImage;

	// 
	int depthWidth, depthHeight;
	unsigned int depthBufferSize;

	int colorWidth, colorHeight;
	unsigned int colorBufferSize;
};

