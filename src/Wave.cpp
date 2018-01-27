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
    
    box.height = 100;
    box.width = audio.size()/2;
    
    //isSelected = false;
}

void Wave::setPos(int x, int y){
    box.x = x;
    box.y = y;
}

void Wave::setFrequency(float newVal){
    frequency = newVal;
    phaseAdder = (frequency / (float) sampleRate) * TWO_PI;
}

void Wave::advanceAudio(){
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }
    
    phase += phaseAdder;
    float sample = sin(phase) * vol;
    
    //cout<<"s"<<sin(phase)<<endl;
    
    nextAudioPos++;
    if (nextAudioPos >= audio.size()){
        nextAudioPos = 0;
    }
    audio[nextAudioPos] = sample;
    
    curSample = sample;
}


void Wave::draw(bool drawActive){
    ofPushMatrix();
    ofTranslate(box.x,box.y);
    
    ofSetColor(drawActive ? 0 : 150);
    ofNoFill();
    
    ofDrawRectangle(0,0, box.width, box.height);
    
    ofBeginShape();
    for (int i=0; i<box.width; i++){
        ofVertex(i, box.height*0.5 + audio[i]*box.height*0.5);
    }
    ofEndShape();
    
    ofPopMatrix();
}


bool Wave::mousePressed(int x, int y){
    return box.inside(x, y);
}
