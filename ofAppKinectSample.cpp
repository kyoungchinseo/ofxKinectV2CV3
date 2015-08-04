#include "ofAppKinectSample.h"

//--------------------------------------------------------------
void ofAppKinectSample::setup(){
	//ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	// ofSetFullscreen(true);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetBackgroundAuto(true);

	kinect = new ofKinectV2Sensor();

	if (!kinect->initializeKinectv2())
		exit();
}

//--------------------------------------------------------------
void ofAppKinectSample::update(){
	kinect->update();	
}

//--------------------------------------------------------------
void ofAppKinectSample::draw(){
	kinect->draw();
}

//--------------------------------------------------------------
void ofAppKinectSample::keyPressed(int key){

}

//--------------------------------------------------------------
void ofAppKinectSample::keyReleased(int key){

}

//--------------------------------------------------------------
void ofAppKinectSample::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofAppKinectSample::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppKinectSample::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppKinectSample::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppKinectSample::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofAppKinectSample::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofAppKinectSample::dragEvent(ofDragInfo dragInfo){ 

}
