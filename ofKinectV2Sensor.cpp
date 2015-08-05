#include "ofKinectV2Sensor.h"


ofKinectV2Sensor::ofKinectV2Sensor(void)
{
	this->bColorMode = true;
	this->bDepthMode = true;
	this->bBodyMode = true;
	this->bBodyIndexMode = true;
	this->bAudioMode = true;

	colorWidth = 0;
	colorHeight = 0;
	depthWidth = 0;
	depthHeight = 0;
}


ofKinectV2Sensor::~ofKinectV2Sensor(void)
{
}

bool ofKinectV2Sensor::init() {
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

	if (bColorMode) {
		// source
 		hResult = this->pSensor->get_ColorFrameSource( &this->pColorSource );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
 			return false;
 		}
		// reader
		hResult = this->pColorSource->OpenReader( &this->pColorReader );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
 			return false;
 		}
		// description
		hResult = pColorSource->get_FrameDescription( &this->pColorDescription );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
 			return false;
 		}
	}

	if (bDepthMode) {
		// source
		hResult = pSensor->get_DepthFrameSource( &this->pDepthSource );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
 			return false;
 		}
		// reader
		hResult = this->pDepthSource->OpenReader( &this->pDepthReader );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
 			return false;
 		}
		// description
		hResult = pDepthSource->get_FrameDescription( &this->pDepthDescription );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
 			return false;
 		}
	}

	if (bBodyMode) {
		// source
		hResult = this->pSensor->get_BodyFrameSource( &this->pBodySource );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IKinectSensor::get_BodyFrameSource()" << std::endl;
 			return false;
 		}
 		// reader
 		hResult = this->pBodySource->OpenReader( &this->pBodyReader );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
 			return false;
 		}
		// get coordinate mapper
 		hResult = this->pSensor->get_CoordinateMapper( &this->pCoordinateMapper );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
 			return false;
 		}
	}

	if (bBodyIndexMode) {
		// source
		hResult = pSensor->get_BodyIndexFrameSource( &this->pBodyIndexSource );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IKinectSensor::get_BodyIndexFrameSource()" << std::endl;
 			return false;
 		}
		// reader
		hResult = this->pBodyIndexSource->OpenReader( &this->pBodyIndexReader );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IBodyIndexFrameSource::OpenReader()" << std::endl;
 			return false;
 		}
	}

	if (bAudioMode) {
		// source
		hResult = pSensor->get_AudioSource( &this->pAudioSource );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IKinectSensor::get_AudioSource()" << std::endl;
 			return false;
 		}
		// reader
		hResult = this->pAudioSource->OpenReader( &this->pAudioReader );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IAudioSource::OpenReader()" << std::endl;
 			return false;
 		}
	}

 	// initialize buffer & size
 	pDepthDescription->get_Width( &depthWidth ); // 512
	pDepthDescription->get_Height( &depthHeight ); // 424
	depthBufferSize = depthWidth * depthHeight * sizeof( unsigned short );

	pColorDescription->get_Width( &colorWidth );
	pColorDescription->get_Height( &colorHeight );
	colorBufferSize = colorWidth * colorHeight * 4 * sizeof( unsigned char );

	// setting up images for displaying
	kinectDepthMap.allocate(depthHeight,depthWidth);
	kinectColorImage.allocate(colorHeight,colorWidth);
	kinectBodyImage.allocate(colorHeight,colorWidth);
	kinectBodyIndexMap.allocate(depthHeight,depthWidth);

	// setting up buffer and map	
	depthBufferMat.create(depthHeight, depthWidth, CV_16SC1 );
	depthMat.create(depthHeight, depthWidth, CV_8UC1 );

	colorBufferMat.create( colorHeight, colorWidth, CV_8UC4 );
	colorMat.create( colorHeight, colorWidth, CV_8UC3 );

	bodyBufferMat.create(colorHeight, colorWidth,CV_8UC4);
	bodyMat.create(colorHeight, colorWidth, CV_8UC3);

	bodyIndexMat.create(depthHeight,depthWidth, CV_8UC3);
}

bool ofKinectV2Sensor::init(bool colorMode, bool depthMode, bool bodyMode, bool bodyIndexMode, bool audioMode)
{
	bColorMode = colorMode;
	bDepthMode = depthMode;
	bBodyMode = bodyMode;
	bBodyIndexMode = bodyIndexMode;
	bAudioMode  = audioMode;

	if (!this->init()) {
		return true;
	} else {
		return false;
	}
}

void ofKinectV2Sensor::update()
{
	if (bDepthMode) {
		updateDepthMap();
	}

	if (bColorMode) {
		updateColorImage();
	}
	
	if (bBodyMode) {
		updateBodyMap();
	}

	if (bBodyIndexMode) {
		updateBodyIndex();
	}

	if (bAudioMode) {
		updateAudio();
	}

	
}

void ofKinectV2Sensor::updateDepthMap()
{
	IDepthFrame* pDepthFrame = nullptr;
	HRESULT hResult = pDepthReader->AcquireLatestFrame( &pDepthFrame );

	if(SUCCEEDED( hResult )){
		hResult = pDepthFrame->AccessUnderlyingBuffer( &depthBufferSize, reinterpret_cast<UINT16**>( &depthBufferMat.data ) );
		if( SUCCEEDED( hResult ) ){
			depthBufferMat.convertTo(depthMat, CV_8U, -255.0f / 4500.0f, 255.0f);
			kinectDepthMap.setFromPixels(depthMat.data, depthWidth, depthHeight);
		}
	}

	if( pDepthFrame != NULL ){
		pDepthFrame->Release();
		pDepthFrame = NULL;
	}
}

void ofKinectV2Sensor::updateColorImage()
{
	IColorFrame* pColorFrame = nullptr;
	HRESULT hResult = pColorReader->AcquireLatestFrame( &pColorFrame );
	if( SUCCEEDED( hResult ) ){
		hResult = pColorFrame->CopyConvertedFrameDataToArray( colorBufferSize, reinterpret_cast<BYTE*>( colorBufferMat.data ), ColorImageFormat_Bgra );
		
		if( SUCCEEDED( hResult ) ){
			//cvtColor(colorBufferMat, colorMat, CV_BGR2RGB);
			cvtColor(colorBufferMat, colorMat, CV_BGRA2RGB);
			kinectColorImage.setFromPixels((unsigned char*)colorMat.data, colorWidth, colorHeight);
		}
	}
	
	if( pColorFrame != NULL ){
		pColorFrame->Release();
		pColorFrame = NULL;
	}
}

void ofKinectV2Sensor::updateBodyMap()
{
	// get color frame
	cv::Vec3b color[6];
	color[0] = cv::Vec3b( 255,   0,   0 );
	color[1] = cv::Vec3b(   0, 255,   0 );
	color[2] = cv::Vec3b(   0,   0, 255 );
	color[3] = cv::Vec3b( 255, 255,   0 );
	color[4] = cv::Vec3b( 255,   0, 255 );
	color[5] = cv::Vec3b(   0, 255, 255 );

	// copy colorImage (color must be enable to update kinectBody)
	bodyBufferMat = colorBufferMat;

	// get body frame
	IBodyFrame* pBodyFrame = nullptr;
	HRESULT hResult = pBodyReader->AcquireLatestFrame( &pBodyFrame );
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
										cv::circle( bodyBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 128, 0 ), 5, CV_AA );
 									} else if( leftHandState == HandState::HandState_Closed ){
 										cv::circle( bodyBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 0, 128 ), 5, CV_AA );
 									} else if( leftHandState == HandState::HandState_Lasso ) {
 										cv::circle( bodyBufferMat, cv::Point( x, y ), 75, cv::Scalar( 128, 128, 0 ), 5, CV_AA );
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
										cv::circle( bodyBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 128, 0 ), 5, CV_AA );
									}
									else if( rightHandState == HandState::HandState_Closed ){
										cv::circle( bodyBufferMat, cv::Point( x, y ), 75, cv::Scalar( 0, 0, 128 ), 5, CV_AA );
									}
									else if( rightHandState == HandState::HandState_Lasso ){
										cv::circle( bodyBufferMat, cv::Point( x, y ), 75, cv::Scalar( 128, 128, 0 ), 5, CV_AA );
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
								cv::circle( bodyBufferMat, cv::Point( x, y ), 5, static_cast< cv::Scalar >( color[count] ), -1, CV_AA );
							}
						}
					}
				}
			}
			//cvtColor(colorBufferMat, colorMat, CV_BGR2RGB);
			//colorscaleImage.setFromPixels(colorMat.data, colorWidth, colorHeight);
			cvtColor(bodyBufferMat, bodyMat, CV_BGRA2RGB);
			kinectBodyImage.setFromPixels(bodyMat.data, colorWidth, colorHeight);
		}
	}
	
	if( pBodyFrame != NULL ){
		pBodyFrame->Release();
		pBodyFrame = NULL;
	}
}

void ofKinectV2Sensor::updateBodyIndex()
{
	cv::Vec3b color[6];
	color[0] = cv::Vec3b( 255,   0,   0 );
	color[1] = cv::Vec3b(   0, 255,   0 );
	color[2] = cv::Vec3b(   0,   0, 255 );
	color[3] = cv::Vec3b( 255, 255,   0 );
	color[4] = cv::Vec3b( 255,   0, 255 );
	color[5] = cv::Vec3b(   0, 255, 255 );

	IBodyIndexFrame *pBodyIndexFrame = nullptr;
	HRESULT hResult = pBodyIndexReader->AcquireLatestFrame( &pBodyIndexFrame );
	if( SUCCEEDED( hResult ) ){
		unsigned int bufferSize = 0;
		unsigned char* buffer = nullptr;
		hResult = pBodyIndexFrame->AccessUnderlyingBuffer( &bufferSize, &buffer );
		if ( SUCCEEDED( hResult ) ){
			for( int y = 0; y < depthHeight; y++ ){
				for( int x = 0; x < depthWidth; x++ ){
					unsigned int index = y * depthWidth + x;
					if( buffer[index] != 0xff ){
						bodyIndexMat.at<cv::Vec3b>( y, x ) = color[buffer[index]];
					} else {
						bodyIndexMat.at<cv::Vec3b>( y, x ) = cv::Vec3b( 0, 0, 0 );
					}
				}
			}

			//cvtColor(bodyBufferMat, bodyMat, CV_BGRA2RGB);
			kinectBodyIndexMap.setFromPixels(bodyIndexMat.data, depthWidth, depthHeight);
		}
	}
	
	if( pBodyIndexFrame != NULL ){
		pBodyIndexFrame->Release();
		pBodyIndexFrame = NULL;
	}
}

void ofKinectV2Sensor::updateAudio()
{
	IAudioBeamFrameList* pAudioFrameList = nullptr;
	HRESULT hResult = pAudioReader->AcquireLatestBeamFrames( &pAudioFrameList );
	if( SUCCEEDED( hResult ) ) {
		UINT count = 0;
		hResult = pAudioFrameList->get_BeamCount( &count );
		if( SUCCEEDED( hResult ) ){
			for( int index = 0; index < count; index++ ){
				// Frame
				IAudioBeamFrame* pAudioFrame = nullptr;
				hResult = pAudioFrameList->OpenAudioBeamFrame( index, &pAudioFrame );
				if( SUCCEEDED( hResult ) ){
					// Get Beam Angle and Confidence
					IAudioBeam* pAudioBeam = nullptr;
					hResult = pAudioFrame->get_AudioBeam( &pAudioBeam );
					if( SUCCEEDED( hResult ) ){
						FLOAT angle = 0.0f;
						FLOAT confidence = 0.0f;
						pAudioBeam->get_BeamAngle( &angle );
						pAudioBeam->get_BeamAngleConfidence( &confidence );

						// Convert from radian to degree : degree = radian * 180 / Pi
						if( confidence > 0.5f ){
							std::cout << "Index : " << index << ", Angle : " << angle * 180.0f / M_PI << ", Confidence : " << confidence << std::endl;
						}
					}
					//SafeRelease( pAudioBeam );
					if( pAudioBeam != NULL ){
						pAudioBeam->Release();
						pAudioBeam = NULL;
					}
				}
				//SafeRelease( pAudioFrame );
				if( pAudioFrame != NULL ){
					pAudioFrame->Release();
					pAudioFrame = NULL;
				}
			}
		}
	}
	//SafeRelease( pAudioFrameList );
	if( pAudioFrameList != NULL ){
		pAudioFrameList->Release();
		pAudioFrameList = NULL;
	}
}

void ofKinectV2Sensor::draw()
{
	drawColorImage(0,0,colorWidth/2, colorHeight/2);
	drawBodySkeleton(colorWidth/2,0,colorWidth/2, colorHeight/2);
	drawDepthMap(0,colorHeight/2,depthWidth, depthHeight);
	drawBodyIndex(depthWidth, colorHeight/2, depthWidth, depthHeight);
}

void ofKinectV2Sensor::drawColorImage(float x, float y, float width, float height)
{
	kinectColorImage.draw(x,y,width,height);
}

void ofKinectV2Sensor::drawDepthMap(float x, float y,  float width, float height)
{
	kinectDepthMap.draw(x,y,width,height);
}

void ofKinectV2Sensor::drawBodyIndex(float x, float y, float width, float height)
{
	kinectBodyIndexMap.draw(x,y,width,height);
}

void ofKinectV2Sensor::drawBodySkeleton(float x, float y, float width, float height)
{
	kinectBodyImage.draw(x,y,width,height);
}

void ofKinectV2Sensor::drawAuidoBeam(void)
{

}