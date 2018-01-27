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

void Wave::draw(ofColor baseCol, bool drawActive){
    
    ofSetLineWidth(3);
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    ofScale(displayScale, displayScale);
    
    ofColor thisCol = baseCol;
    thisCol.a = drawActive ? 255 : 100;
    ofSetColor(thisCol);
    ofNoFill();
    
    ofDrawRectangle(-displayWidth/2,-displayHeight/2, displayWidth, displayHeight);
    
    ofBeginShape();
    for (int i=0; i<displayWidth; i++){
        ofVertex(i - displayWidth/2, audio[i]*displayHeight*0.5);
    }
    ofEndShape();
    
    ofPopMatrix();
}


bool Wave::mousePressed(int x, int y){
    return false;//box.inside(x, y);
}
