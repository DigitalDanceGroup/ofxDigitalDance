#include "ofxDigitalDanceBvh.h"

using namespace mlib;

int ofxDigitalDanceBvh::getNumFrames()
{
    mNumFrames = this->num_frames;
    return mNumFrames;
}

int ofxDigitalDanceBvh::getFrameSize()
{
    return frames.size();
}

void ofxDigitalDanceBvh::update()
{
    frame_new = false;
    
    if (playing && ofGetFrameNum() > 1)
    {
        int last_index = getFrame();
        
        play_head += ofGetLastFrameTime() * rate;
        int index = getFrame();
        
        if (index != last_index)
        {
            need_update = true;
            
            if(index < frames.size())
                currentFrame = frames[index];
            
            if (index >= frames.size())
            {
                if (loop)
                    play_head = 0;
                else
                    playing = false;
            }
            
            if (play_head < 0)
                play_head = 0;
        }
    }
    
    if (need_update)
    {
        need_update = false;
        frame_new = true;
        
        int index = 0;
        updateJoint(index, currentFrame, root);
    }
}


void ofxDigitalDanceBvh::drawPerfume()
{
    for (int i = 0; i < joints.size(); i++)
    {
        ofxBvhJoint *o = joints[i];
        
        if (o->isSite())
        {
        }
        else if (o->getChildren().size() == 1)
        {
            if(i<6 || (i>7 && i<9) || i==13){
                ofSetColor(ofColor::red);
            }
            else if(i>16 && i<31){
                ofSetColor(ofColor::blue);
            }
            else{
                ofSetColor(ofColor(225, 165, 135));
            }
            
            //ofDrawLine(o->getPosition(), o->getChildren()[0]->getPosition());
            if(!o->getChildren()[0]->isSite()){
                if(i==9 || i==14){
                    ofSetColor(ofColor::red);
                }
                ofDrawSphere(o->getPosition().x, o->getPosition().y, o->getPosition().z, 4.0f);
            }
            
            if(i==1|| i==2 || i==3){
                ofSetColor(ofColor::red);
                this->drawElipsoid(o->getPosition(), o->getChildren()[0]->getPosition(), 0.8);
            }
            else
                this->drawElipsoid(o->getPosition(), o->getChildren()[0]->getPosition(), 0.3);
        }
        else if (o->getChildren().size() > 1)
        {
            if(i==0)
                ofSetColor(ofColor::blue);
            else
                ofSetColor(ofColor::red);
            
            
            ofDrawSphere(o->getPosition().x, o->getPosition().y, o->getPosition().z, 3.0f);
            
            for(int i=0; i<o->getChildren().size(); i++){
                //ofDrawLine(o->getPosition(), o->getChildren()[i]->getPosition());
                this->drawElipsoid(o->getPosition(), o->getChildren()[i]->getPosition(), 1.0);
            }
        }
        glPopMatrix();
    }
    
    ofSetColor(ofColor::white);
}

void ofxDigitalDanceBvh::draw()
{
    ofPushStyle();
    for (int i = 0; i < joints.size(); i++)
    {
        ofxBvhJoint *o = joints[i];
        
        if (o->isSite())
        {
        }
        else if (o->getChildren().size() == 1)
        {
            ofDrawLine(o->getPosition(), o->getChildren()[0]->getPosition());
        }
        else if (o->getChildren().size() > 1)
        {
            for(int i=0; i<o->getChildren().size(); i++){
                ofDrawLine(o->getPosition(), o->getChildren()[i]->getPosition());
            }
        }
        glPopMatrix();
    }
    ofPopStyle();
    ofSetColor(ofColor::white);
}

void ofxDigitalDanceBvh::drawElipsoid()
{
    for (int i = 0; i < joints.size(); i++)
    {
        ofxBvhJoint *o = joints[i];
        
        if (o->isSite())
        {
        }
        else if (o->getChildren().size() == 1)
        {
            this->drawElipsoid(o->getPosition(), o->getChildren()[0]->getPosition(), 0.3);
        }
        else if (o->getChildren().size() > 1)
        {
            for(int i=0; i<o->getChildren().size(); i++){
                this->drawElipsoid(o->getPosition(), o->getChildren()[i]->getPosition(), 0.5);
            }
        }
        glPopMatrix();
    }
    ofSetColor(ofColor::white);
}

float ofxDigitalDanceBvh::getFrameTime()
{
    return frame_time;
}

void ofxDigitalDanceBvh::setTransRotate(ofVec3f root_pos, ofQuaternion *q)
{
	FrameData f;
	f.push_back(root_pos.x);
	f.push_back(root_pos.y);
	f.push_back(root_pos.z);

	for (int i = 0; i<joints.size(); i++) {
		if (getJoint(i)->getName() != "Site") {
			ofMatrix4x4 mat = this->getJoint(i)->getMatrix();
			ofQuaternion qut;
			qut.set(mat);
			ofVec3f euler = qut.getEuler();
			f.push_back(euler.z);
			f.push_back(euler.x);
			f.push_back(euler.y);
		}
	}
	frames.push_back(f);
	this->num_frames = frames.size();
}

void ofxDigitalDanceBvh::motionInitialize()
{
	frames.clear();
}

void ofxDigitalDanceBvh::childroop(const ofxBvhJoint* joint, string str) {
	for (int c = 0; c < joint->getChildren().size(); c++) {
		// 各JOINTの情報を書く
		if (joint->getChildren().at(c)->getChildren().size() > 0) {
			// JOINTの行を書く
			ofs << str << "JOINT " << joint->getChildren().at(c)->getName() << endl;

			// "{"を書く
			ofs << str << "{" << endl;

			//スペースの数を増やす
			str += " ";

			// OFFSETの行を書く
			ofVec3f off = joint->getChildren().at(c)->getOffset();
			ofs << str << "OFFSET " << fixed << setprecision(6) << off.x << " " << off.y << " " << off.z << endl;

			// CHANNELSの行を書く
			ofs << str << "CHANNELS 3 Yrotation Xrotation Zrotation" << endl;

			// 再帰
			childroop(joint->getChildren().at(c), str);

			// space減らす
			str.erase(str.find_last_of(" "));
		}
		else {
			// ENDの行を書く
			ofs << str << "End " << joint->getChildren().at(c)->getName() << endl;

			// "{"を書く
			ofs << str << "{" << endl;

			//スペースの数を増やす
			string str2 = str + " ";

			// OFFSETの行を書く
			ofVec3f off = joint->getChildren().at(c)->getOffset();
			ofs << str2 << "OFFSET " << fixed << setprecision(6) << off.x << " " << off.y << " " << off.z << endl;
		}
		// 子がいなくなると "}" を書く
		ofs << str << "}" << endl;
	}
}


void ofxDigitalDanceBvh::exportBvh(string filename) {
	string str = " ";
	ofs.open(filename);
	//HIERARCHY
	ofs << "HIERARCHY" << endl;
	// ROOTの情報を書く
	ofs << "ROOT " << getJoint(0)->getName() << endl;
	ofs << "{" << endl;
	double roff = 0.000000;
	ofs << str << "OFFSET " << fixed << setprecision(6) << roff << " " << roff << " " << roff << endl;
	ofs << str << "CHANNELS 6 Xposition Yposition Zposition Yrotation Xrotation Zrotation" << endl;
	childroop(getJoint(0), str);
	ofs << "}" << endl;

	//MOTION
	ofs << "MOTION" << endl;
	ofs << "Frames: " << num_frames << endl;
	ofs << "Frame Time: " << fixed << setprecision(6) << frame_time << endl;

	for (int i = 0; i<this->num_frames; i++)
	{
		//this->update();
		int j = 0;
		for (j = 0; j<frames.at(i).size() - 1; j++) {
			ofs << frames.at(i).at(j) << " ";
		}
		ofs << fixed << frames.at(i).at(j) << std::endl;
	}

	ofs.close();
}


void ofxDigitalDanceBvh::childroopScaledOffset(const ofxBvhJoint* joint, string str, float scale) {
	for (int c = 0; c < joint->getChildren().size(); c++) {
		// 各JOINTの情報を書く
		if (joint->getChildren().at(c)->getChildren().size() > 0) {
			// JOINTの行を書く
			ofs << str << "JOINT " << joint->getChildren().at(c)->getName() << endl;

			// "{"を書く
			ofs << str << "{" << endl;

			//スペースの数を増やす
			str += " ";

			// OFFSETの行を書く
			ofVec3f off = scale * joint->getChildren().at(c)->getOffset();
			ofs << str << "OFFSET " << fixed << setprecision(6) << off.x << " " << off.y << " " << off.z << endl;

			// CHANNELSの行を書く
			ofs << str << "CHANNELS 3 Zrotation Yrotation Xrotation" << endl;

			// 再帰
			childroopScaledOffset(joint->getChildren().at(c), str, scale);

			// space減らす
			str.erase(str.find_last_of(" "));
		}
		else {
			// ENDの行を書く
			ofs << str << "End " << joint->getChildren().at(c)->getName() << endl;

			// "{"を書く
			ofs << str << "{" << endl;

			//スペースの数を増やす
			string str2 = str + " ";

			// OFFSETの行を書く
			ofVec3f off = scale * joint->getChildren().at(c)->getOffset();
			ofs << str2 << "OFFSET " << fixed << setprecision(6) << off.x << " " << off.y << " " << off.z << endl;
		}
		// 子がいなくなると "}" を書く
		ofs << str << "}" << endl;
	}
}

void ofxDigitalDanceBvh::exportScaledOffset(string filename, float scale) {
	string str = " ";
	ofs.open(filename);
	//HIERARCHY
	ofs << "HIERARCHY" << endl;
	// ROOTの情報を書く
	ofs << "ROOT " << getJoint(0)->getName() << endl;
	ofs << "{" << endl;
	double roff = 0.000000;
	ofs << str << "OFFSET " << fixed << setprecision(6) << roff << " " << roff << " " << roff << endl;
	ofs << str << "CHANNELS 6 Xposition Yposition Zposition Zrotation Yrotation Xrotation" << endl;
	childroopScaledOffset(getJoint(0), str, scale);
	ofs << "}" << endl;

	//MOTION
	ofs << "MOTION" << endl;
	ofs << "Frames: " << num_frames << endl;
	ofs << "Frame Time: " << fixed << setprecision(6) << frame_time << endl;

	for (int i = 0; i<this->num_frames; i++)
	{
		int j = 0;
		for (j = 0; j<frames.at(i).size() - 1; j++) {
			if (j < 3) {
				ofs << scale * frames.at(i).at(j) << " ";
			}
			else {
				ofs << frames.at(i).at(j) << " ";
			}
		}
		ofs << fixed << frames.at(i).at(j) << std::endl;
	}

	ofs.close();
}
//
//vector<vector<double>> ofxDigitalDanceBvh::exportAngular() {
//	vector<vector<double>> ang(this->getNumFrames(), vector<double>(27));
//	for (int i = 0; i<this->num_frames; i++)
//	{
//		int countend = 0;
//		int numj = 0;
//		for (int j = 0; j<this->getNumJoints(); j++) {
//			string name = this->getJoint(j)->getName();
//			if (name == "Hips" || name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" || name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip" || name == "LeftKnee") {
//				ang[i][3 * numj] = frames.at(i).at(3 + (3 * j) - (3 * countend));
//				ang[i][(3 * numj) + 1] = frames.at(i).at(4 + (3 * j) - (3 * countend));
//				ang[i][(3 * numj) + 2] = frames.at(i).at(5 + (3 * j) - (3 * countend));
//				numj++;
//			}
//			else if (name == "Site") {
//				countend++;
//			}
//		}
//	}
//	return ang;
//}

void ofxDigitalDanceBvh::exportAngulartoCSV(string filename) {
	ofs.open(filename);
	for (int i = 0; i<this->num_frames; i++)
	{
		int countend = 0;
		for (int j = 0; j<this->getNumJoints(); j++) {
			string name = this->getJoint(j)->getName();
			if (name == "Hips" || name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" || name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip") {
				ofs << frames.at(i).at(3 + (3 * j) - (3 * countend)) << ","
					<< frames.at(i).at(4 + (3 * j) - (3 * countend)) << ","
					<< frames.at(i).at(5 + (3 * j) - (3 * countend)) << ",";
			}
			else if (name == "LeftKnee") {
				ofs << frames.at(i).at(3 + (3 * j) - (3 * countend))
					<< "," << frames.at(i).at(4 + (3 * j) - (3 * countend))
					<< "," << frames.at(i).at(5 + (3 * j) - (3 * countend));
			}

			else if (name == "Site") {
				countend++;
			}
		}
		ofs << endl;
	}
	ofs.close();
}

void ofxDigitalDanceBvh::exportAngulartoTXT(string filename) {
	ofs.open(filename);
	for (int i = 0; i<this->num_frames; i++)
	{
		int countend = 0;
		for (int j = 0; j<this->getNumJoints(); j++) {
			string name = this->getJoint(j)->getName();
			if (name == "Hips" || name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" || name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip") {
				ofs << frames.at(i).at(3 + (3 * j) - (3 * countend)) / 10.0 << " "
					<< frames.at(i).at(4 + (3 * j) - (3 * countend)) / 10.0 << " "
					<< frames.at(i).at(5 + (3 * j) - (3 * countend)) / 10.0 << " ";
			}
			else if (name == "LeftKnee") {
				ofs << frames.at(i).at(3 + (3 * j) - (3 * countend))
					<< " " << frames.at(i).at(4 + (3 * j) - (3 * countend)) / 10.0
					<< " " << frames.at(i).at(5 + (3 * j) - (3 * countend)) / 10.0;
			}

			else if (name == "Site") {
				countend++;
			}
		}
		ofs << endl;
	}
	ofs.close();
}

void ofxDigitalDanceBvh::exportAngularVelocity(string filename) {
	ofs.open(filename);
	for (int i = 1; i<this->num_frames; i++)
	{
		int countend = 0;
		for (int j = 0; j<this->getNumJoints(); j++) {
			string name = this->getJoint(j)->getName();
			if (name == "Hips" || name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" || name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip" || name == "LeftKnee") {
				ofs << "," << abs(frames.at(i).at(3 + (3 * j) - (3 * countend)) - frames.at(i - 1).at(3 + (3 * j) - (3 * countend))) / this->frame_time
					<< "," << abs(frames.at(i).at(4 + (3 * j) - (3 * countend)) - frames.at(i - 1).at(4 + (3 * j) - (3 * countend))) / this->frame_time
					<< "," << abs(frames.at(i).at(5 + (3 * j) - (3 * countend)) - frames.at(i - 1).at(5 + (3 * j) - (3 * countend))) / this->frame_time;
			}
			else if (name == "Site") {
				countend++;
			}
		}
		ofs << endl;
	}
	ofs.close();
}

// 関節角度で激しさを計算
float ofxDigitalDanceBvh::getWeightEffort() {
	float weighteffort_total = 0.0;
	for (int i = 1; i<this->num_frames; i++)
	{
		int countend = 0; //bvhファイル中のEnd Siteの数をカウント
		int count_joint = 0; //WeightEffortに用いる関節数のカウント
		float weighteffort_total_frame = 0.0; // 各フレームごとのトータルWeightEffort

		for (int j = 0; j<this->getNumJoints(); j++) {
			string name = this->getJoint(j)->getName();

			if (/*name=="Hips"||*/name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" || name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip" || name == "LeftKnee") //WeightEffortに用いる関節
			{
				count_joint++;

				// 各関節ごとにy軸, x軸, z軸周りの回転
				weighteffort_total_frame += abs(frames.at(i).at(3 + (3 * j) - (3 * countend)) - frames.at(i - 1).at(3 + (3 * j) - (3 * countend))) / this->frame_time
					+ abs(frames.at(i).at(4 + (3 * j) - (3 * countend)) - frames.at(i - 1).at(4 + (3 * j) - (3 * countend))) / this->frame_time
					+ abs(frames.at(i).at(5 + (3 * j) - (3 * countend)) - frames.at(i - 1).at(5 + (3 * j) - (3 * countend))) / this->frame_time;
			}
			else if (name == "Site") {
				countend++;
			}
		}

		weighteffort_total = weighteffort_total_frame / (float)count_joint;
	}
	float weighteffort = weighteffort_total / (float)(this->num_frames - 1);
	return weighteffort;
}

// 関節位置の速さで激しさを計算
float ofxDigitalDanceBvh::getVelAve() {
	float vel_total = 0.0;
	for (int i = 1; i<this->num_frames; i++)
	{
		int num_joint = 0; //WeightEffortに用いる関節数のカウント
		float vel_total_frame = 0.0; // 各フレームごとのトータルWeightEffort

		for (int j = 0; j<this->getNumJoints(); j++) {
			string name = this->getJoint(j)->getName();
			if (/*name=="Hips"||*/name == "RightShoulder" || name == "RightElbow" || name == "LeftShoulder" || name == "LeftElbow" || name == "RightHip" || name == "RightKnee" || name == "LeftHip" || name == "LeftKnee") //WeightEffortに用いる関節
			{
				// １フレーム間の関節速度（位置座標）
				setFrame(i - 1);
				update();
				ofVec3f disp = this->getJoint(j + 1)->getPosition();
				setFrame(i);
				update();
				disp = this->getJoint(j + 1)->getPosition() - disp;
				vel_total_frame += disp.length() / this->frame_time;
				num_joint++;
			}
		}
		vel_total += vel_total_frame / (float)num_joint;
	}
	float vel_ave = vel_total / (float)(this->num_frames - 1);
	return vel_ave;
}

// リサンプリング
void ofxDigitalDanceBvh::Resampling(const float frametime, string filename) {
	string str = " ";
	ofs.open(filename);
	//HIERARCHY
	ofs << "HIERARCHY" << endl;
	// ROOTの情報を書く
	ofs << "ROOT " << getJoint(0)->getName() << endl;
	ofs << "{" << endl;
	double roff = 0.000000;
	ofs << str << "OFFSET " << fixed << setprecision(6) << roff << " " << roff << " " << roff << endl;
	ofs << str << "CHANNELS 6 Xposition Yposition Zposition Yrotation Xrotation Zrotation" << endl;
	childroop(getJoint(0), str);
	ofs << "}" << endl;

	//MOTION
	ofs << "MOTION" << endl;
	float endtime = (num_frames - 1) * frame_time;  //開始フレームから終了フレームまでの時間
	const int numframes = 1 + (int)(endtime / frametime);  //リサンプリング後のフレーム数
	ofs << "Frames: " << numframes << endl;
	ofs << "Frame Time: " << fixed << setprecision(6) << frametime << endl;

	for (int i = 0; i<numframes; i++)
	{
		float t = (float)i * frametime;
		if (t >= endtime) {
			int y_start = num_frames - 2;
			int y_stop = num_frames - 1;
			float t_start = y_start * frame_time;
			float t_stop = t_start + frame_time;
			float amt = (t - t_start) / (t_stop - t_start);
			int j = 0;
			for (j = 0; j<frames.at(0).size() - 1; j++) {
				if (j == 0 || j == 1 || j == 2) {
					float y = ofLerp(frames.at(y_start).at(j), frames.at(y_stop).at(j), amt);
					ofs << y << " ";
				}
				else {
					if (abs(frames.at(y_start).at(j) - frames.at(y_stop).at(j)) > 180.0) { //[-180,180]から[0,360]に変換してから補間
						if (frames.at(y_start).at(j) < 0.0) {
							float newstart = 360.0 + frames.at(y_start).at(j);
							float y = ofLerp(newstart, frames.at(y_stop).at(j), amt);
							if (y > 180.0) { //[-180,180]に戻す
								y = y - 360.0;
							}

							if (j == frames.at(0).size() - 1) {
								ofs << y << std::endl;
							}
							else {
								ofs << y << " ";
							}
						}
						else /*if(frames.at(y_stop).at(j) < 0)*/ {
							float newend = 360.0 + frames.at(y_stop).at(j);
							float y = ofLerp(frames.at(y_start).at(j), newend, amt);
							if (y > 180.0) {
								y = y - 360.0;
							}
							if (j == frames.at(0).size() - 1) {
								ofs << y << std::endl;
							}
							else {
								ofs << y << " ";
							}
						}
					}
					else {
						float y = ofLerp(frames.at(y_start).at(j), frames.at(y_stop).at(j), amt);
						if (j == frames.at(0).size() - 1) {
							ofs << y << std::endl;
						}
						else {
							ofs << y << " ";
						}
					}
				}
			}
		}
		else {
			int y_start = (int)(t / frame_time);
			int y_stop = y_start + 1;
			float t_start = y_start * frame_time;
			float t_stop = t_start + frame_time;
			float amt = (t - t_start) / (t_stop - t_start);
			int j = 0;
			for (j = 0; j<frames.at(0).size(); j++) {
				if (j == 0 || j == 1 || j == 2) {
					float y = ofLerp(frames.at(y_start).at(j), frames.at(y_stop).at(j), amt);
					ofs << y << " ";
				}
				else {
					if (abs(frames.at(y_start).at(j) - frames.at(y_stop).at(j)) > 180.0) { //[-180,180]から[0,360]に変換してから補間
						if (frames.at(y_start).at(j) < 0.0) {
							float newstart = 360.0 + frames.at(y_start).at(j);
							float y = ofLerp(newstart, frames.at(y_stop).at(j), amt);
							if (y > 180.0) { //[-180,180]に戻す
								y = y - 360.0;
							}

							if (j == frames.at(0).size() - 1) {
								ofs << y << std::endl;
							}
							else {
								ofs << y << " ";
							}
						}
						else /*if(frames.at(y_stop).at(j) < 0)*/ {
							float newend = 360.0 + frames.at(y_stop).at(j);
							float y = ofLerp(frames.at(y_start).at(j), newend, amt);
							if (y > 180.0) {
								y = y - 360.0;
							}
							if (j == frames.at(0).size() - 1) {
								ofs << y << std::endl;
							}
							else {
								ofs << y << " ";
							}
						}
					}
					else {
						float y = ofLerp(frames.at(y_start).at(j), frames.at(y_stop).at(j), amt);
						if (j == frames.at(0).size() - 1) {
							ofs << y << std::endl;
						}
						else {
							ofs << y << " ";
						}
					}
				}
			}
		}
	}

	ofs.close();
}

void ofxDigitalDanceBvh::InterpolatePush(string filename, ofxDigitalDanceBvh* next, int range) {
	string str = " ";
	ofs.open(filename);
	//HIERARCHY
	ofs << "HIERARCHY" << endl;
	// ROOTの情報を書く
	ofs << "ROOT " << getJoint(0)->getName() << endl;
	ofs << "{" << endl;
	double roff = 0.000000;
	ofs << str << "OFFSET " << roff << " " << roff << " " << roff << endl;
	ofs << str << "CHANNELS 6 Xposition Yposition Zposition Yrotation Xrotation Zrotation" << endl;
	childroop(getJoint(0), str);
	ofs << "}" << endl;

	//MOTION
	ofs << "MOTION" << endl;
	ofs << "Frames: " << this->num_frames + next->num_frames << endl;
	ofs << "Frame Time: " << fixed << setprecision(6) << frame_time << endl;

	for (int i = 0; i<this->num_frames; i++)
	{
		int j = 0;
		for (j = 0; j<this->frames.at(i).size() - 1; j++) {
			ofs << this->frames.at(i).at(j) << " ";
		}
		ofs << fixed << this->frames.at(i).at(j) << std::endl;
	}

	ofs.close();

	ofs.open(filename, std::ios::app);
	// framesの更新
	/*ifstream ifs(filename);
	int newframes = this->num_frames + next->num_frames;
	static const int lineno = 138;
	int cnt = 0;
	string line;
	while(getline(ifs,line)){
	if(++cnt==lineno) {
	ofs << "Frames: " << newframes << endl;
	}
	else {
	ofs << line << endl;
	}
	}
	ifs.close();*/
	//this->update();
	//this->play();
	for (int i = 0; i < next->num_frames; i++)
	{
		//this->update();
		// range内ではx,zの位置座標補正と、その他のすべてのパラメータを線形補間
		if (i < range - 1) {
			int j = 0;
			for (j = 0; j < next->frames.at(i).size() - 1; j++) {
				if (j == 0 || j == 2) {
					// x,zの位置座標はすべてのフレームで変換
					ofs << next->frames.at(i).at(j) - (next->frames.at(0).at(j) - this->frames.at(num_frames - 1).at(j)) << " ";
				}
				else {
					// その他のパラメータを線形補間
					ofs << ofLerp(this->frames.at(this->num_frames - 1).at(j), next->frames.at(range - 1).at(j), ((float)i + 1) / range) << " ";
				}
			}
			ofs << fixed << ofLerp(this->frames.at(this->num_frames - 1).at(j), next->frames.at(range - 1).at(j), ((float)i + 1) / range)
				<< std::endl;
		}

		// range外でもx,zの位置座標補正はするが、その他のすべてのパラメータはそのままの値
		else {
			int j = 0;
			for (j = 0; j<frames.at(i).size() - 1; j++) {
				if (j == 0 || j == 1 || j == 2) {
					// x,zの位置座標はすべてのフレームで変換
					ofs << next->frames.at(i).at(j) - (next->frames.at(0).at(j) - this->frames.at(num_frames - 1).at(j)) << " ";
				}
				else {
					// その他のパラメータはそのまま
					ofs << next->frames.at(i).at(j) << " ";
				}
			}
			ofs << fixed << next->frames.at(i).at(j) << std::endl;
		}
	}
	ofs.close();

	this->unload();
	this->load(filename);
}

void ofxDigitalDanceBvh::InterpolateAttitude(string filename, ofxDigitalDanceBvh* next, int range) {
	string str = " ";
	ofs.open(filename);
	// HIERARCHY
	ofs << "HIERARCHY" << endl;
	// ROOTの情報を書く
	ofs << "ROOT " << getJoint(0)->getName() << endl;
	ofs << "{" << endl;
	double roff = 0.000000;
	ofs << str << "OFFSET " << roff << " " << roff << " " << roff << endl;
	ofs << str << "CHANNELS 6 Xposition Yposition Zposition Yrotation Xrotation Zrotation" << endl;
	childroop(getJoint(0), str);
	ofs << "}" << endl;

	// MOTION
	ofs << "MOTION" << endl;
	ofs << "Frames: " << this->num_frames + range + next->num_frames << endl;
	ofs << "Frame Time: " << fixed << setprecision(6) << frame_time << endl;

	for (int i = 0; i<this->num_frames; i++)
	{
		int j = 0;
		for (j = 0; j<this->frames.at(i).size() - 1; j++) {
			ofs << this->frames.at(i).at(j) << " ";
		}
		ofs << fixed << this->frames.at(i).at(j) << std::endl;
	}

	ofs.close();

	ofs.open(filename, std::ios::app);

	for (int i = 0; i < range; i++)
	{
		//各パラメータを線形補間
		int j = 0;
		for (j = 0; j < frames.at(i).size() - 1; j++) {
			// その他のパラメータを線形補間
			ofs << this->frames.at(this->num_frames - 1).at(j) + (next->frames.at(1).at(j) - this->frames.at(this->num_frames - 1).at(j)) * (((float)i + 1) / range) << " ";
		}
		ofs << fixed << ofLerp(this->frames.at(this->num_frames - 1).at(j), next->frames.at(1).at(j), ((float)i + 1) / range)
			<< std::endl;

	}

	// nextセグメントのフレーム情報を書き足す
	for (int i = 0; i<next->num_frames; i++)
	{
		int j = 0;
		for (j = 0; j<next->frames.at(i).size() - 1; j++) {
			ofs << next->frames.at(i).at(j) << " ";
		}
		ofs << fixed << next->frames.at(i).at(j) << std::endl;
	}
	ofs.close();

	this->unload();
	this->load(filename);
}

void ofxDigitalDanceBvh::LerpBVH(string filename1, string filename2, int range) {

	ofxDigitalDanceBvh bvh1, bvh2; //bvhはbvh1とbvh2を補間して連結したもの
	bvh1.load(filename1);
	bvh2.load(filename2);
	this->load(filename1);

	//連結部の前rangeフレーム分の書き換え
	for (int i = 0; i < range - 1; i++) {
		for (int j = 0; j < this->frames.at(i).size(); j++) {
			if (j == 0 || j == 2) {
				continue;
			}
			else {
				//その他のパラメータを線形補間
				if (abs(bvh1.frames.at(bvh1.num_frames - range).at(j) - bvh2.frames.at(range - 1).at(j)) > 180.0) { //[-180, 180]を[0, 360]に変換してから補間
					if (bvh1.frames.at(bvh1.num_frames - range).at(j) < 0.0) {
						float newstart = 360.0 + bvh1.frames.at(bvh1.num_frames - range).at(j);
						float v = ofLerp(newstart, bvh2.frames.at(range - 1).at(j), ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
						if (v > 180.0) {
							v = v - 360.0;
						}
						this->frames.at((bvh1.num_frames - 1) - i).at(j) = v;
					}
					else/* if(bvh2.frames.at(range-1).at(j) < 0)*/ {
						float newend = 360.0 + bvh2.frames.at(range - 1).at(j);
						float v = ofLerp(bvh1.frames.at(bvh1.num_frames - range).at(j), newend, ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
						if (v > 180.0) {
							v = v - 360.0;
						}
						this->frames.at((bvh1.num_frames - 1) - i).at(j) = v;
					}
				}
				else {
					float v = ofLerp(bvh1.frames.at(bvh1.num_frames - range).at(j), bvh2.frames.at(range - 1).at(j), ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
					this->frames.at((bvh1.num_frames - 1) - i).at(j) = v;
				}
			}
		}
	}

	//連結部の後rangeフレーム分の書き換え
	for (int i = 0; i < bvh2.num_frames; i++)
	{
		FrameData data; //bvhの((bvh1のフレーム数)+(i+1))番目のフレームのデータ
						// range内ではx,zの位置座標補正と、その他のすべてのパラメータを線形補間
		if (i < range - 1) {
			int j = 0;
			for (j = 0; j < bvh2.frames.at(i).size() - 1; j++) {
				if (j == 0 || j == 2) {
					// x, zの位置座標はすべてのフレームで変換
					float v = bvh2.frames.at(i).at(j) - (bvh2.frames.at(0).at(j) - this->frames.at(bvh1.num_frames - 1).at(j));
					data.push_back(v);
				}
				else {
					// その他のパラメータを線形補間
					// MOTIONのパラメータは[-180, 180]degreeなので補正が必要
					if (abs(bvh1.frames.at(bvh1.num_frames - range).at(j) - bvh2.frames.at(range - 1).at(j)) > 180.0) {
						if (bvh1.frames.at(bvh1.num_frames - range).at(j) < 0.0) {
							float newstart = 360.0 + bvh1.frames.at(bvh1.num_frames - range).at(j);
							float v = ofLerp(newstart, bvh2.frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
							if (v > 180.0) {
								v = v - 360.0;
							}
							data.push_back(v);
						}
						else/* if(bvh2.frames.at(range-1).at(j) < 0)*/ {
							float newend = 360.0 + bvh2.frames.at(range - 1).at(j);
							float v = ofLerp(bvh1.frames.at(bvh1.num_frames - range).at(j), newend, ((float)range + (float)i) / (((float)range*2.0) - 1.0));
							if (v > 180.0) {
								v = v - 360.0;
							}
							data.push_back(v);
						}
					}
					else {
						float v = ofLerp(bvh1.frames.at(bvh1.num_frames - range).at(j), bvh2.frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
						data.push_back(v);
					}
				}
			}
			float v = ofLerp(bvh1.frames.at(bvh1.num_frames - range).at(j), bvh2.frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
			data.push_back(v);
		}

		// range外でもx,zの位置座標補正はするが、その他のすべてのパラメータはそのままの値
		else {
			int j = 0;
			for (j = 0; j<bvh2.frames.at(i).size() - 1; j++) {
				if (j == 0/*||j==1*/ || j == 2) {
					// x,zの位置座標はすべてのフレームで変換
					float v = bvh2.frames.at(i).at(j) - (bvh2.frames.at(0).at(j) - this->frames.at(bvh1.num_frames - 1).at(j));
					data.push_back(v);
				}
				else {
					// その他のパラメータはそのまま
					float v = bvh2.frames.at(i).at(j);
					data.push_back(v);
				}
			}
			float v = bvh2.frames.at(i).at(j);
			data.push_back(v);
		}
		frames.push_back(data);
		this->num_frames++;
	}
}

void ofxDigitalDanceBvh::LerpBVH(ofxDigitalDanceBvh* next, int range) {
	int ori_num_frames = this->num_frames;

	//連結部の前rangeフレーム分の書き換え
	for (int i = 0; i < range - 1; i++) {
		for (int j = 0; j < this->frames.at(i).size(); j++) {
			if (j == 0 || j == 2) {
				continue;
			}
			else {
				//その他のパラメータを線形補間
				ofxDigitalDanceBvh bvhh;
				bvhh.setFrame(10);
				if (abs(this->frames.at(ori_num_frames - range).at(j) - next->frames.at(range - 1).at(j)) > 180.0) {
					if (this->frames.at(ori_num_frames - range).at(j) < 0.0) {
						float newstart = 360.0 + this->frames.at(ori_num_frames - range).at(j);
						float v = ofLerp(newstart, next->frames.at(range - 1).at(j), ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
						if (v > 180.0) {
							v = v - 360.0;
						}
						this->frames.at((this->num_frames - 1) - i).at(j) = v;
					}
					else/* if(next->frames.at(range-1).at(j) < 0)*/ {
						float newend = 360.0 + next->frames.at(range - 1).at(j);
						float v = ofLerp(this->frames.at(ori_num_frames - range).at(j), newend, ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
						if (v > 180.0) {
							v = v - 360.0;
						}
						this->frames.at((this->num_frames - 1) - i).at(j) = v;
					}
				}
				else {
					float v = ofLerp(this->frames.at(ori_num_frames - range).at(j), next->frames.at(range - 1).at(j), ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
					this->frames.at((this->num_frames - 1) - i).at(j) = v;
				}
			}
		}
	}

	//連結部の後rangeフレーム分の書き換え
	for (int i = 0; i < next->num_frames; i++)
	{
		FrameData data; //bvhの((bvh1のフレーム数)+(i+1))番目のフレームのデータ
						// range内ではx,zの位置座標補正と、その他のすべてのパラメータを線形補間
		if (i < range - 1) {
			int j = 0;
			for (j = 0; j < next->frames.at(i).size() - 1; j++) {
				if (j == 0 || j == 2) {
					// x, zの位置座標はすべてのフレームで変換
					float v = next->frames.at(i).at(j) - (next->frames.at(0).at(j) - this->frames.at(ori_num_frames - 1).at(j));
					data.push_back(v);
				}
				else {
					// その他のパラメータを線形補間
					// MOTIONのパラメータは[-180, 180]degreeなので補正が必要
					if (abs(this->frames.at(ori_num_frames - range).at(j) - next->frames.at(range - 1).at(j)) > 180.0) {
						if (this->frames.at(ori_num_frames - range).at(j) < 0.0) {
							float newstart = 360.0 + this->frames.at(ori_num_frames - range).at(j);
							float v = ofLerp(newstart, next->frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
							if (v > 180.0) {
								v = v - 360.0;
							}
							data.push_back(v);
						}
						else/* if(next->frames.at(range-1).at(j) < 0)*/ {
							float newend = 360.0 + next->frames.at(range - 1).at(j);
							float v = ofLerp(this->frames.at(ori_num_frames - range).at(j), newend, ((float)range + (float)i) / (((float)range*2.0) - 1.0));
							if (v > 180.0) {
								v = v - 360.0;
							}
							data.push_back(v);
						}
					}
					else {
						float v = ofLerp(this->frames.at(ori_num_frames - range).at(j), next->frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
						data.push_back(v);
					}
				}
			}
			float v = ofLerp(this->frames.at(ori_num_frames - range).at(j), next->frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
			data.push_back(v);
		}

		// range外でもx,zの位置座標補正はするが、その他のすべてのパラメータはそのままの値
		else {
			int j = 0;
			for (j = 0; j<next->frames.at(i).size() - 1; j++) {
				if (j == 0/*||j==1*/ || j == 2) {
					// x,zの位置座標はすべてのフレームで変換
					float v = next->frames.at(i).at(j) - (next->frames.at(0).at(j) - this->frames.at(ori_num_frames - 1).at(j));
					data.push_back(v);
				}
				else {
					// その他のパラメータはそのまま
					float v = next->frames.at(i).at(j);
					data.push_back(v);
				}
			}
			float v = next->frames.at(i).at(j);
			data.push_back(v);
		}
		frames.push_back(data);
		this->num_frames++;
	}
}

void ofxDigitalDanceBvh::CubicInterpolateBVH(ofxDigitalDanceBvh* next, int range) {
	int ori_num_frames = this->num_frames;

	//連結部の前rangeフレーム分の書き換え
	for (int i = 0; i < range - 1; i++) {
		for (int j = 0; j < this->frames.at(i).size(); j++) {
			if (j == 0 || j == 2) {
				continue;
			}
			else {
				//その他のパラメータを線形補間
				if (abs(this->frames.at(ori_num_frames - range).at(j) - next->frames.at(range - 1).at(j)) > 180.0) {
					if (this->frames.at(ori_num_frames - range).at(j) < 0.0) {
						float newstart = 360.0 + this->frames.at(ori_num_frames - range).at(j);
						float v = CubicInterpolate(newstart, next->frames.at(range - 1).at(j), ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
						if (v > 180.0) {
							v = v - 360.0;
						}
						this->frames.at((this->num_frames - 1) - i).at(j) = v;
					}
					else/* if(next->frames.at(range-1).at(j) < 0)*/ {
						float newend = 360.0 + next->frames.at(range - 1).at(j);
						float v = CubicInterpolate(this->frames.at(ori_num_frames - range).at(j), newend, ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
						if (v > 180.0) {
							v = v - 360.0;
						}
						this->frames.at((this->num_frames - 1) - i).at(j) = v;
					}
				}
				else {
					float v = CubicInterpolate(this->frames.at(ori_num_frames - range).at(j), next->frames.at(range - 1).at(j), ((float)range - (float)i - 1.0) / (((float)range*2.0) - 1.0));
					this->frames.at((this->num_frames - 1) - i).at(j) = v;
				}
			}
		}
	}

	//連結部の後rangeフレーム分の書き換え
	for (int i = 0; i < next->num_frames; i++)
	{
		FrameData data; //bvhの((bvh1のフレーム数)+(i+1))番目のフレームのデータ
						// range内ではx,zの位置座標補正と、その他のすべてのパラメータを線形補間
		if (i < range - 1) {
			int j = 0;
			for (j = 0; j < next->frames.at(i).size() - 1; j++) {
				if (j == 0 || j == 2) {
					// x, zの位置座標はすべてのフレームで変換
					float v = next->frames.at(i).at(j) - (next->frames.at(0).at(j) - this->frames.at(ori_num_frames - 1).at(j));
					data.push_back(v);
				}
				else {
					// その他のパラメータを線形補間
					// MOTIONのパラメータは[-180, 180]degreeなので補正が必要
					if (abs(this->frames.at(ori_num_frames - range).at(j) - next->frames.at(range - 1).at(j)) > 180.0) {
						if (this->frames.at(ori_num_frames - range).at(j) < 0.0) {
							float newstart = 360.0 + this->frames.at(ori_num_frames - range).at(j);
							float v = CubicInterpolate(newstart, next->frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
							if (v > 180.0) {
								v = v - 360.0;
							}
							data.push_back(v);
						}
						else/* if(next->frames.at(range-1).at(j) < 0)*/ {
							float newend = 360.0 + next->frames.at(range - 1).at(j);
							float v = CubicInterpolate(this->frames.at(ori_num_frames - range).at(j), newend, ((float)range + (float)i) / (((float)range*2.0) - 1.0));
							if (v > 180.0) {
								v = v - 360.0;
							}
							data.push_back(v);
						}
					}
					else {
						float v = CubicInterpolate(this->frames.at(ori_num_frames - range).at(j), next->frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
						data.push_back(v);
					}
				}
			}
			float v = CubicInterpolate(this->frames.at(ori_num_frames - range).at(j), next->frames.at(range - 1).at(j), ((float)range + (float)i) / (((float)range*2.0) - 1.0));
			data.push_back(v);
		}

		// range外でもx,zの位置座標補正はするが、その他のすべてのパラメータはそのままの値
		else {
			int j = 0;
			for (j = 0; j<next->frames.at(i).size() - 1; j++) {
				if (j == 0/*||j==1*/ || j == 2) {
					// x,zの位置座標はすべてのフレームで変換
					float v = next->frames.at(i).at(j) - (next->frames.at(0).at(j) - this->frames.at(ori_num_frames - 1).at(j));
					data.push_back(v);
				}
				else {
					// その他のパラメータはそのまま
					float v = next->frames.at(i).at(j);
					data.push_back(v);
				}
			}
			float v = next->frames.at(i).at(j);
			data.push_back(v);
		}
		frames.push_back(data);
		this->num_frames++;
	}
}



float ofxDigitalDanceBvh::getConnectivity(ofxDigitalDanceBvh* next) {
	float Sim_pose = 0.0; //姿勢類似度
	float Sim_move = 0.0; //動き類似度

	for (int j = 0; j<this->getNumJoints(); j++) {
		string name_this = this->getJoint(j)->getName();

		if (name_this == "Hips" || name_this == "RightShoulder" || name_this == "RightElbow" || name_this == "LeftShoulder" || name_this == "LeftElbow" || name_this == "RightHip" || name_this == "RightKnee" || name_this == "LeftHip" || name_this == "LeftKnee") //用いる関節の指定
		{
			// 前セグメントの最終フレームにセット
			this->setFrame(this->num_frames - 1);
			this->update();
			// 後セグメントの開始フレームにセット
			next->setFrame(0);
			next->update();

			// セグメント間の連結部前後1フレームの各リンクの方向ベクトル
			ofVec3f link_this1 = this->getJoint(j + 1)->getPosition() - this->getJoint(j)->getPosition();
			ofVec3f link_next1 = next->getJoint(j + 1)->getPosition() - next->getJoint(j)->getPosition();

			// 姿勢類似度Sim_poseにセグメント間の連結部前後1フレームのリンクの方向ベクトルの内積をSim_poseに足す
			Sim_pose += link_this1.getNormalized().dot(link_next1.getNormalized()) / 9.0f;

			// 前セグメントの最終フレーム-1にセット
			this->setFrame(this->num_frames - 2);
			this->update();
			// 後セグメントの開始フレーム+1にセット
			next->setFrame(1);
			next->update();

			// セグメント間の連結部前後2フレーム目の各リンクの方向ベクトル
			ofVec3f link_this2 = this->getJoint(j + 1)->getPosition() - this->getJoint(j)->getPosition();
			ofVec3f link_next2 = next->getJoint(j + 1)->getPosition() - next->getJoint(j)->getPosition();

			ofVec3f prev_vel = (link_this1 - link_this2).getNormalized(); // 前セグメントの最終2フレームの速度（正規化）
			ofVec3f next_vel = (link_next2 - link_next1).getNormalized(); // 後セグメントの開始2フレームの速度（正規化）
			ofVec3f seam_vel = (link_next1 - link_this1).getNormalized(); // 連結部の速度（正規化）

																		  // 動き類似度Sim_move
			float g1 = seam_vel.dot(prev_vel);
			if (g1 < 0.0) {
				g1 = 0.0;
			}
			float g2 = seam_vel.dot(next_vel);
			if (g2 < 0.0) {
				g2 = 0.0;
			}
			Sim_move += (g1 * g2) / 9.0f;
		}
	}
	return Sim_pose + Sim_move;
}



void ofxDigitalDanceBvh::FillBlank(int num_seg, ofxDigitalDanceBvh bvhs[], int arraysize, int start, int goal) {

	cout << "ノードの設定中" << endl;

	int size = (arraysize - 2) * num_seg; //全ノードの数-2
	int size_interseg = arraysize - 2; //bvhsの数-2
	std::vector<Node> nodes;
	nodes.resize(size + 2);

	//全ノードの接続情報
	for (int i = 0; i < nodes.size(); i++)
	{
		if (i == start) {
			continue;
		}
		else if (i == goal) {
			nodes[i].edges_to.resize(size_interseg);
			nodes[i].edges_cost.resize(size_interseg);
			for (int j = 0; j < nodes[i].edges_to.size(); j++) {
				nodes[i].edges_to[j] = j + 2;
				//double kost = 1.0 / bvhs[j+2].getConnectivity(&bvhs[i]);
				nodes[i].edges_cost[j] = 1.0 / bvhs[j + 2].getConnectivity(&bvhs[i]);
			}
		}
		else if ((i - 2) / size_interseg == (num_seg - 1)) {
			nodes[i].edges_to.resize(1);
			nodes[i].edges_cost.resize(1);
			nodes[i].edges_to[0] = start;
			nodes[i].edges_cost[0] = 1.0 / bvhs[start].getConnectivity(&bvhs[((i - 2) % size_interseg) + 2]);
		}
		else {
			nodes[i].edges_to.resize(size_interseg);
			nodes[i].edges_cost.resize(size_interseg);
			for (int j = 0; j < nodes[i].edges_to.size(); j++) {
				nodes[i].edges_to[j] = ((((i - 2) / size_interseg) + 1) * size_interseg) + j + 2;
				/*double kost0 = bvhs[2].getConnectivity(&bvhs[2] );
				double kost = 1.0 / bvhs[j+2].getConnectivity(&bvhs[((i-2) % size_interseg) + 2 ] );*/
				if (j + 2 == ((i - 2) % size_interseg) + 2) {
					nodes[i].edges_cost[j] = 10.0;
				}
				else {
					nodes[i].edges_cost[j] = 1.0 / bvhs[j + 2].getConnectivity(&bvhs[((i - 2) % size_interseg) + 2]);
				}
			}
		}
	}

	//全ノードの初期化
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].done = false;
		nodes[i].cost = -1.0;
	}

	//goalから探索します
	nodes[goal].cost = 0.0; //探索初めのノードのコストは0

							//アルゴリズム実行
	while (true)
	{
		//確定ノードを探す
		Node* doneNode = NULL; //確定ノードにかえるための
		int doneNode_num = NULL;
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].done || nodes[i].cost < 0)
			{
				continue;
			}
			if (doneNode == NULL || nodes[i].cost < doneNode->cost)
			{
				doneNode = &nodes[i];
				doneNode_num = i;
			}
		}
		//未確定ノードがなくなるか、Startが確定ノードになれば終了
		if (doneNode == NULL || nodes[start].done)
		{
			break;
		}

		//確定フラグを立てる
		doneNode->done = true;  //これで参照先の値を書き換え
								//接続先ノードの情報を更新する
		for (int i = 0; i < doneNode->edges_to.size(); i++)
		{
			int to = doneNode->edges_to[i];
			double cost1 = nodes[1].edges_cost[i];
			double cost2 = doneNode->edges_cost[i];
			double cost = doneNode->cost + doneNode->edges_cost[i];
			if (nodes[to].cost < 0 || cost < doneNode->edges_cost[i])
			{
				nodes[to].cost = cost;
				nodes[to].toGoal_num = doneNode_num;
			}
		}
	}

	//startから最短経路をたどってgoalまで
	int next = nodes[start].toGoal_num;
	this->CubicInterpolateBVH(&bvhs[((next - 2) % size_interseg) + 2], 5);
	//cout << ((next-2) % size_interseg) + 2 << endl;
	for (int i = 0; i < num_seg - 1; i++) {
		next = nodes[next].toGoal_num;
		this->CubicInterpolateBVH(&bvhs[((next - 2) % size_interseg) + 2], 5);
		//cout << ((next-2) % size_interseg) + 2 << endl;
	}
	this->CubicInterpolateBVH(&bvhs[goal], 5);
}

void ofxDigitalDanceBvh::FillBlank2(int num_seg, ofxDigitalDanceBvh bvhs[], vector<vector<float>>& con_mat, int arraysize, int start, int goal) {

	int size = (arraysize - 2) * num_seg; //全ノードの数-2
	int size_interseg = arraysize - 2; //bvhsの数-2
	std::vector<Node> nodes;
	nodes.resize(size + 2);

	//全ノードの接続情報
	for (int i = 0; i < nodes.size(); i++)
	{
		if (i == start) {
			continue;
		}
		else if (i == goal) {
			nodes[i].edges_to.resize(size_interseg);
			nodes[i].edges_cost.resize(size_interseg);
			for (int j = 0; j < nodes[i].edges_to.size(); j++) {
				nodes[i].edges_to[j] = j + 2;
				nodes[i].edges_cost[j] = 1.0 / bvhs[j + 2].getConnectivity(&bvhs[i]);
				//nodes[i].edges_cost[j] = con_mat[j+2][i];
			}
		}
		else if ((i - 2) / size_interseg == (num_seg - 1)) {
			nodes[i].edges_to.resize(1);
			nodes[i].edges_cost.resize(1);
			nodes[i].edges_to[0] = start;
			nodes[i].edges_cost[0] = 1.0 / bvhs[start].getConnectivity(&bvhs[((i - 2) % size_interseg) + 2]);
			//nodes[i].edges_cost[0] = con_mat[start][((i-2) % size_interseg) + 2];
		}
		else {
			nodes[i].edges_to.resize(size_interseg);
			nodes[i].edges_cost.resize(size_interseg);
			for (int j = 0; j < nodes[i].edges_to.size(); j++) {
				nodes[i].edges_to[j] = ((((i - 2) / size_interseg) + 1) * size_interseg) + j + 2;
				/*double kost0 = bvhs[2].getConnectivity(&bvhs[2] );
				double kost = 1.0 / bvhs[j+2].getConnectivity(&bvhs[((i-2) % size_interseg) + 2 ] );*/
				if (j + 2 == ((i - 2) % size_interseg) + 2) {
					nodes[i].edges_cost[j] = 100.0;
				}
				else {
					nodes[i].edges_cost[j] = con_mat[j + 2][((i - 2) % size_interseg) + 2];
				}
			}
		}
	}

	//全ノードの初期化
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].done = false;
		nodes[i].cost = -1.0;
	}

	cout << "経路探索中" << endl;
	//goalから探索します
	nodes[goal].cost = 0.0; //探索初めのノードのコストは0

							//アルゴリズム実行
	while (true)
	{
		//確定ノードを探す
		Node* doneNode = NULL; //確定ノードにかえるための
		int doneNode_num = NULL;
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].done || nodes[i].cost < 0)
			{
				continue;
			}
			if (doneNode == NULL || nodes[i].cost < doneNode->cost)
			{
				doneNode = &nodes[i];
				doneNode_num = i;
			}
		}
		//未確定ノードがなくなるか、Startが確定ノードになれば終了
		if (doneNode == NULL || nodes[start].done)
		{
			break;
		}

		//確定フラグを立てる
		doneNode->done = true;  //これで参照先の値を書き換え
								//接続先ノードの情報を更新する
		for (int i = 0; i < doneNode->edges_to.size(); i++)
		{
			int to = doneNode->edges_to[i];
			double cost1 = nodes[1].edges_cost[i];
			double cost2 = doneNode->edges_cost[i];
			double cost = doneNode->cost + doneNode->edges_cost[i];
			if (nodes[to].cost < 0 || cost < doneNode->edges_cost[i])
			{
				nodes[to].cost = cost;
				nodes[to].toGoal_num = doneNode_num;
			}
		}
	}

	//startから最短経路をたどってgoalまで
	int next = nodes[start].toGoal_num;
	this->CubicInterpolateBVH(&bvhs[((next - 2) % size_interseg) + 2], 5);
	//cout << ((next-2) % size_interseg) + 2 << endl;
	for (int i = 0; i < num_seg - 1; i++) {
		next = nodes[next].toGoal_num;
		this->CubicInterpolateBVH(&bvhs[((next - 2) % size_interseg) + 2], 5);
		//cout << ((next-2) % size_interseg) + 2 << endl;
	}
	this->CubicInterpolateBVH(&bvhs[goal], 5);
}

void ofxDigitalDanceBvh::exportFrameBVH(const string &filename, int start, int end, float scale)
{
	out.open(filename.c_str());
	if (!out) {
		std::cerr << "Can't Save BVH" << std::endl;
	}
	string blank;

	// Bone Structure
	DFSJoint(const_cast<ofxBvhJoint*>(this->getJoint(0)), blank, scale);

	// Bone Move
	out << "MOTION" << std::endl;
	out << "Frames: " << this->num_frames << std::endl;
	out << "Frame Time: " << frame_time << std::endl;

	for (int i = start; i<end; i++)
	{
		this->setFrame(i);
		this->update();

		int j;
		for (int j = 0; j< frames.at(i).size(); j++) {
			// Position
			if (j == 0 || j == 1 || j == 2)
			{
				out << frames.at(i).at(j)*scale << " ";
			}
			// Rotation
			else
			{
				out << frames.at(i).at(j) << " ";
			}
		}
		//out << frames.at(i).at(j);
		out << std::endl;
	}

	out.close();

	std::cout << "finish" << std::endl;
}

// Depth First Search for Joint
void ofxDigitalDanceBvh::DFSJoint(ofxBvhJoint *joint, string blank, float scale)
{
	ofVec3f v;
	v = joint->getOffset();

	if (joint->isRoot())
	{
		v.set(ofVec3f(0.0, 0.0, 0.0));
		out << "HIERARCHY" << std::endl;
		out << "ROOT Hips" << "\n" << "{" << std::endl;
		out << " OFFSET " << v.x * scale << " " << v.y * scale << " " << v.z * scale << std::endl;
		out << " CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation" << std::endl;
		for (int i = 0; i<joint->getChildren().size(); i++)
		{
			DFSJoint(joint->getChildren().at(i), blank, scale);
		}
	}
	else if (!joint->isSite())
	{
		blank += " ";
		out << blank.c_str() << "JOINT " << joint->getName() << std::endl;
		out << blank.c_str() << "{" << std::endl;
		out << blank.c_str() << " OFFSET " << v.x * scale << " " << v.y * scale << " " << v.z * scale << std::endl;
		out << blank.c_str() << " CHANNELS " << "3 " << "Zrotation Xrotation Yrotation" << std::endl;
		for (int i = 0; i<joint->getChildren().size(); i++)
		{
			DFSJoint(joint->getChildren().at(i), blank, scale);
		}
	}
	else {
		blank += " ";
		out << blank.c_str() << "End " << joint->getName() << std::endl;
		out << blank.c_str() << "{" << std::endl;
		out << blank.c_str() << " OFFSET " << v.x * scale << " " << v.y * scale << " " << v.z * scale << std::endl;
	}
	out << blank.c_str() << "}" << std::endl;
}

void ofxDigitalDanceBvh::segmentationBVH(string filename, int rows, double data[]) {

	for (int n = 1; n<rows; n++) {
		string file = filename;
		stringstream FileName;
		FileName << file << "-" << setw(3) << setfill('0') << n - 1 << ".bvh";
		FileName >> file;

		string str = " ";
		ofs.open(file);
		// HIERARCHY
		ofs << "HIERARCHY" << endl;
		// ROOTの情報を書く
		ofs << "ROOT " << getJoint(0)->getName() << endl;
		ofs << "{" << endl;
		double roff = 0.000000;
		ofs << str << "OFFSET " << roff << " " << roff << " " << roff << endl;
		ofs << str << "CHANNELS 6 Xposition Yposition Zposition Yrotation Xrotation Zrotation" << endl;
		childroop(getJoint(0), str);
		ofs << "}" << endl;

		//MOTION
		ofs << "MOTION" << endl;
		ofs << "Frames: " << (int)data[n] - (int)data[n - 1] << endl;
		ofs << "Frame Time: " << fixed << setprecision(6) << frame_time << endl;

		for (int i = (int)data[n - 1]; (int)i<data[n]; i++)
		{
			int j = 0;
			for (j = 0; j<frames.at(i).size() - 1; j++) {
				ofs << frames.at(i).at(j) << " ";
			}
			ofs << fixed << frames.at(i).at(j) << std::endl;
		}

		ofs.close();
	}
}

void ofxDigitalDanceBvh::segmentationBVH_4C(string filename) {
	// 4カウント分のフレーム数
	const int new_num_frames = 74;
	// 何セグメントに区切るか(切り捨て)
	int num_seg = this->num_frames / new_num_frames;

	for (int n = 0; n<num_seg; n++) {
		// file = filename-00i.bvhに
		string file = filename;
		stringstream FileName;
		FileName << file << "-" << setw(3) << setfill('0') << n << ".bvh";
		FileName >> file;

		string str = " ";
		ofs.open("data\\" + filename + "\\" + file);
		// HIERARCHY
		ofs << "HIERARCHY" << endl;
		// ROOTの情報を書く
		ofs << "ROOT " << getJoint(0)->getName() << endl;
		ofs << "{" << endl;
		double roff = 0.000000;
		ofs << str << "OFFSET " << roff << " " << roff << " " << roff << endl;
		ofs << str << "CHANNELS 6 Xposition Yposition Zposition Yrotation Xrotation Zrotation" << endl;
		childroop(getJoint(0), str);
		ofs << "}" << endl;

		//MOTION
		ofs << "MOTION" << endl;
		ofs << "Frames: " << new_num_frames << endl;
		ofs << "Frame Time: " << fixed << setprecision(6) << this->frame_time << endl;

		for (int i = n*new_num_frames; i<(n + 1)*new_num_frames; i++)
		{
			int j = 0;
			for (j = 0; j<frames.at(i).size() - 1; j++) {
				ofs << frames.at(i).at(j) << " ";
			}
			ofs << fixed << frames.at(i).at(j) << std::endl;
		}

		ofs.close();
	}
}

void ofxDigitalDanceBvh::drawElipsoid(ofPoint p1, ofPoint p2, float thickness)
{
    ofPushMatrix();
    ofQuaternion q;
    ofVec3f v1(1,0,0);
    ofVec3f v2 = p2 - p1;
    q.makeRotate(v1, v2);
    q.normalize();
    
    float length = (p2-p1).length();
    float angle, x, y, z;
    q.getRotate(angle, x, y, z);

    ofTranslate(p1);
    ofRotate(angle, x, y, z);
    ofScale(1.0, thickness, thickness);
    
    ofDrawSphere(length/2, 0, length/2);
    ofPopMatrix();
}

//
//int ofxDigitalDanceBvh::GreedySampling(float q, float weighteffort[], float max_WE, vector<vector<float>>& D, int Sn[], int n, float rho) {
//	cout << D.size() << endl;
//
//	int bvh_path_num = 0; // 返り値の初期化
//	float max_score = 0.0; // 最大化したいスコア
//	
//	for(int i=0; i<D.size(); i++){
//		// すでにサンプルとして選ばれているデータ（Sn[]にあるデータ）は除く
//		bool overlapped_data = false;
//		for(int num_Sn = 0; num_Sn < n; num_Sn++){
//			if(i == Sn[num_Sn]){
//				overlapped_data = true;
//			}
//		}
//		if(overlapped_data){
//			continue;
//		}
//		
//		// すでに選ばれていなければ返すパス番号の探索に移る
//		if(max_score < (rho * rel_Music_WE(q, weighteffort[i], D.size()))+((1.0-rho) * phi_DistanceScore(i, D, Sn, n))){
//			bvh_path_num = i;
//			max_score = (rho * rel_Music_WE(q, weighteffort[i], D.size()))+((1.0-rho) * phi_DistanceScore(i, D, Sn, n));
//		}
//	}
//
//	return bvh_path_num;
//}
//
//float ofxDigitalDanceBvh::rel_Music_WE(float q, float weighteffort, float max_WE){
//	return 1.0 - ((q - weighteffort)/max_WE);
//}
//
//float ofxDigitalDanceBvh::phi_DistanceScore(int bvh_path_num, vector<vector<float>>& D, int Sn[], int n){
//	float min_distance = 1.0;
//	
//	for(int i = 0; i < n; i++){
//		if(D[bvh_path_num][Sn[i]]<min_distance){
//			min_distance = D[bvh_path_num][Sn[i]];
//		}
//	}
//
//	return min_distance;
//}
//
//int ofxDigitalDanceBvh::mat_dim(vector<vector<float>>& con_mat){
//	cout << "次元数" << con_mat.size() << endl;
//	cout << "列数" << con_mat[0].size() << endl;
//	for (int i = 0; i < con_mat.size(); i++)
//	{
//		for (int j = 0; j < con_mat.size(); j++)
//		{
//			cout << con_mat[i][j] << " ";
//		}
//		cout << endl;
//	}
//	return con_mat.size();
//}

int ofxDigitalDanceBvh::GreedySampling(float q, float weighteffort[], float max_WE, vector<vector<float>>& D, int Sn[], int n, float rho) {
	cout << D.size() << endl;

	int bvh_path_num = 0; // 返り値の初期化
	float max_score = 0.0; // 最大化したいスコア
	
	for(int i=0; i<D.size(); i++){
		// すでにサンプルとして選ばれているデータ（Sn[]にあるデータ）は除く
		bool overlapped_data = false;
		for(int num_Sn = 0; num_Sn < n; num_Sn++){
			if(i == Sn[num_Sn]){
				overlapped_data = true;
			}
		}
		if(overlapped_data){
			continue;
		}
		
		// すでに選ばれていなければ返すパス番号の探索に移る
		if(max_score < (rho * rel_Music_WE(q, weighteffort[i], D.size()))+((1.0-rho) * phi_DistanceScore(i, D, Sn, n))){
			bvh_path_num = i;
			max_score = (rho * rel_Music_WE(q, weighteffort[i], D.size()))+((1.0-rho) * phi_DistanceScore(i, D, Sn, n));
		}
	}

	return bvh_path_num;
}

float ofxDigitalDanceBvh::rel_Music_WE(float q, float weighteffort, float max_WE){
	return 1.0 - ((q - weighteffort)/max_WE);
}

float ofxDigitalDanceBvh::phi_DistanceScore(int bvh_path_num, vector<vector<float>>& D, int Sn[], int n)
{
	float min_distance = 1.0;
	
	for(int i = 0; i < n; i++){
		if(D[bvh_path_num][Sn[i]]<min_distance){
			min_distance = D[bvh_path_num][Sn[i]];
		}
	}
	return min_distance;
}
