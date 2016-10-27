#pragma once

#include "ofMain.h"
#include "ofxBvh.h"

namespace mlib
{
    
class ofxDigitalDanceBvh : public ofxBvh
{
public:

    ofxDigitalDanceBvh();
    ~ofxDigitalDanceBvh();
    
    ///-------------------
    /// getter
    ///-------------------
    const int getNumFrames() const;
    const int getFrameSize() const;
    const float getFrameTime() const;
    const float getWeightEffort();
    const float getVelAve();
    const float getConnectivity(ofxDigitalDanceBvh* next);

    ///-------------------
    /// setter
    ///-------------------
    void setTransRotate(ofVec3f root_pos, ofQuaternion *q);
    
    /*
     @ description
        For setting at arbitrary frame.
     @ parameter
        frame:  frame number which you want to set
     */
    virtual void setFrame(int frame);

    ///-------------------
    /// update
    ///-------------------
    virtual void update();
    
    ///-------------------
    /// draw
    ///-------------------
    
    /*
     @ description
        draw skelton using line
     */
    void drawLine();
    
    /*
     @ description
        draw skelton using drawing elipsoid <- also private function
     */
    void drawElipsoid();

    /*
     @ description
        draw bone using elipsoid with color for perfume motion data
     */
    void drawPerfume();
    
    /*
     @ description
        draw skeleton using elipsoid with blending two motion
     @ parameter
     */
    void drawMixMotion(ofxDigitalDanceBvh *next, float value, ofQuaternion& quat, ofVec3f& trans);
    void drawMixMotion(ofxDigitalDanceBvh *next, float value, ofQuaternion& quat1, ofVec3f& trans1, ofQuaternion& quat2, ofVec3f& trans2);

    ///-------------------
    /// exporter
    ///-------------------
    void exportBvh(string filename);
    void exportScaledOffset(string filename, float scale);
    void exportFrameBVH(const string &filename, int start, int end, float scale);
    void exportAngulartoCSV(string filename);
    void exportAngulartoTXT(string filename);
    void exportAngularVelocity(string filename);
    //vector<vector<double>> ofxBvh::exportAngular();
    
    ///-------------------
    /// interpolation
    ///-------------------
    void InterpolatePush(string filename, ofxDigitalDanceBvh* next, int range);
    void InterpolateAttitude(string filename, ofxDigitalDanceBvh* next, int range);
    void LerpBVH(string filename1, string filename2, int range);
    void LerpBVH(ofxDigitalDanceBvh* next, int range);
    void LerpBVH(ofxDigitalDanceBvh* next, int range, float interpolate, int startFrame1, int endFrame1, int startFrame2, int endFrame3);
	void CubicInterpolateBVH(ofxDigitalDanceBvh* next, int range);
    float CubicInterpolate(float start, float stop, float amt);
    static float calcInterpolateValue(const float& p, const float& range);

    /*
    @ description
        If you want to make connect two motion smoothly without records, you can use this function.
        before you use this, you should set both motion at arbitrary frame(you can use func:setMotionFrame)
        <---previous---current---next--->
    @ parameter
        current:      current pose
        next:         next pose
        previousMat:  matrix of root pose when previous motion start 
    */
    static const ofMatrix4x4 calcTrackPoseMatrix(ofxDigitalDanceBvh& current,
                                                 ofxDigitalDanceBvh& next,
                                                 const ofMatrix4x4& previousMat );
    
    ///-------------------
    /// segmentation
    ///-------------------
    void segmentationBVH(string filename, int rows, double data[]);
    void segmentationBVH_4C(string filename);
    
    ///-------------------
    /// Sampling
    ///-------------------
    int GreedySampling(float q, float weighteffort[], float max_WE, vector<vector<float>>& D, int Sn[], int n, float rho = 1.0);
    void Resampling(const float frametime, string filename);

    ///-------------------
    /// fill brank
    ///-------------------
    void FillBlank(int num_seg, ofxDigitalDanceBvh bvhs[], int arraysize, int start = 0, int goal = 1);
    void FillBlank2(int num_seg, ofxDigitalDanceBvh bvhs[], vector<vector<float>>& con_mat, int arraysize, int start = 0, int goal = 1);

    ///-------------------
    /// loop
    ///-------------------
    void childroop(const ofxBvhJoint* joint, string str);
    void childroopScaledOffset(const ofxBvhJoint* joint, string str, float scale);
    void DFSJoint(ofxBvhJoint *joint, string blank, float scale);
    
    ///-------------------
    /// compate pose
    ///-------------------
    float comparePose(ofxDigitalDanceBvh *next, ofQuaternion &quat, ofVec3f &trans);

    ///-------------------
    /// specfic function
    ///-------------------
	void motionInitialize();
    float rel_Music_WE(float q, float weighteffort, float max_WE);
    float phi_DistanceScore(int bvh_path_num, vector<vector<float>>& D, int Sn[], int n);
        
private:
    
    /// variable
    ofstream ofs;
    ofstream out;
    
    /// function
    void drawElipsoid(ofPoint p1, ofPoint p2, float thickness);
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