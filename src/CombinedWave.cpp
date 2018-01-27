//
//  CombinedWave.cpp
//  ggj_transmission
//
//  Created by Andrew Wallace on 1/27/18.
//

#include "CombinedWave.hpp"


void CombinedWave::setup(int bufferSize, int homeX, int homeY){
    audio.assign(bufferSize, 0.0);
    homePos.set(homeX, homeY);
    
    waveDisplayHeight = 100;
    
    randVal = ofRandom(0,1000);
}

void CombinedWave::update(float deltaTime){
    
    pos = homePos;
    
}

void CombinedWave::draw(float myPan){
    
    float myScale =  ofMap(myPan, 0, 1, 0.9, 1.1);
    myScale += sin( ofGetElapsedTimef() + ofNoise(ofGetElapsedTimef()*0.1, randVal)*0.3 ) * 0.03;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofScale(myScale, myScale);
    
    //draw the background
    ofColor thisCol = fillColor;
    thisCol.a =  ofMap(myPan, 0, 0.5, 150, 255, true);
    ofSetColor(thisCol );
    ofFill();
    ofSetCircleResolution(40);
    ofDrawEllipse(0, 0, audio.size(), waveDisplayHeight*2.5);
    
    
    //draw the wave
    ofSetLineWidth(3);
    ofSetColor(lineColor);
    ofNoFill();
    ofBeginShape();
    for (int i=0; i<audio.size(); i++){
        ofVertex(i - (int)(audio.size()/2), audio[i] * waveDisplayHeight);
    }
    ofEndShape();
    
    ofPopMatrix();
}
