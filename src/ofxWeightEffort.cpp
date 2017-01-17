#include <ofMain.h>
#include "ofxWeightEffort.h"

using namespace mlib;

ofxWeightEffort::ofxWeightEffort()
{

}

ofxWeightEffort::~ofxWeightEffort()
{

}

void ofxWeightEffort::setWeight(ofxDigitalDanceBvh &bvh)
{
//	mWeight.resize(21);
//
//	for (auto &w : mWeight) {
//		w = 0.0f;
//	}
//
//	//// 1. Hips - Chest
//	//mWeight[0] = std::make_tuple("Hips", "Chest", 1.0);
//
//	//// 2. Chest - Chest2
//	//mWeight[1] = std::make_tuple("Chest", "Chest2", 1.0);
//
//	//// 3. Chest2 - Neck
//	//mWeight[2] = std::make_tuple("Chest2", "Neck", 1.0);
//
//	//// 4. Chest2 - LeftCollar
//	//// 5. Chest2 - RightCollar
//	//mWeight[3] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[4] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 6. LeftCollar - LeftShoulder
//	//// 7. RightCollar - RightShoulder
//	//mWeight[5] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[6] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 8. LeftShoulder - LeftElbow
//	//// 9. RightSoulder - RightElbow
//	//mWeight[7] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[8] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 10. LeftElbow - LeftWrist
//	//// 11. RightElbow - RightWrist
//	//mWeight[9] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[10] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 12. LeftWrist - LeftHandFore3
//	//// 13. RightWrist - RightHandFore3
//	//mWeight[3] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[4] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 14. Hips - LeftHip
//	//// 15. Hips - RightHip
//	//mWeight[3] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[4] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 16. LeftHips - LeftKnee
//	//// 17. RightHips - RightKnee
//	//mWeight[3] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[4] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 18. LeftKnee - LeftAnkle
//	//// 19. RightKnee - RightAngle
//	//mWeight[3] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[4] = std::make_tuple("Chest2", "RightCollar", 1.0);
//
//	//// 20. LeftAnkle - LeftToe
//	//// 21. RightAngle - RightToe
//	//mWeight[3] = std::make_tuple("Chest2", "LeftCollar", 1.0);
//	//mWeight[4] = std::make_tuple("Chest2", "RightCollar", 1.0);
}

void setVector(ofxDigitalDanceBvh &bvh, vector<ofVec3f> &vec)
{
	for (int i = 0; i < bvh.getNumJoints(); i++)
	{
		ofxBvhJoint *o = const_cast<ofxBvhJoint*>(bvh.getJoint(i));

		if (o->isSite())
		{
		}
		else if (o->getChildren().size() == 1)
		{
			ofVec3f v = o->getChildren()[0]->getPosition() - o->getPosition();
			vec.push_back(v);
		}
		else if (o->getChildren().size() > 1)
		{
			for (int i = 0; i < o->getChildren().size(); i++) {
				ofVec3f v = o->getChildren()[i]->getPosition() - o->getPosition();
				vec.push_back(v);
			}
		}
	}
}


void ofxWeightEffort::computeWeightEffort(ofxDigitalDanceBvh &bvh, int frame)
{
	if (bvh.getNumFrames() < frame + 1) {
		return;
	}

	int currentFrame;
	bool changed = false;
	std::vector<ofVec3f> frame_1;
	std::vector<ofVec3f> frame_2;

	// if current frame and required frame are different each other
	if (bvh.getFrame() != frame) {
		currentFrame = bvh.getFrame();
		changed = true;
	}

	// frame1
	bvh.setFrame(frame);
	setVector(bvh, frame_1);

	// frame2
	bvh.setFrame(frame + 1);
	setVector(bvh, frame_2);

	int numJoints = frame_1.size();
	for (int i = 0; i < numJoints; i++)
	{
		frame_2[i] - frame_1[i];
	}
}
