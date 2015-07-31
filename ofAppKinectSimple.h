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

/*
template
inline void SafeRelease( Interface *& pInterfaceToRelease )
{
	if( pInterfaceToRelease != NULL ){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}
*/


class ofAppKinectSimple : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		bool initializeKinectv2();

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
