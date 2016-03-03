#include "ofApp.h"


/*
 This was a code test given by Local Projects
 
 Fast prototype programmed by Tony Lim
 
 tonyinteractive.com
 
 */
ofImage image;		//Original image
ofImage image2;		//Modified image

////////////////////
const int N = 6;		//Number of bands in spectrum
float spectrum[ N ];	//Smoothed spectrum values
float Rad = 700;		//Cloud raduis parameter
float Vel = 0.2;		//Cloud points velocity parameter
int bandRad = 2;		//Band index in spectrum, affecting Rad value
int bandVel = 16;		//Band index in spectrum, affecting Vel value

const int n = 180;		//Number of cloud points

//Offsets for Perlin noise calculation for points
float tx[n], ty[n];
ofPoint p[n];			//Cloud's points positions

float time0 = 0;		//Time value, used for dt computing


void ofApp::setup()
{
    ofSetWindowShape(1920, 1080);
    ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, 0);
    XML.load("mySettings.xml");
    
    TTF.load("mono.ttf", 7);
    
    /////////////////////////////////////////////////////////////
    
    Input = new ofxDatGuiTextInput("Question", "DO YOU LIKE SQUAREPUSHER?");
    Input->onTextInputEvent(this, &ofApp::onTextInputEvent);
    Input->setWidth(800, .2);
    Input->setPosition(ofGetWidth()/2 - Input->getWidth()/2, 240);
    font.load("ofxbraitsch/fonts/automat.ttf", 65);
    
    Input2 = new ofxDatGuiTextInput("Token", "Type Something Here");
    Input2->onTextInputEvent(this, &ofApp::onTextInputEvent);
    Input2->setWidth(800, .2);
    Input2->setPosition(ofGetWidth()/2 - Input2->getWidth()/2, 240);
    
    Yes = new ofxDatGuiSlider("BACKGROUND", -10, 10, -6.0);
    Yes ->setValue(-6.0);
    Yes -> onSliderEvent(this, &ofApp::onSliderEvent);
    
    buttonYes = new ofxDatGuiButton("YES");
    buttonYes->setBackgroundColor(ofColor::orangeRed);
    buttonNo = new ofxDatGuiButton("NO");
    questionCreate = new ofxDatGuiButton("CLICK HERE TO START");
    
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setWidth(900, 0.2);
    
    
    folder = gui->addFolder("Questions & Scores", ofColor::white);
    folder->setWidth(500, 0.2);
    
    // textGUI
    
    Input = folder->addTextInput("Question", "");
    
    Input2 = folder->addTextInput("Token", "");
    
    Yes = folder->addSlider("BACKGROUND", -10, 10,-5.0);
    Yes ->setValue(-6.0);
    Yes -> bind(yesValue1);
    
    buttonYes = folder ->addButton("YES");
    buttonYes->setBackgroundColor(ofColor::skyBlue);
    buttonYes->setLabelColor(ofColor::orangeRed);
    buttonNo =  folder -> addButton("NO");
    buttonNo->setBackgroundColor(ofColor::skyBlue);
    buttonNo->setLabelColor(ofColor::orangeRed);
    
    folder->expand();
    
    gui->addBreak();
    
    questionCreate = folder -> addButton("CLICK HERE TO EDIT XML");
    gui->addHeader(":: drag me to reposition ::");
    
    
    gui->addFooter();
    
    
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    
    //gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    
    //QuestionIncrement childrenNum
    //    Qs = XML.getNumChildren("question");
    //    Ts = XML.getNumChildren("TOKENS");
    questionIncrement = -1;
    questionContent = "Do you like Squarepusher?";
    
    // adding datGUI Rectangle sliders///////////////////////////
    //gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->addHeader("GRAPH SLIDERS");
    //adding slider movement////////////////
    sliderInt1 = new ofxDatGuiSlider(XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L1"), -20, 80, -20);
    sliderInt1->setWidth(ofGetScreenWidth(), .1); // make label area 20% of width //
    sliderInt1->setPosition(0, 1200+70 - sliderInt1->getHeight()/2 - 100);
    sliderInt1->onSliderEvent(this, &ofApp::onSliderEvent);
    sliderInt1->setEnabled(false);
    
    
    sliderInt2 = new ofxDatGuiSlider(XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L2"), -20, 80,-20);
    sliderInt2->setWidth(ofGetScreenWidth(), .1); // make label area 20% of width //
    sliderInt2->setPosition(0, 1200+140 - sliderInt2->getHeight()/2 - 100);
    sliderInt2->onSliderEvent(this, &ofApp::onSliderEvent);
    sliderInt2->setEnabled(false);
    
    sliderInt3 = new ofxDatGuiSlider(XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L3"), -20, 80, -20);
    sliderInt3->setWidth(ofGetScreenWidth(), .1); // make label area 20% of width //
    sliderInt3->setPosition(0, 1200+210 - sliderInt3->getHeight()/2 - 100);
    sliderInt3->onSliderEvent(this, &ofApp::onSliderEvent);
    sliderInt3->setEnabled(false);
    
    sliderInt4 = new ofxDatGuiSlider(XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L4"), -20, 80, -20);
    sliderInt4->setWidth(ofGetScreenWidth(), .1); // make label area 20% of width //
    sliderInt4->setPosition(0, 1200+280 - sliderInt4->getHeight()/2 - 100);
    sliderInt4->onSliderEvent(this, &ofApp::onSliderEvent);
    sliderInt4->setEnabled(false);
    
    
    
    mFullscreen = true;
    refreshWindow();
    
    //adding audioVIS
    //image.loadImage( "image/gCircle.jpg" );	//Load image
    
    image.load( "monroe2.jpg" );
    
    //image.loadImage( "image/mouse.jpg" );
    image2.clone( image );
    //ofSetWindowShape(image.getWidth()/2, image.getHeight()/2);
				
    //////////////
    sound.load( "uto.mp3" );
    //sound.loadSound( "music/mouse.wav" );
    
    //sound.loadSound( "4B.mp3" );
    
    sound.setLoop( true );
    
    sound.play();
    
    
    for (int i=0; i<N; i++) {
        spectrum[i] = 0.0f;
    }//Copy image to image2
    
    
    
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    
    if (e.target->is("Question"))
    {
        
        
        ofXml point;
        //point.addChild("question");
        //point.setTo("question");
        point.addValue("question", e.target->getText());
        XML.addXml(point);
        
        ofXml tokens;
        
        tokens.setTo("question");
        //tokens.setTo("//question");
        tokens.addChild("TOKENS");
        tokens.setTo("TOKENS");
        //tokens.setTo("//TOKENS");
        tokens.addValue("L1", "MusicianShip");
        tokens.addValue("YES", "5");
        tokens.addValue("NO", "-3");
        tokens.setTo("//TOKENS");
        
        tokens.addValue("L2", "Progressiveness");
        tokens.addValue("YES", "10");
        tokens.addValue("NO", "-3");
        //tokens.setTo("//L2");
        tokens.setTo("//TOKENS");
        
        tokens.addValue("L3", "Lyrics");
        tokens.addValue("YES", "10");
        tokens.addValue("NO", "-3");
        //tokens.setTo("//L2");
        tokens.setTo("//TOKENS");
        
        tokens.addValue("L4", "Performance");
        tokens.addValue("YES", "10");
        tokens.addValue("NO", "-3");
        //tokens.setTo("//L2");
        tokens.setTo("//TOKENS");
        
        XML.addXml(tokens);
        
        XML.save("mySettings.xml");
        
        
        
        
        
        
    }
    if(e.target->is("Token")){
        ofXml tokens;
        //        tokens.setTo("settings/question/token");
        //
        //        tokens.addValue("token", e.target->getText());
        //        tokens.addValue("yes", 5);
        //        tokens.addValue("no", 6);
        //        XML.addXml(tokens);
    }
    //XML.setTo("settings");
    XML.save("mySettings.xml");
    
    // text input events carry the text of the input field //
    cout << "From Event Object: " << e.text << endl;
    // although you can also retrieve it from the event target //
    cout << "From Event Target: " << e.target->getText() << endl;
    
    
}
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if(e.target == Yes)
    {
        ofSetBackgroundColor(ofColor::white*e.scale);
        //        XML.addValue("settings/question/token/yes", yesValue1);
        XML.save("mySettings.xml");
        cout << "value = " << e.value << " : scale = " << e.scale << endl;
    }
}
//buttonpopo
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    
    
    if(e.target->is ("YES"))
    {
        int Qs = XML.getNumChildren("question");
        int Ts = XML.getNumChildren("TOKENS");
        
        questionIncrement++;
        questionContent = XML.getValue("//question[" +ofToString(questionIncrement)+ "]");
        
        L1 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L1");
        L2 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L2");
        L3 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L3");
        L4 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L4");
        yesValue1 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/YES[0]");
        yesValue2 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/YES[1]");
        yesValue3 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/YES[2]");
        yesValue4 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/YES[3]");
        score1 += yesValue1;
        score2 += yesValue2;
        score3 += yesValue3;
        score4 += yesValue4;
        sliderInt1->setValue(score1+score1n);
        sliderInt2->setValue(score2+score2n);
        sliderInt3->setValue(score3+score3n);
        sliderInt4->setValue(score4+score4n);
        
        cout << "question = " << questionContent << questionIncrement <<endl;
        cout << "L1 = " << L1 << endl;
        cout << "L2 = " << L2 << endl;
        cout << "score1 = " << score1 << endl;
        cout << "YES1 = " << yesValue1 << endl;
        cout << "YES3 = " << yesValue3 << endl;
        cout << "YES4 = " << yesValue4 << endl;
        
        cout << "NO = " << noValue1 << endl;
        
        cout << "number of QUestions = " << Qs <<endl;
        cout << "number of TOKENS = " << Ts <<endl;
    }else if (e.target-> is ("NO")){
        
        questionIncrement++;
        questionContent = XML.getValue("//question[" +ofToString(questionIncrement)+ "]");
        
        L1 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L1");
        L2 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L2");
        L3 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L3");
        L4 = XML.getValue("//TOKENS["+ofToString(questionIncrement)+ "]" + "/L4");
        
        noValue1 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/NO[0]");
        noValue2 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/NO[1]");
        noValue3 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/NO[2]");
        noValue4 = XML.getValue<int>("//TOKENS["+ofToString(questionIncrement-2)+ "]" + "/NO[3]");
        score1n += noValue1;
        score2n += noValue2;
        score3n += noValue3;
        score4n += noValue4;
        sliderInt1->setValue(score1+score1n);
        sliderInt2->setValue(score2+score2n);
        sliderInt3->setValue(score3+score3n);
        sliderInt4->setValue(score4+score4n);
        
        cout << "question = " << questionContent << questionIncrement <<endl;
        cout << "L1 = " << L1 << endl;
        cout << "L2 = " << L2 << endl;
        cout << "NO = " << noValue1 << endl;
        cout << "NO2 = " << noValue2 << endl;
        cout << "NO3 = " << noValue3 << endl;
        cout << "NO4 = " << noValue4 << endl;
        
    }
    
    if (e.target->is("CLICK HERE TO START"))
    {
        dragPts.clear();
        questionIncrement = 0;
        XML.clear();
        
        XML.addChild("settings");
        
        // let's go back to the root (this is the same thing as reset() btw)
        XML.setTo("//settings");
        
        // add a new stroke and then figure out how many strokes there are
        XML.addChild("QUESTIONS");
        int strokes = XML.getNumChildren("QUESTIONS");
        //cout << "YES = " << strokes << endl;
        
        // use a compound path: STROKE[2] for example to set the current element
        // to the 3rd stroke tag
        XML.setTo("QUESTIONS[" + ofToString(strokes-1) + "]");
        
        xmlStructure	= "<QUESTIONS>\n";
        
        XML.save("mySettings.xml");
        
        cout<< "questionMade" << endl;
        
    }
}


void ofApp::update()
{
    Input2->update();
    Input->update();
    questionContent;
    Yes->update();
    //adding audioVIS
    
    /////////////////////////////////////////////////
    float time = ofGetElapsedTimef();
    
    //Build image2 using image
    for (int y=0; y<image.getHeight(); y++) {
        for (int x=0; x<image.getWidth(); x++) {
            //Use y and time for computing shifted x1
            float sinAmp = cos(spectrum[2] * 30)*0.05;
            float cosAmp = sin(spectrum[4] * 200)*0.05;
            float cosAmp2 = tanf(spectrum[0] * 200)*0.05;
            
            float amp = cos( y * sinAmp );
            float amp2 = sin( x * cosAmp );
            int x1 = x + sin(10)*25*amp;
            int y1 = y + cos(20 )*25*amp2;
            
            //Clamp x1 to range [0, image.width-1]
            x1 = ofClamp( x1, 0, image.getWidth() - 1 );
            y1 = ofClamp( y1, 0, image.getHeight() - 1 );
            
            //Set image2(x, y) equal to image(x1, y)
            ofColor color = image.getColor( x1, y1 );
            image2.setColor( x, y, color );
        }
    }
    image2.update();
    ////////////////////////////////////////////////////
    ofSoundUpdate();
    
    
    float *val = ofSoundGetSpectrum( N );
    
    for ( int i=0; i<N; i++ ) {
        spectrum[i] *= 0.9599;	//Slow decreasing
        spectrum[i] = max( spectrum[i], val[i] );
    }
    
    
    
}

void ofApp::draw()

{
    //Input->draw();
    if(questionIncrement > 3){
        image2.draw( ofGetScreenWidth()/3+150, ofGetScreenHeight()/18);
    }
    ///////////////////////////////////////////////
    
    
    //	ofRect( 10, 700, N * 6, -100 );
    string str = ofToString(questionContent);
    ofRectangle bounds = font.getStringBoundingBox(str, ofGetWidth()/2, ofGetHeight()/2);
    ofSetColor(ofColor::orangeRed);
    font.drawString(str, bounds.x-bounds.width/2, bounds.y-bounds.height/2);
    
    //Input2->draw();
    
    
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 200);
    
    ofDrawRectangle(160, 0, ofGetWidth()-160, 20);
    ofDrawRectangle(160, ofGetHeight()-20, ofGetWidth()-160, 20);
    
    //we draw our status message at the top
    //ofSetColor(210, 90, 100);
    ofSetColor(240, 240, 240);
    
    
    //    for (int i = 0; i < sliderNum; i ++) {
    //        rects[i]->draw();
    //    }
    //draw rect sliders////////////////////////////////
    sliderInt1->draw();
    sliderInt2->draw();
    sliderInt3->draw();
    sliderInt4->draw();
    
    
    
}


void ofApp::keyPressed  (int key){
    
    
    
    if (key == 'z'){
        XML.load("mySettings.xml");
        
        questionContent = XML.getValue("//question[0]");
        
        //        cout << "number of Qs = " << Qs << endl;
        //        cout << "number of Ts = " << Ts << endl;
        
        message = questionContent;
        
    }
    
    //    if(key == 's')
    //    {
    //        XML.save("mySettings.xml");
    //
    //        message = "settings saved to xml!";
    //    }
    //    if(key == 'q')
    //    {
    //        XML.clear();
    //        XML.save("mySettings.xml");
    //
    //        message = "XML settings cleared!";
    //    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    //-------
    //we change the background color based on
    //the two mouse coords coming in
    float xpct = (float)x / ofGetWidth();
    float ypct = (float)y / ofGetHeight();
    red			= xpct * 255.0;
    green		= ypct * 255.0;
    blue		= (int)(red - green) % 255;
    
    
    
    if(lineCount > 64){
        //we find the first <PT> tag with the
        //x and y data and we remove it from the begining
        //this way the displayed text always shows the newest data
        //without going offscreen.
        int pos = xmlStructure.find("</token>");
        xmlStructure = xmlStructure.substr(pos+6);
    }
    
    //here we add the most recent point to our fake xml string
    xmlStructure += "    <token>\n        <yes>"+ofToString(x)+"</yes>\n        <no>"+ofToString(y)+"</no>\n    </token>\n";
    lineCount+=4; //we have added 4 lines so increment by 4
    
    //------------
    //we also record the x y points into an array - so we can draw it
    ofVec2f v(x,y);
    dragPts.push_back(v);
    //XML.save("mySettings.xml");
    
    
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
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
void ofApp::toggleFullscreen()
{
    mFullscreen = !mFullscreen;
    refreshWindow();
}

void ofApp::refreshWindow()
{
    ofSetFullscreen(mFullscreen);
    if (!mFullscreen) {
        ofSetWindowShape(1920, 1400);
        ofSetWindowPosition((ofGetScreenWidth()/2)-(1920/2), 0);
    }
}


void ofApp::exit(){
    XML.save("mySettings.xml");
}


