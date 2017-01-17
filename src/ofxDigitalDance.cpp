#include "ofxDigitalDance.h"
#include <tuple>
#include <ofMain.h>
#include "ofxDigitalDance.h"

ofxDigitalDance::ofxDigitalDance()
{

}

ofxDigitalDance::~ofxDigitalDance()
{

}

void ofxDigitalDance::updateMotion()
{
	mMotion.update();
}

void ofxDigitalDance::loadMotion(const string filename)
{
	mMotion.load(filename);
	mMotion.play();
}
