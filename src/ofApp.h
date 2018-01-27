#pragma once

#include "ofMain.h"
#include "Wave.hpp"
#include "CombinedWave.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void setScreenSize();
    void restart();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void startWinSequence();
    void endWinSequence();
    
    void audioOut(float * input, int bufferSize, int nChannels);
    
    float getFreq(int halfStepsFrom440);
    
    int bufferSize;
    int sampleRate;
    ofSoundStream soundStream;
    
    //vector <float> playerAudio, mysteryAudio;
    int nextAudioPos;
   
    int numWaves;
    vector<Wave> waves;
    vector<bool> playerActiveWaves, mysteryActiveWaves;
    
#define PLAYER_ID 0
#define MYSTERY_ID 1
    CombinedWave displayWaves[2];
    
    float pan;
    float targetPan;
    
    //game states
    int gameState;
#define STATE_GAME 1
#define STATE_WIN 2
#define STATE_WIN_END 3
    
    //winning
    float winTime;
    float winTimer;
    
    float winEffectTimer;
    
    float winPhaseVolPrc;
    
    //colors
    ofColor fillColor;
    ofColor lineColor;
    ofColor extraColor;
    
		
};
