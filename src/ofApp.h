#pragma once

#include "ofMain.h"
#include "Wave.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
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
    
    
    void audioOut(float * input, int bufferSize, int nChannels);
    
    int bufferSize;
    int sampleRate;
    ofSoundStream soundStream;
    
    vector <float> playerAudio, mysteryAudio;
    int nextAudioPos;
   
    //vector<Wave> playerWaves;
    int combineType;
    
    int numWaves;
    vector<Wave> waves;
    vector<bool> playerActiveWaves, mysteryActiveWaves;
    
    //vector<Wave> mysteryWaves;
    //int mysteryCombineType;
    
    bool playMysteryWave;
    
    float pan;
		
};
