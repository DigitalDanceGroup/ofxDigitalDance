#pragma once

#include "ofMain.h"
#include "ofxDigitalDanceBvh.h"

using namespace mlib;

class ofxDigitalDance
{
public:
	ofxDigitalDance();
	~ofxDigitalDance();

	void init();
	void updateMotion();
	void drawVideo();

	// getter
	ofxDigitalDanceBvh &getMotion() { return mMotion; }
	ofSoundPlayer &getAudio() { return mSound; }

	// loader
	void loadMotion(const string filename);
	void loadAudio(const string filename);
	void loadVideo(const string filename);

private:
	// motion data
	ofxDigitalDanceBvh mMotion;

	// sound data
	ofSoundPlayer mSound;

	// movie data
	ofVideoPlayer mVideo;

	// emotional data

};
