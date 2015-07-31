

#include "ofAppKinectSimple.h"



//
bool ofAppKinectSimple::initializeKinectv2() {
	HRESULT hResult = S_OK;

	// Open Kinect
	hResult = GetDefaultKinectSensor( &this->pSensor );
	if( FAILED( hResult ) ){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
 		return false;
 	}
 	hResult = this->pSensor->Open( );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
 		return false;
 	}

 	// Open Source
 	hResult = this->pSensor->get_ColorFrameSource( &this->pColorSource );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
 		return false;
 	}
 	hResult = this->pSensor->get_BodyFrameSource( &this->pBodySource );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_BodyFrameSource()" << std::endl;
 		return false;
 	}
 	hResult = pSensor->get_DepthFrameSource( &this->pDepthSource );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
 		return false;
 	}
 	// Open Reader
 	hResult = this->pColorSource->OpenReader( &this->pColorReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
 		return false;
 	}
 	hResult = this->pBodySource->OpenReader( &this->pBodyReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
 		return false;
 	}
 	hResult = this->pDepthSource->OpenReader( &this->pDepthReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
 		return false;
 	}
 	// get descriptions
 	hResult = pDepthSource->get_FrameDescription( &this->pDepthDescription );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
 		return false;
 	}
 	hResult = pColorSource->get_FrameDescription( &this->pColorDescription );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
 		return false;
 	}
 	// get coordinate mapper
 	hResult = this->pSensor->get_CoordinateMapper( &this->pCoordinateMapper );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
 		return false;
 	}
 	this->pDepthDescription->get_Width( &depthWidth ); // 512
	this->pDepthDescription->get_Height( &depthHeight ); // 424
	this->depthBufferSize = depthWidth * depthHeight * sizeof( unsigned short );

	this->pColorDescription->get_Width( &colorWidth );
	this->pColorDescription->get_Height( &colorHeight );
	this->colorBufferSize = colorWidth * colorHeight * 4 * sizeof( unsigned char );

	this->grayscaleImage.allocate(depthHeight, depthWidth);
	this->colorscaleImage.allocate(colorHeight, colorWidth);



}

//--------------------------------------------------------------
void ofAppKinectSimple::setup(){
	//ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	// ofSetFullscreen(true);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetBackgroundAuto(true);

	if (!this->initializeKinectv2())
		exit();
}

//--------------------------------------------------------------
void ofAppKinectSimple::update(){
	// get depth frame
	Mat bufferMat( depthHeight, depthWidth, CV_16SC1 );
	Mat depthMat(depthHeight, depthWidth, CV_8UC1 );

	Mat colorBufferMat( colorHeight, colorWidth, CV_8UC4 );
	Mat colorMat( colorHeight, colorWidth, CV_8UC3 );

	// Frame
	IDepthFrame* pDepthFrame = nullptr;
	HRESULT hResult = pDepthReader->AcquireLatestFrame( &pDepthFrame );

	if(SUCCEEDED( hResult )){
		hResult = pDepthFrame->AccessUnderlyingBuffer( &depthBufferSize, reinterpret_cast<UINT16**>( &bufferMat.data ) );
		if( SUCCEEDED( hResult ) ){
			bufferMat.convertTo(depthMat, CV_8U, -255.0f / 4500.0f, 255.0f);
			grayscaleImage.setFromPixels(depthMat.data, depthWidth, depthHeight);
		}
	}
//	SafeRelease( pDepthFrame );
	if( pDepthFrame != NULL ){
		pDepthFrame->Release();
		pDepthFrame = NULL;
	}

	// get color frame
	cv::Vec3b color[6];
	color[0] = cv::Vec3b( 255,   0,   0 );
	color[1] = cv::Vec3b(   0, 255,   0 );
	color[2] = cv::Vec3b(   0,   0, 255 );
	color[3] = cv::Vec3b( 255, 255,   0 );
	color[4] = cv::Vec3b( 255,   0, 255 );
	color[5] = cv::Vec3b(   0, 255, 255 );

	IColorFrame* pColorFrame = nullptr;
	hResult = pColorReader->AcquireLatestFrame( &pColorFrame );
	if( SUCCEEDED( hResult ) ){
		hResult = pColorFrame->CopyConvertedFrameDataToArray( colorBufferSize, reinterpret_cast<BYTE*>( colorBufferMat.data ), ColorImageFormat_Bgra );
		
		if( SUCCEEDED( hResult ) ){
			//cvtColor(colorBufferMat, colorMat, CV_BGR2RGB);
			cvtColor(colorBufferMat, colorMat, CV_BGRA2RGB);
			colorscaleImage.setFromPixels((unsigned char*)colorMat.data, colorWidth, colorHeight);
		}
	}
	///SafeRelease( pColorFrame );
	if( pColorFrame != NULL ){
		pColorFrame->Release();
		pColorFrame = NULL;
	}


	// get body frame
	IBodyFrame* pBodyFrame = nullptr;
	hResult = pBodyReader->AcquireLatestFrame( &pBodyFrame );
	if( SUCCEEDED( hResult ) ){
		IBody* pBody[BODY_COUNT] = { 0 };
		hResult = pBodyFrame->GetAndRefreshBodyData( BODY_COUNT, pBody );
		if( SUCCEEDED( hResult ) ){
			for( int count = 0; count < BODY_COUNT; count++ ){
 				BOOLEAN bTracked = false;
 				hResult = pBody[count]->get_IsTracked( &bTracked );
				if( SUCCEEDED( hResult ) && bTracked ){
					Joint joint[JointType::JointType_Count];
					hResult = pBody[ count ]->GetJoints( JointType::JointType_Count, joint );
					if( SUCCEEDED( hResult ) ){
						// Left Hand State
						HandState leftHandState = HandState::HandState_Unknown;
						hResult = pBody[count]->get_HandLeftState( &leftHandState );
						if( SUCCEEDED( hResult ) ){
							ColorSpacePoint colorSpacePoint = { 0 };
							hResult = pCoordinateMapper->MapCameraPointToColorSpace( joint[JointType::JointType_HandLeft].Position, &colorSpacePoint );
							if( SUCCEEDED( hResult ) ){
								int x = int(colorSpacePoint.X);
								int y = int(colorSpacePoint.Y);
								int shift = 80;
								if( ( x >= 0+shift ) && ( x < colorWidth-shift ) && ( y >= 0+shift ) && ( y < colorHeight-shift ) ){							
									if( leftHandState == HandState::HandState_Open ){ 										
										cv::circle( colorBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 128, 0 ), 5, CV_AA );
 									} else if( leftHandState == HandState::HandState_Closed ){
 										cv::circle( colorBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 0, 128 ), 5, CV_AA );
 									} else if( leftHandState == HandState::HandState_Lasso ) {
 										cv::circle( colorBufferMat, cv::Point( x, y ), 75, cv::Scalar( 128, 128, 0 ), 5, CV_AA );
 									}
 							    }
 							}
 						}
 						// Right Hand State
 						HandState rightHandState = HandState::HandState_Unknown;
 						hResult = pBody[count]->get_HandRightState( &rightHandState );
						if( SUCCEEDED( hResult ) ){
							ColorSpacePoint colorSpacePoint = { 0 };
							hResult = pCoordinateMapper->MapCameraPointToColorSpace( joint[JointType::JointType_HandRight].Position, &colorSpacePoint );
							if( SUCCEEDED( hResult ) ){
								int x = int( colorSpacePoint.X );
								int y = int( colorSpacePoint.Y );
								int shift = 80;
								if( ( x >= 0+shift ) && ( x < colorWidth-shift ) && ( y >= 0+shift ) && ( y < colorHeight-shift ) ){
									if( rightHandState == HandState::HandState_Open ){
										cv::circle( colorBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 128, 0 ), 5, CV_AA );
									}
									else if( rightHandState == HandState::HandState_Closed ){
										cv::circle( colorBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 0, 128 ), 5, CV_AA );
									}
									else if( rightHandState == HandState::HandState_Lasso ){
										cv::circle( colorBufferMat, cv::Point( x, y ), 75, cv::Scalar( 128, 128, 0 ), 5, CV_AA );
									}
								}
							}
						}

						// Joint
						for( int type = 0; type < JointType::JointType_Count; type++ ){
 							ColorSpacePoint colorSpacePoint = { 0 };
 							pCoordinateMapper->MapCameraPointToColorSpace( joint[type].Position, &colorSpacePoint );
							int x = int( colorSpacePoint.X );
							int y = int( colorSpacePoint.Y );
							int shift = 10;
							if( ( x >= 0 + shift ) && ( x < colorWidth-shift ) && ( y >= 0+shift ) && ( y < colorHeight-shift ) ){
								cv::circle( colorBufferMat, cv::Point( x, y ), 5, static_cast< cv::Scalar >( color[count] ), -1, CV_AA );
							}
						}
					}
				}
			}
			//cvtColor(colorBufferMat, colorMat, CV_BGR2RGB);
			//colorscaleImage.setFromPixels(colorMat.data, colorWidth, colorHeight);
			cvtColor(colorBufferMat, colorMat, CV_BGRA2RGB);
			colorscaleImage.setFromPixels(colorMat.data, colorWidth, colorHeight);
		}
	}
	//SafeRelease( pBodyFrame );
	if( pBodyFrame != NULL ){
		pBodyFrame->Release();
		pBodyFrame = NULL;
	}
}

//--------------------------------------------------------------
void ofAppKinectSimple::draw(){
	colorscaleImage.draw(0, 0);
	grayscaleImage.draw(0, 0);
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
