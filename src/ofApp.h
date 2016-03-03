#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

class Rectangle {
    
public:
    Rectangle(int x, int y, int width, int height){
        _x = x;
        _y = y;
        _width = width;
        _height = height;
    }
    
    bool inside(ofPoint mouse)
    {
        return mouse.distance( ofPoint(_x, _y) ) <= 10;
    }
    
    void draw(){
        ofSetColor(ofColor::blue);
        ofDrawRectangle(_x, _y, this->_width, _height);
    }
    
    int _x, _y, _width, _height;
    
};



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    ofxDatGui* gui;
    ofxDatGuiTextInput* Input;
    ofxDatGuiTextInput* Input2;
    ofxDatGuiSlider* Yes;
    ofxDatGuiSlider* No;
    ofxDatGuiFolder* folder;
    ofxDatGuiDropdown* dropdown;
    ofxDatGuiButton* buttonYes;
    ofxDatGuiButton* buttonNo;
    ofxDatGuiButton* questionCreate;
    //ofxDatGuiButton* toggle;
    
    //ranged sliders//////////////////////
    ofxDatGuiSlider* sliderInt1;
    ofxDatGuiSlider* sliderInt2;
    ofxDatGuiSlider* sliderInt3;
    ofxDatGuiSlider* sliderInt4;
    
    
    ////////////////////////////////////
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    vector<ofxDatGuiComponent*> components;
    
    
    
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
    
    void refreshWindow();
    void toggleFullscreen();
    void exit();
    
    bool mFullscreen;
    //increments////////////////////
    int questionIncrement;
    
    string questionContent;
    
    
    /////////////////////////////////
    
    ofXml XML;
    ofTrueTypeFont font;
    ofTrueTypeFont TTF;
    
    string xmlStructure;
    string message;
    
    vector<ofVec2f> dragPts;
    
    int pointCount;
    int lineCount;
    int lastTagNumber;
    
    float red;
    float green;
    float blue;
    
    ////Adding Rectangle Class variables
    vector<Rectangle*> rects;
    vector<ofxDatGuiSlider*> sWidths;
    
private:
    int sliderNum = 4;
    
    // childrenNUmbers
    //    int Qs;
    //    int Ts;
    //Question Contents/////////////
    string L1;
    string L2;
    string L3;
    string L4;
    int yesValue1;
    int yesValue2;
    int yesValue3;
    int yesValue4;
    int noValue1;
    int noValue2;
    int noValue3;
    int noValue4;
    
    int score1;
    int score2;
    int score3;
    int score4;
    
    int score1n;
    int score2n;
    int score3n;
    int score4n;
    
    bool isFullscreen;
    
    //audioVIS
    ofSoundPlayer sound;	//Sound sample
    
    
    
    
};
