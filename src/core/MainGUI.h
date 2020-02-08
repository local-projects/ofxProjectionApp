//
//  MainGUI.hpp
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//
//

#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxNotificationCenter.h"
#include "IDManager.h"
#include "ofxImGui.h"

class MainGUI{
public:
    MainGUI();
    ~MainGUI();
    
    void setup(vector<string> &appStates, string _currentDirectory);
    void update();
    void draw();
    
    /*
     Cropping Interface
     */
    float getCropSize();
    
    /*
     Gui Object
     */
    void toggleGuiVisiblity();
	bool isGuiVisible(){return showImgui;}
	void setGuiVisible(bool v){showImgui = v;}

	void updateNumWarps(int n);
	vector<bool> getWarpVisibility(){return warpVisible;};

	bool getCaptureMouse();
    /*
     Directory Management
     */
    string getCurrentDirectory();
    
private:
    
    /*
     Gui Object
     */
	ofxImGui::Gui * imgui  = nullptr;

    
    /*
     Callbacks
     */
	void onKeyPressed(ofKeyEventArgs &args);

    //! Config state management
    vector<string> states;
    
    /*
     ofxNotifcationSystem Callbacks
     */
    void onNewCropData(ofxNotificationCenter::Notification& n);
    
    /*
     Directory Management
     */
    
    /*
      projectionDirectory is the directory that all projection settings will be saved to.
      currentDirectory is the directory of the current or last saved projections
     
     For example, 
     currentDirectory = TESTA;
     projecitonDirectory = data/projections/
     
     fullPath = projectionDirectory + currentDirectory;
     */
    string projectionDirectory = "projections";
    string currentDirectory = "";

	void setupImGui();
	float retinaDpi = 1.0;
	bool showImgui = true;
	float cropInterfaceSize = 1.0;
	float cropY = 0;
	float cropX = 0;
	float cropWidth_ = 1;
	float cropHeight_ = 1;
	float cropPosInWarp_X = 0;
	float cropPosInWarp_Y = 0;
	vector<string> savedConfigs; //list of map configs saved

	string currentState;
	string currentConfig;

	vector<bool> warpVisible;

};
