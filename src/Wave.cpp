//
//  Wave.cpp
//  ggj_transmission
//
//  Created by Andrew Wallace on 1/26/18.
//

#include "Wave.hpp"


void Wave::setup(float _frequence, int _sampleRate){
    sampleRate = _sampleRate;
    setFrequency(_frequence);
    
    setPos(0,0);
    
    audio.assign(512, 0.0);
    nextAudioPos = 0;
    
    vol = 1;
    
    phase = 0;
    
    displayHeight = 100;
    displayWidth = audio.size()*0.33;
    displayScale = 0.75;
}

void Wave::setPos(int x, int y){
    homePos.x = x;
    homePos.y = y;
    pos.x = x;
    pos.y = y;
}

void Wave::setFrequency(float newVal){
    frequency = newVal;
    phaseAdder = (frequency / (float) sampleRate) * TWO_PI;
    phase = 0;
}

void Wave::advanceAudio(){
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }
    
    phase += phaseAdder;
    float sample = sin(phase) * vol * winEffectVol;
    
    //cout<<"s"<<sin(phase)<<endl;
    
    nextAudioPos++;
    if (nextAudioPos >= audio.size()){
        nextAudioPos = 0;
    }
    audio[nextAudioPos] = sample;
    
    curSample = sample;
}

void Wave::update(bool gameEnd){
    float zeno = 0.01;
    
    float bottomPos = ofGetHeight();
    
    if (gameEnd){
        pos.y = (1.0f-zeno)*pos.y + zeno*bottomPos;
    }else{
        pos.y = (1.0f-zeno)*pos.y + zeno*homePos.y;
    }
}

void Wave::draw(ofColor baseCol, ofColor extraCol, bool drawActive){
    
    ofSetLineWidth(3);
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    ofScale(displayScale, displayScale);
    
    //main rect
    ofColor thisCol = baseCol;
    thisCol.a = drawActive ? 255 : 160;
    ofSetColor(thisCol);
    ofNoFill();
    ofDrawRectangle(-displayWidth/2,-displayHeight/2, displayWidth, displayHeight);
    
    //the wave
    ofBeginShape();
    for (int i=0; i<displayWidth; i++){
        ofVertex(i - displayWidth/2, audio[i]*displayHeight*0.4);
    }
    ofEndShape();
    
    //corners
    ofColor altCol = extraCol;
    altCol.a = drawActive ? 200 : 60;
    ofSetColor(altCol);
    ofSetLineWidth(2);
    float cornerSpacing = 5;
    float cornerLengthA = 10;
    float cornerLengthB = 10;
    if (drawActive){
        cornerLengthA += sin(ofGetElapsedTimef() * frequency * 0.03) * 3;
        cornerLengthB += sin(PI + ofGetElapsedTimef() * frequency * 0.03) * 3;
    }
    
    ofVec2f topLeft(-displayWidth/2-cornerSpacing,-displayHeight/2-cornerSpacing);
    ofVec2f botRight(displayWidth/2+cornerSpacing, displayHeight/2+cornerSpacing);
    
    //top left
    ofDrawLine(topLeft.x, topLeft.y, topLeft.x+cornerLengthA, topLeft.y);
    ofDrawLine(topLeft.x, topLeft.y, topLeft.x, topLeft.y+cornerLengthB);
    
    //top right
    ofDrawLine(botRight.x, topLeft.y, botRight.x-cornerLengthB, topLeft.y);
    ofDrawLine(botRight.x, topLeft.y, botRight.x, topLeft.y+cornerLengthA);
    
    //bottom left
    ofDrawLine(topLeft.x, botRight.y, topLeft.x+cornerLengthB, botRight.y);
    ofDrawLine(topLeft.x, botRight.y, topLeft.x, botRight.y-cornerLengthA);
    
    //bottom right
    ofDrawLine(botRight.x, botRight.y, botRight.x-cornerLengthA, botRight.y);
    ofDrawLine(botRight.x, botRight.y, botRight.x, botRight.y-cornerLengthB);
    
    ofPopMatrix();
}


bool Wave::mousePressed(int x, int y){
    return false;//box.inside(x, y);
}
