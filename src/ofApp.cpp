#include "ofApp.h"

//some colors: http://www.colourlovers.com/palette/1767756/Crescendoe

//--------------------------------------------------------------
void ofApp::setup(){
    
    fillColor.set(167,29,27);
    lineColor.set(20,20,28);
    
    ofEnableSmoothing();
    
    ofBackground(lineColor);
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    bufferSize = 512;
    sampleRate = 44100;
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    //playerAudio.assign(bufferSize, 0.0);
    //mysteryAudio.assign(bufferSize, 0.0);
    
    numWaves = 8;
    playerActiveWaves.assign(numWaves, false);
    mysteryActiveWaves.assign(numWaves, false);
    
    for (int i=0; i<numWaves; i++){
        int x = 10 + (i%4) * 260;
        int y = 100 + (i/4) * 150;
        
        Wave wave;
        wave.setup(440, sampleRate);
        wave.setPos(x,y);
        waves.push_back(wave);
    }
    
    displayWaves[PLAYER_ID].setup(bufferSize, bufferSize/2 + 10, ofGetHeight()-200);
    displayWaves[MYSTERY_ID].setup(bufferSize, ofGetWidth()-bufferSize/2-10, ofGetHeight()-200);
    for (int i=0; i<2; i++){
        displayWaves[i].fillColor = fillColor;
        displayWaves[i].lineColor = lineColor;
    }
    
    restart();
    
    nextAudioPos = 0;
    
}

//--------------------------------------------------------------
void ofApp::restart(){
    
    int numToAdd = 3;
    vector<int> mysteryIDs;
    
    int lowStepsDist = -25;
    int highStepsDist = 15;
    
    //reset all waves
    for (int i=0; i<numWaves; i++){
        //start by turning all off
        mysteryActiveWaves[i] = false;
        playerActiveWaves[i] = false;
        
        //give a default val to replace
        waves[i].setFrequency(-1);
        
        //slightly random vol
        waves[i].vol = ofRandom(0.5,1.25);
        
    }
    
    //select numToAdd to turn on
    for (int i=0; i<numToAdd; i++){
        bool isGood = false;
        while(!isGood){
            int rand = ofRandom( mysteryActiveWaves.size() );
            if (mysteryActiveWaves[rand] == false){
                mysteryActiveWaves[rand] = true;
                mysteryIDs.push_back(rand);
                isGood = true;
            }
        }
    }
    
    //set them up as a chord
    int cordStart = ofRandom(-25, 10);
    //THIS REQUIRES 3 TO BE ON RIGHT NOW
    waves[ mysteryIDs[0] ].setFrequency( getFreq(cordStart-12) );
    waves[ mysteryIDs[1] ].setFrequency( getFreq(cordStart+12+3) );
    waves[ mysteryIDs[2] ].setFrequency( getFreq(cordStart+7) );
//    waves[ mysteryIDs[0] ].setFrequency( getFreq(cordStart) );
//    waves[ mysteryIDs[1] ].setFrequency( getFreq(cordStart+4-12) );
//    waves[ mysteryIDs[2] ].setFrequency( getFreq(cordStart+7) );
    
    //generate random frequenceies for the remaining waves
    for (int i=0; i<numWaves; i++){
        if (waves[i].frequency < 0){
            cout<<"get rand for "<<i<<endl;
            //set a frequency that definitely has not been used
            bool newSound = false;
            while (newSound == false){
                int halfSteps = ofRandom(lowStepsDist, highStepsDist);
                float newFreq = getFreq( halfSteps);
                newSound = true;
                for (int k=0; k<i-1; k++){
                    if ( (int)newFreq == (int)waves[k].frequency ){
                        newSound = false;
                    }
                }
                waves[i].setFrequency( newFreq );
            }
        }
    }
    
    
    
    for (int i=0; i<mysteryIDs.size(); i++){
        cout<<"love to add "<<mysteryIDs[i]<<endl;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    //cout<<sourceWaves[0].frequency<<endl;
    
    for (int i=0; i<2; i++){
        displayWaves[i].update(ofGetLastFrameTime());
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    
    //cout<<"boop "<<ofGetFrameNum()<<endl;
   
    for (int k = 0; k < bufferSize; k++){
        nextAudioPos++;
        if (nextAudioPos >= displayWaves[0].audio.size()){
            nextAudioPos = 0;
        }
        
        for (int i=0; i<numWaves; i++){
            waves[i].advanceAudio();
        }
        
        //mystery
        bool playerAddedFirst = false;
        float playerSample = 0;
        for (int i=0; i<numWaves; i++){
            if (playerActiveWaves[i]){
                if (!playerAddedFirst){
                    playerSample = waves[i].curSample;
                    playerAddedFirst = true;
                }else{
                    playerSample *= waves[i].curSample;
                }
            }
        }
        displayWaves[PLAYER_ID].audio[k] = playerSample;
        //playerAudio[nextAudioPos] = playerSample;
        
        
        //mystery
        bool mystAddedFirst = false;
        float mysterySample = 0;
        for (int i=0; i<numWaves; i++){
            if (mysteryActiveWaves[i]){
                if (!mystAddedFirst){
                    mysterySample = waves[i].curSample;
                    mystAddedFirst = true;
                }else{
                    mysterySample *= waves[i].curSample;
                }
            }
        }
        displayWaves[MYSTERY_ID].audio[k] = mysterySample;
        //mysteryAudio[nextAudioPos] = mysterySample;

        float sample = pan*mysterySample + (1.0f-pan) * playerSample;
        
        //float sample = ofRandom(0.25);
        output[k*nChannels] = sample ;
        output[k*nChannels + 1] = sample ;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //wave buttons
    for (int i=0; i<waves.size(); i++){
        waves[i].draw(fillColor, playerActiveWaves[i]);
    }
    
    //the combined waves
    for (int i=0; i<2; i++){
        float thisPan = i==0 ? (1.0f-pan) : pan;
        displayWaves[i].draw(thisPan);
    }
    
//    ofPushMatrix();
//    ofTranslate(0, ofGetHeight()-100);
//
//    ofSetColor(0);
//    ofNoFill();
//
//    ofBeginShape();
//    for (int i=0; i<playerAudio.size(); i++){
//        ofVertex(i, playerAudio[i]*100);
//    }
//    ofEndShape();
//
//    ofBeginShape();
//    for (int i=0; i<mysteryAudio.size(); i++){
//        ofVertex(i+bufferSize+100, mysteryAudio[i]*100);
//    }
//    ofEndShape();
//
//
//    ofPopMatrix();

    //winner?
    bool winner = true;
    for (int i=0; i<numWaves; i++){
        if (mysteryActiveWaves[i] != playerActiveWaves[i]){
            winner = false;
        }
    }
    if (winner){
        ofDrawBitmapString("YOU WIN. PRESS R", ofGetWidth()/2, ofGetHeight()/2);
    }
    
    
    
    //debug shit
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 5, 20);
    //string playingText = playMysteryWave ? "mystery" : "yours";
    //ofDrawBitmapString("playing "+playingText, 5, 40);
    //ofDrawBitmapString("combine: "+ofToString(combineType), 5, 60);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'r'){
        restart();
    }
    
    if (key == 'a'){
        cout<<"mys ";
        for (int i=0; i<numWaves; i++){
            cout<<mysteryActiveWaves[i];
        }
        cout<<endl;
        
        cout<<"pla ";
        for (int i=0; i<numWaves; i++){
            cout<<playerActiveWaves[i];
        }
        cout<<endl;
    }
    
    //numbers to debug turn on or off sounds
    for (int i=0; i<numWaves; i++){
        if (key == (int)('1')+i){
            playerActiveWaves[i] = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //numbers to debug turn on or off sounds
    for (int i=0; i<numWaves; i++){
        if (key == (int)('1')+i){
            playerActiveWaves[i] = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    pan = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1, true);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for (int i=0; i<waves.size(); i++){
        if (waves[i].mousePressed(x, y)){
            playerActiveWaves[i] = !playerActiveWaves[i];
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
//formula from https://pages.mtu.edu/~suits/NoteFreqCalcs.html
float ofApp::getFreq(int halfStepsFrom440){
    return 440 * powf(1.059463094359, (float)halfStepsFrom440);
}
