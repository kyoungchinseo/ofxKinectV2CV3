#include "ofApp.h"


void ofApp::testopenCV3module() {
	Mat image;
    image = imread("data/simple.jpg"); // Read the file
	Mat imageRGB;

	// convert BGR image to RGB
	cvtColor(image, imageRGB, CV_RGB2BGR);

    if(! imageRGB.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
    }

	//vidGrabber.setVerbose(true);
    //vidGrabber.initGrabber(320,240);
	

   // namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
   // imshow( "Display window", image ); // Show our image inside it.

//   waitKey(0); // Wait for a keystroke in the window
	
	ofxImage.allocate(image.cols,image.rows);
		

	cout << image.cols << " " << image.rows << endl;
	ofxImage.setFromPixels(imageRGB.data, image.cols,image.rows);

}

//--------------------------------------------------------------
void ofApp::setup(){
	testopenCV3module();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofxImage.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
