#include "ofxMotionRhythm.h"

ofxMotionRhythm::ofxMotionRhythm() 
	: mStep(0), mOffset(0), mTempoX(0), mTempoY(10)
{

}

ofxMotionRhythm::~ofxMotionRhythm()
{

}

// calculate local minimum at each window step and define it as motion rhythm
void ofxMotionRhythm::computeRhythm(vector<float> WeightEffort)
{

}

void ofxMotionRhythm::draw(int x, int y, int w, int h)
{
	mOffset = mTempoX;
	mStep = mTempoY + 1;
	for (int i = 0; i < w; i += mStep) {
		ofPoint p1(x + mOffset + i, y );
		ofPoint p2(x + mOffset + i, y + h);
		ofDrawLine(p1, p2);
	}
}

void ofxMotionRhythm::keyPressed(int key)
{
	switch (key)
	{
		case OF_KEY_RIGHT:
			mTempoX += 1;
			break;
		case OF_KEY_LEFT:
			mTempoX -=1;
			break;
		case OF_KEY_UP:
			mTempoY += 1;
			break;
		case OF_KEY_DOWN:
			mTempoY -=1;
			break;
		default:
			break;
	}
}