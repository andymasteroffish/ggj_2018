//
//  Wave.hpp
//  ggj_transmission
//
//  Created by Andrew Wallace on 1/26/18.
//

#ifndef Wave_hpp
#define Wave_hpp

#include "ofMain.h"

class Wave{
public:
    
    void setup(float _frequence, int _sampleRate);
    void setPos(int x, int y);
    void setFrequency(float newVal);
    void advanceAudio();
    
    void update(bool gameEnd);
    void draw(ofColor baseCol, bool drawActive);
    
    bool mousePressed(int x, int y);
    
    float phase;
    
    float frequency;
    float phaseAdder;
    
    int sampleRate;
    float vol;
    float curSample;
    
    vector <float> audio;
    int nextAudioPos;
    
    //ofRectangle box;
    ofVec2f pos;
    ofVec2f homePos;
    
    float displayWidth;
    float displayHeight;
    float displayScale;
    //bool isSelected;
    
    float winEffectVol;
    
};

#endif /* Wave_hpp */
