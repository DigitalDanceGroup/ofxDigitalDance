#pragma once

#include "ofMain.h"
#include "ofxBvh.h"

class ofxDigitalDanceBvh : public ofxBvh
{
public:

	//ファイルの出力
	void exportBvh(string filename);  //現在のbvhファイルをfilename.bvhに吐き出す
	void exportScaledOffset(string filename, float scale); //offsetの大きさを調整して出力(注意！！！！：Xrot,Yrot,Zrotの順番が違うよ)
	vector<vector<double>> exportAngular();

	void exportAngulartoCSV(string filename);  //各関節の角度をcsvファイルに出力
	void exportAngulartoTXT(string filename); //txtファイルに
	void exportAngularVelocity(string filename);  //各関節の角速度をcsvファイルに出力
	float getWeightEffort(); //bvhファイルのWeightEffortを返す関数
	float getVelAve(); //bvhファイルの各関節の位置速度を返す関数

	void childroop(const ofxBvhJoint* joint, string str); //export(filename)関数で使う
	void childroopScaledOffset(const ofxBvhJoint* joint, string str, float scale); //exportScaledOffset(string filename, float scale)関数で使う

	//リサンプリング
	void Resampling(const float frametime, string filename); //filename.bvhにリサンプリングしたbvhを吐き出す

	//補間
	void InterpolatePush(string filename, ofxDigitalDanceBvh* next, int range); //連結後のrangeフレーム分だけ線形補間で書き換え
	void InterpolateAttitude(string filename, ofxDigitalDanceBvh* next, int range); //間延びする補間
	void LerpBVH(string filename1, string filename2, int range); //セグメント間を線形補間して繋ぐ
	void LerpBVH(ofxDigitalDanceBvh* next, int range);
	void CubicInterpolateBVH(ofxDigitalDanceBvh* next, int range); //セグメント間を三次関数補間して繋ぐ
	float CubicInterpolate(float start, float stop, float amt) {
		return ((-2.0)*(stop - start)*amt*amt*amt) + (3.0*(stop - start)*amt*amt) + start;
	};

	//連結
	float getConnectivity(ofxDigitalDanceBvh* next); //this.bvhとnext.bvhの連結性評価関数の算出
	void FillBlank(int num_seg, ofxDigitalDanceBvh bvhs[], int arraysize, int start = 0, int goal = 1); //ノードの接続情報設定時に連結性の計算もランタイムで（めっちゃ遅い）
	void FillBlank2(int num_seg, ofxDigitalDanceBvh bvhs[], vector<vector<float>>& con_mat, int arraysize, int start = 0, int goal = 1); //ノードの接続情報設定時の，連結性の計算はほとんど事前計算で（ダイクストラ法）
																															 // 岡田さん
	void motionInitialize();
	void setTransRotate(ofVec3f root_pos, ofQuaternion *q);
	void exportFrameBVH(const string &filename, int start, int end, float scale);// 文字化け...
	void DFSJoint(ofxBvhJoint *joint, string blank, float scale);

	// bvhを分割する
	void segmentationBVH(string filename, int rows, double data[]);  //BVHファイルの分割(柿塚)
	void segmentationBVH_4C(string filename); //4カウント毎に分割（perfume用）

	// Sampling手法
	int GreedySampling(float q, float weighteffort[], float max_WE, vector<vector<float>>& D, int Sn[], int n, float rho = 1.0); // Douらの貪欲法によるサンプリング
	float rel_Music_WE(float q, float weighteffort, float max_WE); // 音楽とダンスの激しさの類似度
	float phi_DistanceScore(int bvh_path_num, vector<vector<float>>& D, int Sn[], int n); // 新たなサンプルとすでに選ばれたサンプルの距離の最小値を返す関数

	int mat_dim(vector<vector<float>>& con_mat);
	
private:
	ofstream ofs;
	ofstream out;	
};


//ノード
struct Node
{
	//直接接続されたエッジとノードの情報の情報
	std::vector<int> edges_to; //接続先のノード番号
	std::vector<double> edges_cost; //接続しているエッジのコスト

	//ダイクストラ法のためのデータ
	bool done; //確定ノードか否か
	double cost; //このノードへの現時点での最小コスト。-1のときはそのノードを未探索としています。
	int toGoal_num; ////ゴールへの最短ルートにつながるノード番号
};