//
//  CombinedWave.hpp
//  ggj_transmission
//
//  Created by Andrew Wallace on 1/27/18.
//

#ifndef CombinedWave_hpp
#define CombinedWave_hpp

#include "ofMain.h"

class CombinedWave{
public:
    
    void setup(int bufferSize);
    void setPos(int homeX, int homeY, float _scale);
    void reset();
    void update(float deltaTime, float winSequenceTimer);
    
    void drawBG(float myPan);
    void draw(float myPan);
    
    void startWinSequence();
    void endWinSequence();
    
    float getPhasedSample(int curPos, int offset);
    
    ofVec2f pos;
    ofVec2f homePos;
    
    float waveDisplayHeight;
    
    vector <float> audio;
    
    ofColor fillColor;
    ofColor lineColor;
    ofColor extraColor;
    
    float randVal;
    
    float displayScale;
    float winScaleAdjust;
    
    bool doingWinSequence;
    bool finishingWinSequence;
    
    float noiseRange, targetNoiseRange;
    
};

#endif /* CombinedWave_hpp */
