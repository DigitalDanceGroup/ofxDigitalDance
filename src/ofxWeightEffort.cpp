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
			ofQuaternion v = o->getChildren()[0]->getRotate() - o->getRotate();
			vec.push_back(v.getEuler());
		}
		else if (o->getChildren().size() > 1)
		{
			for (int i = 0; i < o->getChildren().size(); i++) {
				ofQuaternion v = o->getChildren()[i]->getRotate() - o->getRotate();
				vec.push_back(v.getEuler());
			}
		}
	}
}


float ofxWeightEffort::computeWeightEffort(ofxDigitalDanceBvh &bvh, int frame)
{
	if (frame > 0 && frame < bvh.getNumFrames())
	{
		int count_joint = 0; // number of joint for WeightEffort
		int countend = 0;
		double we_total = double(0.0); // total ofweight effort

		for (int j = 0; j < bvh.getNumJoints(); j++) {
			string name = bvh.getJoint(j)->getName();

			// joint name for WeightEffort
			if (name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" ||
				name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip" || name == "LeftKnee")
			{

				ofVec3f v0 = bvh.getRotationEuler(frame, j - countend);
				ofVec3f v1 = bvh.getRotationEuler(frame - 1, j - countend);
				ofVec3f v = v0 - v1;
				we_total += (abs(v.x) + abs(v.y) + abs(v.z));
				count_joint++;
			}
			else if (name == "Site") {
				countend++;
			}
		}
		we_total = we_total / (float)count_joint;
		return we_total;
	}
	else {
		return 0.0f;
	}
}

vector<float> &ofxWeightEffort::computeWeightEffortAll(ofxDigitalDanceBvh &bvh)
{
	vector<float> we;
	for (int i = 0; i < bvh.getNumFrames(); i++) {
		float value = computeWeightEffort(bvh, i);
		we.push_back(value);
	}
	return we;
}