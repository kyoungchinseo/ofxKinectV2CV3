#include "ofMain.h"
#include "ofApp.h"
#include "ofAppKinectSimple.h"
#include "ofAppKinectSample.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1920,1080,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	//ofRunApp(new ofApp());  // test with openCV3
	//ofRunApp(new ofAppKinectSimple()); // test with kinect sdk
	ofRunApp(new ofAppKinectSample()); 
}
