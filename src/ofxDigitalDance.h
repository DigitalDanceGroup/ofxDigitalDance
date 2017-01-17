#pragma once

#include "ofMain.h"
#include "ofxDigitalDanceBvh.h"

using namespace mlib;

class ofxDigitalDance
{
public:
	ofxDigitalDance();
	~ofxDigitalDance();

private:
	// motion data
	ofxDigitalDanceBvh mMotion;

	// sound data
	ofSoundPlayer mSound;

	// movie data
	ofVideoPlayer mVideo;


	// emotional data

};
