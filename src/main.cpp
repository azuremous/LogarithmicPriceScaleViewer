#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
//========================================================================
int main( ){

    ofGLFWWindowSettings settings;
    ofAppGLFWWindow window;
    ofCreateWindow(settings);
    window.setup(settings);
    
    ofRunApp(new ofApp());

}
