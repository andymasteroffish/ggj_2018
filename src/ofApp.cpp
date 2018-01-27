#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    bufferSize = 512;
    sampleRate = 44100;
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    playerAudio.assign(bufferSize, 0.0);
    mysteryAudio.assign(bufferSize, 0.0);
    
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
    
    restart();
    
    
    playMysteryWave = true;
    
    nextAudioPos = 0;
    
    combineType = 0;
}

//--------------------------------------------------------------
void ofApp::restart(){
    for (int i=0; i<numWaves; i++){
        mysteryActiveWaves[i] = false;
        waves[i].vol = ofRandom(0.5,1.25);
        waves[i].setFrequency(ofRandom(100,1000));
    }
    
    //testing
    //C, D, Eb, F, Gb, Ab, A and B
    waves[0].setFrequency(261.6);   //C
    waves[1].setFrequency(293.7);   //D
    waves[2].setFrequency(311.1);   //Eb
    waves[3].setFrequency(349.2);   //F
    waves[4].setFrequency(369.994);   //Gb
    waves[5].setFrequency(415.30);   //Ab
    waves[6].setFrequency(440);   //A
    waves[7].setFrequency(493.9);   //B
    
    int numToAdd = 3;
    for (int i=0; i<numToAdd; i++){
        bool isGood = false;
        while(!isGood){
            int rand = ofRandom( mysteryActiveWaves.size() );
            if (mysteryActiveWaves[rand] == false){
                mysteryActiveWaves[rand] = true;
                isGood = true;
            }
                
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    //cout<<sourceWaves[0].frequency<<endl;
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    
    //cout<<"boop "<<ofGetFrameNum()<<endl;
   
    for (int k = 0; k < bufferSize; k++){
        nextAudioPos++;
        if (nextAudioPos >= playerAudio.size()){
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
                    if (combineType == 0){
                        playerSample *= waves[i].curSample;
                    }
                    if (combineType == 1){
                        playerSample += waves[i].curSample;
                    }
                }
            }
        }
        playerAudio[nextAudioPos] = playerSample;
        
        
        //mystery
        bool mystAddedFirst = false;
        float mysterySample = 0;
        for (int i=0; i<numWaves; i++){
            if (mysteryActiveWaves[i]){
                if (!mystAddedFirst){
                    mysterySample = waves[i].curSample;
                    mystAddedFirst = true;
                }else{
                    if (combineType == 0){
                        mysterySample *= waves[i].curSample;
                    }
                    if (combineType == 1){
                        mysterySample += waves[i].curSample;
                    }
                }
            }
        }
        mysteryAudio[nextAudioPos] = mysterySample;

        float sample = pan*mysterySample + (1.0f-pan) * playerSample;
        
        //float sample = ofRandom(0.25);
        output[k*nChannels] = sample ;
        output[k*nChannels + 1] = sample ;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for (int i=0; i<waves.size(); i++){
        waves[i].draw(playerActiveWaves[i]);
    }
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight()-100);
    
    ofSetColor(0);
    ofNoFill();
    
    ofBeginShape();
    for (int i=0; i<playerAudio.size(); i++){
        ofVertex(i, playerAudio[i]*100);
    }
    ofEndShape();
    
    ofBeginShape();
    for (int i=0; i<mysteryAudio.size(); i++){
        ofVertex(i+bufferSize+100, mysteryAudio[i]*100);
    }
    ofEndShape();
    
    
    ofPopMatrix();

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
    if (key == 'c'){
        combineType++;
        if (combineType == 2){
            combineType = 0;
        }
    }
    
    if (key == 'r'){
        restart();
    }
    
    if (key == 'p'){
        playMysteryWave = !playMysteryWave;
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
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    pan = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1);
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
