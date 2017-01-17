#pragma once

#include "ofMain.h"
#include "ofxDigitalDanceBvh.h"

using namespace mlib;
typedef std::tuple<string, string, float> jointTuple;

class weightEffort {
public:
	void setWeight(ofxDigitalDanceBvh &bvh);
	void computeWeightEffort(ofxDigitalDanceBvh &bvh, int frame);


private:
	vector<float> mWeight;
	//vector<jointTuple> mWeight;
};