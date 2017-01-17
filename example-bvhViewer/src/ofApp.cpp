#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	ofBackground(0);

	bvh.resize(3);

	// setup bvh
	bvh[0].load("aachan.bvh");
	bvh[1].load("kashiyuka.bvh");
	bvh[2].load("nocchi.bvh");

    for (int i = 0; i < bvh.size(); i++) {
		bvh[i].play();
		bvh[i].setLoop(true);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < bvh.size(); i++) {
		bvh[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableLighting();
    ofEnableDepthTest();
    light.enable();
    light.setPosition(cam.getPosition());
   
    cam.begin();

	for (int i = 0; i < bvh.size(); i++) {
		bvh[i].drawLine();
	}

	cam.end();
    
    ofDisableDepthTest();
    ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        default:
            break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
