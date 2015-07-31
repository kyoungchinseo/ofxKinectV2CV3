

#include "ofAppKinectSimple.h"


void ofAppKinectSimple::testopenCV3module() {
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
void ofAppKinectSimple::setup(){
	testopenCV3module();
}

//--------------------------------------------------------------
void ofAppKinectSimple::update(){

}

//--------------------------------------------------------------
void ofAppKinectSimple::draw(){
	ofxImage.draw(0,0);
}

//--------------------------------------------------------------
void ofAppKinectSimple::keyPressed(int key){

}

//--------------------------------------------------------------
void ofAppKinectSimple::keyReleased(int key){

}

//--------------------------------------------------------------
void ofAppKinectSimple::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofAppKinectSimple::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppKinectSimple::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppKinectSimple::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofAppKinectSimple::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofAppKinectSimple::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofAppKinectSimple::dragEvent(ofDragInfo dragInfo){ 

}
