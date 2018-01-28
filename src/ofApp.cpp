#include "ofApp.h"

//some colors: http://www.colourlovers.com/palette/1767756/Crescendoe


//--------------------------------------------------------------
void ofApp::setup(){
    
    loadSettings();
    
    fillColor.set(167,29,27);
    lineColor.set(20,20,28);
    extraColor.set(247,232,201, 150);
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    ofBackground(lineColor);
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    bufferSize = 512;
    sampleRate = 44100;
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    numWaves = 6;
    playerActiveWaves.assign(numWaves, false);
    mysteryActiveWaves.assign(numWaves, false);
    
    //setup the wave buttons
    for (int i=0; i<numWaves; i++){
        Wave wave;
        wave.setup(440, sampleRate);
        waves.push_back(wave);
    }
    
    //and the combined waves
    for (int i=0; i<2; i++){
        displayWaves[i].setup(bufferSize);
        
        displayWaves[i].fillColor = fillColor;
        displayWaves[i].lineColor = lineColor;
        displayWaves[i].extraColor = extraColor;
    }
    
    winTime = 0.5;
    winTimer = 0;
    
    targetPan = 0.5;
    pan = targetPan;
    
    restart();
    
    nextAudioPos = 0;
    
    setScreenSize();
    
    //setup serial
    if (useSerial){
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
        int baud = 9600;
        serial.setup(1, baud); //open the first device
        nTimesRead = 0;
        nBytesRead = 0;
        memset(bytesReadString, 0, 4);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::setScreenSize(){
    
    float screenScale = (float)ofGetWidth() / 1200.0f;
    
    
    //combined waves
    for (int i=0; i<2; i++){
        float thisY = ofGetHeight() * 0.35;
        float thisX = ofGetWidth()/2 + ofGetWidth()*0.25 * (i==0 ? -1 : 1);
        displayWaves[i].setPos(thisX, thisY, screenScale);
    }
    
    //wave buttons
    //int centerPos = (numWaves-1)/2;
    for (int i=0; i<numWaves; i++){
        //int order = i;
//        int order = 2;
//        if (i==1 || i == 4){
//            order = 1;
//        }
//        if (i==2 || i == 3){
//            order = 0;
//        }
        
        float thisScale = ((float)ofGetWidth()-200) / (waves[i].displayWidth*(float)numWaves);
        thisScale *= 0.85;
        
        float xPadding = 200 * thisScale ;
        int x = ofMap(i, 0, numWaves-1, xPadding, ofGetWidth()-xPadding);
        
        float topY = ofGetHeight() * 0.75;
        float botY = (ofGetHeight() - (waves[i].displayHeight * thisScale)) * 0.98;
//        float topY = ofGetHeight() * 0.75;
//        float botY = (ofGetHeight() - (waves[i].displayHeight * thisScale)) * 0.95;
        //int y = ofMap( abs(order) , 0, centerPos, topY, botY);
        //int y = ofMap( abs(order) , 0, 2, topY, botY);
        float yPrc = (float)i/(float)(numWaves-1);
        yPrc = powf(yPrc, 1.75);
        float y = (1.0f-yPrc)*botY + yPrc*topY;
        
        waves[i].setPos(x,y);
        waves[i].displayScale = thisScale;
    }
    
    if (ofGetScreenWidth() == ofGetWidth()){
        ofHideCursor();
    }else{
        ofShowCursor();
    }
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
    
//    for (int i=0; i<mysteryIDs.size(); i++){
//        cout<<"love to add "<<mysteryIDs[i]<<endl;
//    }
//
//    cout<<"mys ";
//    for (int i=0; i<numWaves; i++){
//        cout<<mysteryActiveWaves[i];
//    }
//    cout<<endl;
    
    for (int i=0; i<2; i++){
        displayWaves[i].reset();
    }
    
    gameState = STATE_GAME;
    
    winEffectTimer = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (useSerial){
        updateSerial();
    }
    
    //cout<<sourceWaves[0].frequency<<endl;
   
    if (gameState == STATE_GAME){
        //winner?
        bool winner = true;
        for (int i=0; i<numWaves; i++){
            if (mysteryActiveWaves[i] != playerActiveWaves[i]){
                winner = false;
            }
        }
        if (winner){
            winTimer += ofGetLastFrameTime();
            if (winTimer >= winTime){
                startWinSequence();
            }
        }else{
            winTimer -= ofGetLastFrameTime();
            winTimer = MAX(0, winTimer);
        }
    }
    
    if (gameState == STATE_WIN){
        winEffectTimer += ofGetLastFrameTime();
        
        float panZeno = 0.05;
        targetPan = (1.0f-panZeno)*pan + panZeno * 0.5;
        
        float phaseStartTime = 2.5;
        if (winEffectTimer > phaseStartTime){
            float phaseTimer = winEffectTimer-phaseStartTime;
            winPhaseVolPrc = 0.5 + cos( powf(phaseTimer, 2)) * 0.5;
            for (int i=0; i<2; i++){
                displayWaves[i].winScaleAdjust = 1.0f + winPhaseVolPrc * ofMap(phaseTimer, 0,4, 0,0.5f, true);
            }
        }
        
        if (winEffectTimer > 8){
            endWinSequence();
        }
    }
    
    float winVolZeno = 0.1;
    if (gameState == STATE_WIN_END){
        for (int i=0; i<numWaves; i++){
            waves[i].winEffectVol = (1.0f-winVolZeno) * waves[i].winEffectVol;
        }
        //restart the game once we hit 0
        if (waves[0].winEffectVol < 0.001){
            restart();
        }
    }else{
        for (int i=0; i<numWaves; i++){
            waves[i].winEffectVol = (1.0f-winVolZeno) * waves[i].winEffectVol + 1.0f*winVolZeno;
        }
    }
    
    //move the pan
    float panLerp = 0.1f;
    pan = (1.0f-panLerp)*pan + panLerp*targetPan;
    
    //update the display waves
    for (int i=0; i<2; i++){
        displayWaves[i].update(ofGetLastFrameTime(), winEffectTimer);
    }
    
    //and the regular waves
    for (int i=0; i<numWaves; i++){
        waves[i].update(gameState == STATE_WIN || gameState == STATE_WIN_END);
    }
}

//--------------------------------------------------------------
void ofApp::updateSerial(){
    nTimesRead = 0;
    nBytesRead = 0;
    int nRead  = 0;  // a temp variable to keep count per read
    
    unsigned char bytesReturned[3];
    
    memset(bytesReadString, 0, 4);
    memset(bytesReturned, 0, 3);
    
    while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
        nTimesRead++;
        nBytesRead = nRead;
    };
    
    memcpy(bytesReadString, bytesReturned, 3);
    
    if (nBytesRead > 0){
        //cout<<"read:"<<bytesReadString<<endl;
        readString += bytesReadString;
    }
    
    
    char touchVals[NUM_TOUCH_BUTTONS];
    touchVals[0] = '0';
    touchVals[1] = '1';
    touchVals[2] = '2';
    touchVals[3] = '3';
    touchVals[4] = '4';
    touchVals[5] = '5';
    touchVals[6] = '6';
    touchVals[7] = '7';
    touchVals[8] = '8';
    touchVals[9] = '9';
    touchVals[10] = 'a';
    touchVals[11] = 'b';
    
    char releaseVals[NUM_TOUCH_BUTTONS];
    releaseVals[0] = 'q';
    releaseVals[1] = 'w';
    releaseVals[2] = 'e';
    releaseVals[3] = 'r';
    releaseVals[4] = 't';
    releaseVals[5] = 'y';
    releaseVals[6] = 'u';
    releaseVals[7] = 'i';
    releaseVals[8] = 'o';
    releaseVals[9] = 'p';
    releaseVals[10] = 'k';
    releaseVals[11] = 'l';
    
    for (int i=0; i<readString.length(); i++){
        char thisChar = readString[i];
        
        if (gameState == STATE_GAME){
        
            for (int k = 0; k<numWaves; k++){
                if (thisChar == touchVals[k]){
                    playerActiveWaves[k] = true;
                }
                if (thisChar == releaseVals[k]){
                    playerActiveWaves[k] = false;
                }
            }
            
            int numPanButtons = 5;
            for (int k = 0; k<numPanButtons; k++){
                if (thisChar == touchVals[numWaves+k]){
                    targetPan = (float)k/(float)numPanButtons;
                }
            }
        }
        
        if (thisChar == 'x'){
            cout<<"BAD BAD BAD"<<endl;
        }
        
    }
    readString = "";
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
        float sample = pan*mysterySample + (1.0f-pan) * playerSample;
        
        //if the game is over, combine the real sample with a phased out one
        if (gameState == STATE_WIN){
            float phaseSample = displayWaves[MYSTERY_ID].getPhasedSample(k, bufferSize/2);
            sample = (1-winPhaseVolPrc)*sample + winPhaseVolPrc*phaseSample;
            sample *= ofMap(winEffectTimer, 0.0f, 8.0f, 1.0f, 1.5f);
        }
        
        sample *= masterVolume;
        
        //float sample = ofRandom(0.25);
        output[k*nChannels] = sample ;
        output[k*nChannels + 1] = sample ;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //globalScale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.1, 2);
//    ofPushMatrix();
//    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
//    ofScale(globalScale, globalScale);
//    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    
    //wave connections
    for (int i=0; i<waves.size(); i++){
        if (playerActiveWaves[i]){
            ofColor thisCol = extraColor;
            thisCol.a = waves[i].winEffectVol * 255;
            ofSetColor(thisCol);
            
            ofSetLineWidth( 2 );
            float startX = waves[i].pos.x + sin(ofGetElapsedTimef() * waves[i].frequency * 0.01 ) * 15;
            float startY = waves[i].pos.y-waves[i].displayHeight*0.5*waves[i].displayScale;
            ofDrawLine(startX, startY, displayWaves[PLAYER_ID].pos.x, displayWaves[PLAYER_ID].pos.y);
        }
        
        
        //wave connections on win
        if (gameState == STATE_WIN && mysteryActiveWaves[i]){
            ofColor col = extraColor;
            col.a = ofMap(winEffectTimer, 0, 3, 0, 255, true);
            ofSetColor(col);
            ofSetLineWidth( 2 );
            float startX = waves[i].pos.x + sin(ofGetElapsedTimef() * waves[i].frequency * 0.01 ) * 15;
            float startY = waves[i].pos.y-waves[i].displayHeight*0.5*waves[i].displayScale;
            ofDrawLine(startX, startY, displayWaves[MYSTERY_ID].pos.x, displayWaves[MYSTERY_ID].pos.y);
        }
    }
    
    //wave buttons
    for (int i=0; i<waves.size(); i++){
        waves[i].draw(fillColor, extraColor, playerActiveWaves[i]);
    }
    
    //the combined waves
    for (int i=0; i<2; i++){
        float thisPan = i==0 ? (1.0f-pan) : pan;
        displayWaves[i].drawBG(thisPan);
    }
    for (int i=0; i<2; i++){
        float thisPan = i==0 ? (1.0f-pan) : pan;
        displayWaves[i].draw(thisPan);
    }
    
    
    //ofPopMatrix();
    
    
    //credits
    
    ofColor credCol = extraColor;
    credCol.a = ofMap(ofGetElapsedTimef(), 20, 30, 255, 0, true);
    ofSetColor(credCol);
    string creditsText = "";
    creditsText += "Game by Andy Wallace, @andy_makes, for GGJ 2018\n";
    creditsText += "Created in openFrameworks\n";
    creditsText += "Color scheme: Crescendoe by leanneeb\ncolourlovers.com/palette/1767756/Crescendoe\n";
    creditsText += "Special thanks to Jane Friedhoff\n";
    ofDrawBitmapString(creditsText, 5, 15);
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
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
    
    //numbers to debug turn on or off sounds
    if (gameState == STATE_GAME){
        for (int i=0; i<numWaves; i++){
            if (key == inputKeys[i]){
                playerActiveWaves[i] = true;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (gameState == STATE_GAME){
        //numbers to debug turn on or off sounds
        for (int i=0; i<numWaves; i++){
            if (key == inputKeys[i]){
                playerActiveWaves[i] = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (gameState == STATE_GAME){
        targetPan = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1, true);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
   
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
    setScreenSize();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::startWinSequence(){
    cout<<"START WIN"<<endl;
    gameState = STATE_WIN;
    winEffectTimer = 0;
    
    for (int i=0; i<2; i++){
        displayWaves[i].startWinSequence();
    }
}

//--------------------------------------------------------------
void ofApp::endWinSequence(){
    gameState = STATE_WIN_END;
    for (int i=0; i<2; i++){
        displayWaves[i].endWinSequence();
    }
}

//--------------------------------------------------------------
//formula from https://pages.mtu.edu/~suits/NoteFreqCalcs.html
float ofApp::getFreq(int halfStepsFrom440){
    return 440 * powf(1.059463094359, (float)halfStepsFrom440);
}

//--------------------------------------------------------------
void ofApp::loadSettings(){
    ofBuffer buffer = ofBufferFromFile("settings.txt");
    
    int curLineNum = 0;
    if(buffer.size()) {
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            
            string line = *it;
            
            if (line.size() > 0){
                if (line[0] != ';'){
                    cout << line << endl;
                    if (curLineNum == 0){
                        useSerial = line.find("TRUE") == 0;
                    }
                    if (curLineNum > 0 && curLineNum <= 6){
                        inputKeys[curLineNum-1] = line[0];
                    }
                    if (curLineNum == 7){
                        masterVolume = ofToFloat(line);
                    }
                    if (curLineNum == 8){
                        if(line.find("TRUE") == 0){
                            ofToggleFullscreen();
                        }
                    }
                    
                    curLineNum++;
                    
                }
            }
        }
    }
    else{
        cout<<"no settings file found. Using defauls"<<endl;
        useSerial = false;
        for (int i=0; i<numWaves; i++){
            inputKeys[i] = ('1')+i;
        }
        masterVolume = 1.0;
        ofToggleFullscreen();
    }
}
