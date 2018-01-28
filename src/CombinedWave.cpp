//
//  CombinedWave.cpp
//  ggj_transmission
//
//  Created by Andrew Wallace on 1/27/18.
//

#include "CombinedWave.hpp"


void CombinedWave::setup(int bufferSize){
    audio.assign(bufferSize, 0.0);
    
    randVal = ofRandom(0,1000);
    
    noiseRange = 0;
    targetNoiseRange  = 0;
}

void CombinedWave::setPos(int homeX, int homeY, float _scale){
    
    homePos.set(homeX, homeY);
    
    waveDisplayHeight = 100;
    displayScale = _scale;
    
}

void CombinedWave::reset(){
    doingWinSequence = false;
    finishingWinSequence = false;
    winScaleAdjust = 1;
    
}

void CombinedWave::update(float deltaTime, float winSequenceTimer){
    
    if (!doingWinSequence && !finishingWinSequence){
        pos = homePos;
        targetNoiseRange = 10 * displayScale;
    }
    
    float noiseSpeed = 0.1;
    float noiseZeno = 0.1f;
    noiseRange = (1.0f-noiseZeno)*noiseRange + noiseZeno*targetNoiseRange;
    
    if (doingWinSequence){
        targetNoiseRange *= ofMap(winSequenceTimer, 0.0f, 2.0f, 1.0f, 0.0f, true);
        
        float shiftCenterPrc = ofMap(winSequenceTimer, 3.0f, 6.0f, 0.0f, 1.0f, true);
        pos.x = (1.0-shiftCenterPrc)*homePos.x + shiftCenterPrc * ((float)ofGetWidth()*0.5);
    }
    
    if (finishingWinSequence){
        float zeno = 0.15;
        winScaleAdjust = (1.0f-zeno)*winScaleAdjust + zeno*1;
        pos.x = (1.0f-zeno)*pos.x + zeno*homePos.x;
        targetNoiseRange = 0;
    }
    
    
    pos.x += -noiseRange + ofNoise(ofGetElapsedTimef() * noiseSpeed, 0, randVal) * noiseRange * 2;
    pos.y = homePos.y -noiseRange + ofNoise(ofGetElapsedTimef() * noiseSpeed, 100, randVal) * noiseRange * 2;
    
}

void CombinedWave::drawBG(float myPan){
    
    float myScale =  ofMap(myPan, 0, 1, 0.9, 1.1) * displayScale;
    myScale += sin( ofGetElapsedTimef() + ofNoise(ofGetElapsedTimef()*0.1, randVal)*0.3 ) * 0.03;
    myScale *= winScaleAdjust;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofScale(myScale, myScale);
    
    //draw the background
    ofFill();
    ofSetCircleResolution(40);
    ofSetColor(lineColor);
    ofDrawEllipse(0, 0, audio.size(), waveDisplayHeight*2.5);
    
    ofColor thisCol = fillColor;
    thisCol.a =  ofMap(myPan, 0, 0.5, 150, 255, true);
    ofSetColor(thisCol);
    ofDrawEllipse(0, 0, audio.size(), waveDisplayHeight*2.5);
    
    ofPopMatrix();
}

void CombinedWave::draw(float myPan){
    
    float myScale =  ofMap(myPan, 0, 1, 0.9, 1.1) * displayScale;
    myScale += sin( ofGetElapsedTimef() + ofNoise(ofGetElapsedTimef()*0.1, randVal)*0.3 ) * 0.03;
    myScale *= winScaleAdjust;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofScale(myScale, myScale);
    
    //draw the wave
    ofSetLineWidth(3);
    ofSetColor(lineColor);
    ofNoFill();
    ofBeginShape();
    for (int i=0; i<audio.size(); i++){
        float thisX = i - (int)(audio.size()/2);
        float thisY = audio[i] * waveDisplayHeight;
        ofVertex(thisX, thisY);
    }
    ofEndShape();
    
    //some wigglers around the edge
    int numDots = 7;
    float ovalW = audio.size() * 0.51;
    float ovalH = waveDisplayHeight*2.5 * 0.51;
    
    float angleStep = TWO_PI/(float)numDots;
    ofSetColor(extraColor.r, extraColor.g, extraColor.b, 150);
    ofSetLineWidth(2);
    float panAdjust = ofMap(myPan, 0, 1, 0.5, 1.5);
    for (int i=0; i<numDots; i++){
        float thisAngle = angleStep*i + ofGetElapsedTimef() * 0.75 * panAdjust;
        float thisAngle2 = thisAngle+0.1;
        float dotX1 = cos(thisAngle) * ovalW ;
        float dotY1 = sin(thisAngle) * ovalH ;
        float dotX2 = cos(thisAngle2) * ovalW ;
        float dotY2 = sin(thisAngle2) * ovalH ;
        //ofDrawCircle(dotX, dotY, 2);
        ofDrawLine(dotX1, dotY1, dotX2, dotY2);
    }
    
    
    ofPopMatrix();
}

void CombinedWave::startWinSequence(){
    doingWinSequence = true;
}
void CombinedWave::endWinSequence(){
    doingWinSequence = false;
    finishingWinSequence = true;
}

float CombinedWave::getPhasedSample(int curPos, int offset){
    int idNum = curPos-offset;
    if (idNum < 0){
        idNum += audio.size();
    }
    
    return audio[idNum];
    
}

