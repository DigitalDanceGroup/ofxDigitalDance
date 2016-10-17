#pragma once

#include "ofMain.h"
#include "ofxBvh.h"

namespace mlib
{
    
class ofxDigitalDanceBvh : public ofxBvh
{
public:

    int getNumFrames();
    int getFrameSize();

    // export function
	void exportBvh(string filename);
    void exportScaledOffset(string filename, float scale);
    //vector<vector<double>> ofxBvh::exportAngular();

    virtual void update();
    virtual void draw();
    void drawElipsoid();
    void drawPerfume();

	void exportAngulartoCSV(string filename);
    void exportAngulartoTXT(string filename);
    void exportAngularVelocity(string filename);
    float getWeightEffort();
    float getVelAve();
    float getFrameTime();
    
	void childroop(const ofxBvhJoint* joint, string str);
    void childroopScaledOffset(const ofxBvhJoint* joint, string str, float scale);
    void Resampling(const float frametime, string filename);
				
    // Interpolation
    void InterpolatePush(string filename, ofxDigitalDanceBvh* next, int range);
    void InterpolateAttitude(string filename, ofxDigitalDanceBvh* next, int range);
    void LerpBVH(string filename1, string filename2, int range);
    void LerpBVH(ofxDigitalDanceBvh* next, int range);
	void CubicInterpolateBVH(ofxDigitalDanceBvh* next, int range);
    float CubicInterpolate(float start, float stop, float amt) {
		return ((-2.0)*(stop - start)*amt*amt*amt) + (3.0*(stop - start)*amt*amt) + start;
	};

	float getConnectivity(ofxDigitalDanceBvh* next);
    void FillBlank(int num_seg, ofxDigitalDanceBvh bvhs[], int arraysize, int start = 0, int goal = 1);
	void FillBlank2(int num_seg, ofxDigitalDanceBvh bvhs[], vector<vector<float>>& con_mat, int arraysize, int start = 0, int goal = 1);
    
	void motionInitialize();
	void setTransRotate(ofVec3f root_pos, ofQuaternion *q);
	void exportFrameBVH(const string &filename, int start, int end, float scale);
    void DFSJoint(ofxBvhJoint *joint, string blank, float scale);

	// segmentation
    void segmentationBVH(string filename, int rows, double data[]);
    void segmentationBVH_4C(string filename);
    
    // Sampling
	int GreedySampling(float q, float weighteffort[], float max_WE, vector<vector<float>>& D, int Sn[], int n, float rho = 1.0);
    
    float rel_Music_WE(float q, float weighteffort, float max_WE);
    float phi_DistanceScore(int bvh_path_num, vector<vector<float>>& D, int Sn[], int n);
    
    void drawEllipsoid(ofPoint p1, ofPoint p2, float thickness);
    
private:
    ofstream ofs;
    ofstream out;
    int mNumFrames;
};

struct Node
{
	std::vector<int> edges_to;
    std::vector<double> edges_cost;
    
    bool done;
    double cost;
    int toGoal_num;
};

}