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

#define M_PI 3.141592

class ofKinectV2Sensor
{
public:
	ofKinectV2Sensor(void);
	~ofKinectV2Sensor(void);

	bool init();
	bool init(bool colorMode, bool depthMode, bool bodyMode, bool bodyIndexMode, bool audioMode);
	void update();
	void updateDepthMap();
	void updateColorImage();
	void updateBodyMap();
	void updateBodyIndex();
	void updateAudio();
	void draw();
	void drawColorImage(float x, float y, float width, float height);
	void drawDepthMap(float x, float y,  float width, float height);
	void drawBodyIndex(float x, float y, float width, float height);
	void drawBodySkeleton(float x, float y, float width, float height);
	void drawAuidoBeam(void);
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

	ofxCvColorImage kinectColorImage;
	ofxCvColorImage kinectBodyImage;
	ofxCvColorImage kinectBodyIndexMap;
	ofxCvGrayscaleImage kinectDepthMap;

	// 
	int depthWidth, depthHeight;
	unsigned int depthBufferSize;

	int colorWidth, colorHeight;
	unsigned int colorBufferSize;

	Mat depthBufferMat;
	Mat depthMat;

	Mat colorBufferMat;
	Mat colorMat;

	Mat bodyBufferMat;
	Mat bodyMat;

	Mat bodyIndexMat;

};

