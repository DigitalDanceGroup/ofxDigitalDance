#pragma once

#include <ofMain.h>
#include "ofxDigitalDanceBvh.h"

//typedef std::tuple<string, string, float> jointTuple;

namespace mlib
{
// declaration
class ofxDigitalDanceBvh;

class ofxWeightEffort
{
public:
	ofxWeightEffort();
	~ofxWeightEffort();

	// set joint weight at each joint
	void setWeight(ofxDigitalDanceBvh &bvh);	// TODO

	// compute WE at each frame
	static float computeWeightEffort(ofxDigitalDanceBvh &bvh, int frame);

	// compute sum of WE in full sequences
	static vector<float> &computeWeightEffortAll(ofxDigitalDanceBvh &bvh);

private:
	vector<float> mWeight;
	vector<float> mWEValue; // number of frames
	//vector<jointTuple> mWeight;
};

}