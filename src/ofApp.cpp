#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    displayFont.load("font/Hack-Regular.ttf", 27);
    lineHeight = displayFont.stringHeight("Q");
    ofSetWindowShape(ofGetScreenWidth() * 0.8, ofGetScreenHeight() * 0.6);
    maxHeight = ofGetHeight() * 0.8;
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(255,255,255), ofColor(204,235,204));
    if(currentFinanceDatas.size() > 0){
        drawTypo(fileName, 20, 20);
        drawDataInfo();
        drawDataLine();
    }else{
        drawTypo("drag csv file", 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_RIGHT:
            if(startDataID < currentFinanceDatas.size() - 1){
                startDataID++;
            }
            break;
            
        case OF_KEY_LEFT:
            if(startDataID > 0){
                startDataID--;
            }
            break;
            
        case 's':{
            vector<financeData> newDatas;
            int size = currentFinanceDatas.size();
            if(endDataID != -1){ size = endDataID + 1; }
            for (int i = startDataID; i < size; i++) {
                auto f = currentFinanceDatas[i];
                if(i == startDataID){
                    minData[logarithmic_line] = f.getVal(log_val);
                    minData[linear_line] = f.getVal(close_val);
                }
                else if(f.getVal(log_val) < minData[logarithmic_line]){ minData[logarithmic_line] = f.getVal(log_val);
                    minData[linear_line] = f.getVal(close_val);
                }
                newDatas.push_back(f);
            }
            currentFinanceDatas = newDatas;
            setNewData();
            startDataID = 0;
            endDataID = -1;
        }
            break;
        case ' ':{
            endDataID = -1;
            minData[logarithmic_line] = 0.0;
            minData[linear_line] = 0.0;
            currentFinanceDatas = financeDatas;
            setNewData();
            startDataID = 0;
        }
            
            break;
            
        case OF_KEY_LEFT_SHIFT:
            selectEndData = true;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    selectEndData = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    setPin(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    setPin(x, y);
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
    fileName = ofSplitString(dragInfo.files[0], "/").back();
    ofSetWindowTitle(fileName);
    ofBuffer buf = ofBufferFromFile(dragInfo.files[0]);
    string text = buf.getText();
    auto lines = ofSplitString(text, "\n");
    financeDatas.clear();
    for (int i = 1; i < lines.size(); i++) {
        auto &l = lines[i];
        vector<string> parseData = ofSplitString(l, ",");
        financeData f;
        f.init(parseData);
        if(f.getVal(log_val) > maxData[logarithmic_line]){ maxData[logarithmic_line] = f.getVal(log_val); }
        if(f.getVal(log_val) < minData[logarithmic_line]){ minData[logarithmic_line] = f.getVal(log_val); }
        if(f.getVal(close_val) > maxData[linear_line]){ maxData[linear_line] = f.getVal(close_val); }
        if(f.getVal(close_val) < minData[linear_line]){ minData[linear_line] = f.getVal(close_val); }
        financeDatas.push_back(f);
    }
    maxData[logarithmic_line] = ceil(maxData[logarithmic_line]);
    maxData[linear_line] = ceil(maxData[linear_line]);
    currentFinanceDatas = financeDatas;
    setNewData();
}

//--------------------------------------------------------------
void ofApp::setNewData(){
    scale = (screenWidth - gap) / (float)currentFinanceDatas.size();
    dataLine[logarithmic_line].clear();
    for (int x = 0; x < currentFinanceDatas.size(); x++) {
        double height = maxHeight - ofMap(currentFinanceDatas[x].getVal(log_val), minData[logarithmic_line], maxData[logarithmic_line], 0.0, maxHeight);
        double pos = x * scale;
        if(x == 0){ minLineY = height; }
        dataLine[logarithmic_line].addVertex(pos, height);
    }
    
    dataLine[linear_line].clear();
    for (int x = 0; x < currentFinanceDatas.size(); x++) {
        double height = maxHeight - ofMap(currentFinanceDatas[x].getVal(close_val), minData[linear_line], maxData[linear_line], 0.0, maxHeight);
        double pos = x * scale;
        dataLine[linear_line].addVertex(pos, height);
    }
}

//--------------------------------------------------------------
void ofApp::drawDataInfo(){
    ofPushMatrix();
    ofTranslate(20, lineHeight);
    drawTypo("date:"+currentFinanceDatas[startDataID].date_val, 0, lineHeight);
    ofTranslate(0, lineHeight);
    for (int i = 0; i < 7; i++) {
        drawTypo(dataTypeName[i]+currentFinanceDatas[startDataID].getValAsString(i), 0, lineHeight * i);
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawDataLine(){
    ofPushMatrix();
    ofTranslate(screenWidth/2 - (currentFinanceDatas.size() * scale)/2, screenHeight/2 - maxHeight/2);
    ofPushStyle();
    
    ofPushStyle();
    ofSetLineWidth(2);
    ofSetColor(111,118,153);
    ofDrawLine(0, minLineY, currentFinanceDatas.size() * scale, minLineY);
    ofPopStyle();
    
    ofPushStyle();
    ofSetLineWidth(4);
    ofSetColor(255,176,75);
    dataLine[linear_line].draw();
    ofSetColor(236,99,149);
    dataLine[logarithmic_line].draw();
    ofPopStyle();
    
    for (int x = 0; x < currentFinanceDatas.size(); x++) {
        double height = maxHeight - ofMap(currentFinanceDatas[x].getVal(log_val), minData[logarithmic_line], maxData[logarithmic_line], 0.0, maxHeight);
        double pos = x * scale;
        if(x == startDataID){
            ofSetColor(30,148,232);
            ofDrawCircle(pos, height, 8.0);
            drawTypo(ofToString(currentFinanceDatas[x].getYear()), pos, height - lineHeight*2);
            drawTypo(ofToString(currentFinanceDatas[x].getMonth()) + "/" + ofToString(currentFinanceDatas[x].getDay()), pos, height - lineHeight);
        }else if(x == endDataID){
            ofSetColor(154,25,255);
            ofDrawCircle(pos, height, 8.0);
            drawTypo(ofToString(currentFinanceDatas[x].getYear()), pos, height - lineHeight*2);
            drawTypo(ofToString(currentFinanceDatas[x].getMonth()) + "/" + ofToString(currentFinanceDatas[x].getDay()), pos, height - lineHeight);
        }
    }
    
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::setPin(float x, float y){
    float moveX = screenWidth/2 - (currentFinanceDatas.size() * scale)/2;
    float moveY = screenHeight/2 - maxHeight/2;
    
    for (int i = 0; i < dataLine[logarithmic_line].getVertices().size(); i++) {
        float dis = ofDist(x, y, moveX + dataLine[logarithmic_line].getVertices()[i].x, moveY + dataLine[logarithmic_line].getVertices()[i].y);
        if(dis < scale * 0.95){
            if(selectEndData){ endDataID = i; }
            else{ startDataID = i; }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawTypo(const string &s, float x, float y){
    auto size = getRect(s);
    ofSetColor(3,51,79);
    ofDrawRectangle(ofRectangle(x, y, size.width, lineHeight));
    ofSetColor(186,235,196);
    displayFont.drawString(s, x, y + lineHeight * 0.8);
}

//--------------------------------------------------------------
ofRectangle ofApp::getRect(const string &s){
    return displayFont.getStringBoundingBox(s, 0, 0);
}
