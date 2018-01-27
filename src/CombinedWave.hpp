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
    
    void setup(int bufferSize, int homeX, int homeY);
    void update(float deltaTime);
    void draw(float myPan);
    
    ofVec2f pos;
    ofVec2f homePos;
    
    float waveDisplayHeight;
    
    vector <float> audio;
    
    ofColor fillColor;
    ofColor lineColor;
    
    float randVal;
    
};

#endif /* CombinedWave_hpp */
