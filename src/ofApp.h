#pragma once

#include "ofMain.h"
//LogarithmicPriceScaleViewer

const string dataTypeName[] = {
    "open:",
    "high:",
    "low:",
    "close:",
    "adj:",
    "vol:",
    "log:"
};

typedef enum{
    linear_line,
    logarithmic_line
}line_type;

typedef enum{
    open_val,
    high_val,
    low_val,
    close_val,
    adj_close_val,
    volume_val,
    log_val
}DATA_TYPE;

typedef struct{
    int year;
    int month;
    int day;
    
    void init(const string &s){
        vector<string>data = ofSplitString(s, "-");
        year = ofToInt(data[0]);
        month = ofToInt(data[1]);
        day = ofToInt(data[2]);
    }
}timeInfo;

typedef struct{
    vector<double> data_val;
    string date_val;
    timeInfo time;
    
    void init(const vector<string> & _data){
        date_val = _data[0];
        time.init(date_val);
        data_val.clear();
        for (int i = 1; i < _data.size(); i++) {
            data_val.push_back(ofToDouble(_data[i]));
        }
        data_val.push_back(log(data_val[adj_close_val]));
    }
    
    string getValAsString(int n){ return ofToString(data_val[n]); }
    
    double getVal(DATA_TYPE t) const { return data_val[t]; }
    
    int getYear() { return time.year; }
    int getMonth() { return time.month; }
    int getDay() { return time.day; }
    
    int getTime(int n){
        if(n == 0){ return time.year; }
        else if(n == 1){ return time.month; }
        else if(n == 2){ return time.day; }
        return -1;
    }
    
} financeData;

class ofApp : public ofBaseApp{
private:
    ofTrueTypeFont displayFont;
    vector<financeData>financeDatas;
    vector<financeData>currentFinanceDatas;

    ofPolyline dataLine[2];

    string fileName;

    int startDataID;
    int endDataID;
    int lineHeight;

    double maxData[2];
    double minData[2];
    double maxHeight;
    double screenWidth;
    double screenHeight;
    double scale;
    double minLineY;
    double gap;

    bool selectEndData = false;
    
protected:
    void setNewData();
    void setMinMax(const vector<financeData> &data);
    void setPin(float x, float y);
    void drawDataInfo();
    void drawDataLine();
    void drawTypo(const string &s, float x, float y);
    ofRectangle getRect(const string & s);
    
public:
    ofApp():
    fileName(""),
    startDataID(0),
    endDataID(-1),
    lineHeight(0),
    maxHeight(0.0),
    scale(0.0),
    minLineY(0.0),
    gap(100.0),
    minData{0,0},
    maxData{0,0}
    {}
    
    void setup();
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
    
};
