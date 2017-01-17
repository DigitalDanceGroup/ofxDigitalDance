#pragma once

#include <ofMain.h>
#include "ofxDigitalDanceBvh.h"

typedef std::tuple<string, string, float> jointTuple;

namespace mlib
{
// declaration
class ofxDigitalDanceBvh;

class ofxWeightEffort
{
public:
	ofxWeightEffort();
	~ofxWeightEffort();

	void setWeight(ofxDigitalDanceBvh &bvh);
	void computeWeightEffort(ofxDigitalDanceBvh &bvh, int frame);

private:
	vector<float> mWeight;
	//vector<jointTuple> mWeight;
};

}