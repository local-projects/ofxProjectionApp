
//
//  ofxProjectionApp.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/5/19.
//
//

#include "ofxProjectionApp.h"

ofxProjectionApp::ofxProjectionApp()
{
    
}

ofxProjectionApp::~ofxProjectionApp()
{
    
}

void ofxProjectionApp::setup(ofFbo * _canvasRef, bool _loadFromFile,  ofVec2f _appSize, float _scaleDenominator, ofxInterface::Node* _sceneRef, string _directoryPath)
{
    warpController = new ofxWarpController();
    
    canvasRef = _canvasRef;
    canvasSize = ofVec2f(canvasRef->getTexture().getHeight(),  canvasRef->getTexture().getWidth());
    
    loadFromFile = _loadFromFile;
    directoryPath = _directoryPath;
    
    appSize = _appSize;
    scaleDenominator = _scaleDenominator;
    
    sceneRef = _sceneRef;
    
    /*
     Set up listeners
     */
    ofAddListener(ofEvents().mousePressed, this, &ofxProjectionApp::onMousePressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxProjectionApp::onKeyReleased);
    
}

void ofxProjectionApp::setupWarps()
{
    
    if(!loadFromFile)
    {
        
        //Set up projector origin that can be added to for each projector
        glm::vec2 projectorOrigin = glm::vec2(0.0f, 0.0f);
        std:shared_ptr<ofxWarpBase> warp;
        
        int numProjectors = ProjectorManager::one().projectors.size();
        
        for(int i = 0; i < numProjectors ; i++)
        {
            projectorOrigin.x = ProjectorManager::one().projectors[i]->size.x * i;
            
            int _numWarps = ProjectorManager::one().projectors[i]->numWarps;
            for(int j = 0; j < _numWarps; j++)
            {
                if(perspective)
                {
                    warp = warpController->buildWarp<ofxWarpPerspectiveBilinear>();
                    
                    warp->setSize(ProjectorManager::one().projectors[i]->size.x/ProjectorManager::one().projectors[i]->numWarps, ProjectorManager::one().projectors[i]->size.y);
                    
                    //Edges are the alphas?
                    warp->setEdges(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                    warp->setGamma(glm::vec3(2.0f, 2.0f, 2.0f));
                    /*
                     //Set up control points for each warp such that the warps are distributed
                     evently across each projector. Assuming that only x pos needs to change
                     and y positions should span across the entire height of the applicaiton.
                     */
                    
                    /*
                     Make sure all values are normalized (i.e 0 to 1) and not the actualy pixel height and pixel width.
                     */
                    // Top left control point

                    float xPos_l = projectorOrigin.x + (ProjectorManager::one().projectors[i]->size.x/ProjectorManager::one().projectors[i]->numWarps)*j;
                    xPos_l = xPos_l/appSize.x;
                    xPos_l = xPos_l/scaleDenominator;
                    
                    float xPos_r = projectorOrigin.x + (ProjectorManager::one().projectors[i]->size.x/ProjectorManager::one().projectors[i]->numWarps)*(j + 1);
                    xPos_r = xPos_r/appSize.x;
                    xPos_r = xPos_r/scaleDenominator;

                    glm::vec2 tl = glm::vec2(xPos_l, 0.0f);
                    glm::vec2 tr = glm::vec2(xPos_l, 1.0f);
                    glm::vec2 bl = glm::vec2(xPos_r, 0.0f);
                    glm::vec2 br = glm::vec2(xPos_r, 1.0f);
                    
					warp->setControlPoint(ControlPoints::TOP_LEFT, tl);
					warp->setControlPoint(ControlPoints::TOP_RIGHT, bl);
					warp->setControlPoint(ControlPoints::BOTTOM_LEFT, br);
					warp->setControlPoint(ControlPoints::BOTTOM_RIGHT, tr);


                    ofLogNotice("ofxProjectionApp") << "warpNum-#" << warpController->getNumWarps() << "-xPos_l, xPos_r: " << xPos_l << " ," << xPos_r;
                }
                else
                {
                    warp = warpController->buildWarp<ofxWarpBilinear>();
                    
                    warp->setSize(ProjectorManager::one().projectors[i]->size.x/ProjectorManager::one().projectors[i]->numWarps,
                                  ProjectorManager::one().projectors[i]->size.y);
                    
                    //Edges are the alphas?
                    warp->setEdges(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                    warp->setGamma(glm::vec3(2.0f, 2.0f, 2.0f));
                    /*
                     //Set up control points for each warp such that the warps are distributed
                     evently across each projector. Assuming that only x pos needs to change
                     and y positions should span across the entire height of the applicaiton.
                     */
                    
                    /*
                     Make sure all values are normalized (i.e 0 to 1) and not the actualy pixel height and pixel width.
                     */
                    // Top left control point
                    
                    
                    float xPos_l = projectorOrigin.x + (ProjectorManager::one().projectors[i]->size.x/ProjectorManager::one().projectors[i]->numWarps)*j;
                    xPos_l = xPos_l/appSize.x;
                    xPos_l = xPos_l/scaleDenominator;
                    
                    float xPos_r = projectorOrigin.x + (ProjectorManager::one().projectors[i]->size.x/ProjectorManager::one().projectors[i]->numWarps)*(j + 1);
                    xPos_r = xPos_r/appSize.x;
                    xPos_r = xPos_r/scaleDenominator;
                    
                    
                    glm::vec2 tl = glm::vec2(xPos_l, 0.0f);
                    glm::vec2 tr = glm::vec2(xPos_l, 1.0f);
                    glm::vec2 bl = glm::vec2(xPos_r, 0.0f);
                    glm::vec2 br = glm::vec2(xPos_r, 1.0f);
                    
                    
                    warp->setControlPoint(ControlPoints::TOP_LEFT, tl );
                    warp->setControlPoint(ControlPoints::TOP_RIGHT, tr );
                    warp->setControlPoint(ControlPoints::BOTTOM_LEFT,  bl );
                    warp->setControlPoint(ControlPoints::BOTTOM_RIGHT, br );
                    
                    ofLogNotice("ofxProjectionApp") << warpController->getNumWarps() << "-xPos_l, xPos_r: " << xPos_l << " ," << xPos_r;
                }
            }
        }
		setupCroppingManager(); 

    }
    else
    {
        ofLogNotice("ofxProjectionApp::setupWarps") << "Loading from file: " << directoryPath;
        
        //Set up warpPath
        warpPath = ofFilePath::join(directoryPath, warpDataFileName);
        
        //Set up cropPath
        cropPath = ofFilePath::join(directoryPath, cropCropFileName);

		loadWarpSettings();
        
		setupCroppingManager();
		loadCropSettings();
    }
    
    ofLogNotice("ofxProjectionApp::setupWarps") << "Set up " << warpController->getNumWarps() << " warps.";
    
    setupEdges();
    
    /*
     Set up callbacks for notification center.
     */
    ofxNotificationCenter::one().addObserver(this, &ofxProjectionApp::onSaveSettings, IDManager::one().saveProjSetting_id);
    ofxNotificationCenter::one().addObserver(this, &ofxProjectionApp::onLoadSettings, IDManager::one().loadProjSetting_id);
        
    
    //setupCroppingManager();
    
    setupCropJsonData();
}

void ofxProjectionApp::setupCroppingManager()
{
    
    /*
     Cropping Manager
     */
    cropMan = new CroppingManager();
    //ofVec2f size, ofVec2f pos, int numCrops, ofFbo *_canvasRef
    cropMan->setup(appSize*guiMan->getCropSize(), ofVec2f(0,0), warpController->getNumWarps(), canvasRef);
    cropMan->setVisible(false);
    sceneRef->addChild(cropMan);
    
    cropMan->setupScaledProjectors(ProjectorManager::one().projectors); 
}

void ofxProjectionApp::setupEdges()
{
    /*
     Set up a set of edges for each warp
     */
    for(int i = 0; i < warpController->getNumWarps(); i++)
    {
		// If this edge already exists, there is no need to create a new one.
		if (i < edgeGuis.size()) {
			continue;
		}
		else {
			// Setup an edge gui
			EdgeBlend* temp = new EdgeBlend();
			temp->setup(i);
			edgeGuis.push_back(temp);
			//Cycle through edges based on edge type

		}
    }
}

void ofxProjectionApp::setupGuiManager(vector<string> &appStates)
{
    guiMan = new MainGUI();
    guiMan->setup(appStates, directoryPath);
}

bool ofxProjectionApp::getCaptureMouse(){
	return guiMan->getCaptureMouse();
}

bool ofxProjectionApp::getImguiTextFocus(){
	return guiMan->getImguiTextFocus();
}

void ofxProjectionApp::update(){

	guiMan->guiBegin();

	//disable warp mouse when messing with imgui
	if(guiMan->getCaptureMouse()){
		warpController->setIgnoreMouseInteractions(true);
	}else{
		warpController->setIgnoreMouseInteractions(false);
	}


    //Update Edge blending params

	int newNumWarp = warpController->getNumWarps();

	if(numWarps != newNumWarp){
		guiMan->updateNumWarps(newNumWarp);
	}

	vector<bool> warpVis = guiMan->getWarpVisibility();

    for(int i =0; i < warpController->getNumWarps(); i++){

		auto warp = warpController->getWarp(i);

		if(!warpVis[i] && warp->isEditing()){
			warp->setEditing(false);
		}

		if (warp && i < edgeGuis.size()){

			edgeGuis[i]->runGui(guiMan->getRetinaDpi());

			//Edge Blending
			warp->setGamma(edgeGuis[i]->getGamma());
			warp->setEdges(edgeGuis[i]->getEdges());
			warp->setExponent(edgeGuis[i]->getExponent());
		}     
    }

	guiMan->runGui();

	guiMan->guiEnd();
}

void ofxProjectionApp::draw()
{
    int numWarps = warpController->getNumWarps();

	vector<bool> warpVis = guiMan->getWarpVisibility();

	if(warpVis.size() != numWarps){
		return;
	}

    for(int i =0; i < numWarps; i++){

		auto warp = warpController->getWarp(i);
		if(warpVis[i]){

			warp->begin();{

				//Get warp bounds to draw into
				auto bounds = warp->getBounds();

				//ofSetColor(ofColor::red);
				//ofDrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height);

				if( i < cropMan->getCropDataSize() ){

					ofClear(0,0,0,0);
					ofVec2f cropPos = cropMan->getCropData(i).pos;
					ofVec2f drawPos = ofVec2f(cropMan->getCropData(i).drawPos.x*bounds.width, cropMan->getCropData(i).drawPos.y*bounds.height);
					ofVec2f size = cropMan->getCropData(i).size;

					canvasRef->getTexture().drawSubsection(bounds.x + drawPos.x,
														   bounds.y + drawPos.y,
														   bounds.width, bounds.height,
														   cropPos.x, cropPos.y,
														   size.x, size.y);
				}
				else
				{
					ofLogError("ofxProjectionApp::draw") << "There are more warps then cropData!";
				}
			}
			warp->end();
		}
    }
	warpController->draw();
	guiMan->guiDraw();

}

#pragma mark GUI STATES


void ofxProjectionApp::setCropManagerVisible(bool visiblity)
{
    cropMan->setVisible(visiblity);
}

void ofxProjectionApp::setMainGuiVisible(bool visibility)
{
	guiMan->setGuiVisible(visibility);
}

bool ofxProjectionApp::isMainGuiVisible(){
	return guiMan->isGuiVisible();
}


#pragma mark CROP DATA
void ofxProjectionApp::setupCropJsonData()
{
    
    for(int i = 0; i < cropMan->getCropDataSize(); i++)
    {

#ifdef OFXPROJECTIONAPP_USE_OFXJSON
        ofxJSONElement data;
        data["warpID"] = ofxJSONElement();
        data["width"] = ofxJSONElement();
        data["height"] = ofxJSONElement();
        data["xPos"] = ofxJSONElement();
        data["yPos"] = ofxJSONElement();
        
        cropData.append(data);
#else
		ofJson js;
		js["warpID"] = 0;
		js["width"] = 0.0f;
		js["height"] = 0.0f;
		js["xPos"] = 0.0f;
		js["yPos"] = 0.0f;

		cropData.push_back(js);
#endif
    }
    
}

void ofxProjectionApp::clearCropJsonData()
{
    for(int i = 0; i < cropMan->getCropDataSize(); i++)
    {
#ifdef OFXPROJECTIONAPP_USE_OFXJSON
        cropData[i]["warpID"] = "";
        cropData[i]["width"] = "";
        cropData[i]["height"] = "";
        cropData[i]["xPos"] = "";
        cropData[i]["yPos"] = "";
#else
		cropData[i]["warpID"] = 0;
		cropData[i]["width"] = 0.0f;
		cropData[i]["height"] = 0.0f;
		cropData[i]["xPos"] = 0.0f;
		cropData[i]["yPos"] = 0.0f;
#endif
    }
}

void ofxProjectionApp::saveCropJsonData(string fileName)
{
    
    //NEED TO DO:

#ifdef OFXPROJECTIONAPP_USE_OFXJSON

    for(int i = 0; i < cropMan->getCropDataSize(); i++)
    {
        cropData[i]["warpID"] = cropMan->getCropData(i).index;
        cropData[i]["width"] = cropMan->getCropData(i).size.x;
        cropData[i]["height"] = cropMan->getCropData(i).size.y;
        cropData[i]["xPos"] = cropMan->getCropData(i).pos.x;
        cropData[i]["yPos"] = cropMan->getCropData(i).pos.y;
        cropData[i]["drawPosX"] = cropMan->getCropData(i).drawPos.x;
        cropData[i]["drawPosY"] = cropMan->getCropData(i).drawPos.y;
    }
    
    ofLogNotice() << "Saving " << cropData.getRawString();
    
    cropData.save(fileName, true);

#else

	for (int i = 0; i < cropMan->getCropDataSize(); i++)
	{
		cropData[i]["warpID"] = cropMan->getCropData(i).index;
		cropData[i]["width"] = cropMan->getCropData(i).size.x;
		cropData[i]["height"] = cropMan->getCropData(i).size.y;
		cropData[i]["xPos"] = cropMan->getCropData(i).pos.x;
		cropData[i]["yPos"] = cropMan->getCropData(i).pos.y;
		cropData[i]["drawPosX"] = cropMan->getCropData(i).drawPos.x;
		cropData[i]["drawPosY"] = cropMan->getCropData(i).drawPos.y;
	}

	ofJson out = cropData;

	string ss = "";
	for (int i = 0; i < cropData.size(); i++) ss += cropData[i].dump(1) + "\n";
	ofLogNotice() << "Saving crop data" << ss;

	ofSaveJson(ofToDataPath(fileName), out);

#endif
    
    
}

#pragma mark NOTIFICATION CENTER
void ofxProjectionApp::onSaveSettings(ofxNotificationCenter::Notification & n)
{
    //! Save warp settings
    string notificationID = n.ID; //the notification ID is available to you
    string directory = n.data["directory"]; //get what you need from the data field
    string path = ofFilePath::join(directory, warpDataFileName);
    //string path = ofFilePath::join(Global::one().projectionDirectory, fileName);
    warpPath = path;
    warpController->saveSettings(warpPath);
    
    //! Save crop settings
    string cropPath = ofFilePath::join(directory, cropCropFileName);
    //string cropPath = ofFilePath::join(Global::one().projectionDirectory, cropDataFileName);
    saveCropJsonData(cropPath);
    cropPath = path;
    ofLogNotice("ofxProjectionApp::onSaveSettings") << "Saving current projection settings at " << path;
}

void ofxProjectionApp::onLoadSettings(ofxNotificationCenter::Notification& n)
{
    string notificationID = n.ID; //the notification ID is available to you
    string directory = n.data["directory"]; //get what you need from the data field
    warpPath = ofFilePath::join(directory, warpDataFileName);
    cropPath = ofFilePath::join(directory, cropCropFileName);
    loadNewSettings();
    
}

#pragma mark STANDARD CALLBACKS
void ofxProjectionApp::onMousePressed(ofMouseEventArgs & args)
{
    if(args.button == OF_MOUSE_BUTTON_RIGHT)
    {
        for(int i=0; i < warpController->getNumWarps(); i++)
        {
            auto warp = warpController->getWarp(i);
            if(warp->isEditing())
            {
                edgeGuis[i]->setPosition(args.x, args.y);
                edgeGuis[i]->setVisible(true);
            }
        }
    }
}

void ofxProjectionApp::onKeyReleased(ofKeyEventArgs &args)
{
	switch (args.key)

	{
	case 'D': 
	{
		ofLogNotice("ofxProjectionApp") << "//-------- print warp data --------// ";

		for (int i = 0; i < warpController->getNumWarps(); i++)
		{
			auto warp = warpController->getWarp(i);
			auto bounds = warp->getBounds();

			ofLogNotice() << "warp#" << i << " (w,h): " << bounds.getWidth() << ", " << bounds.getHeight();
			ofLogNotice() << "warp#" << i << " (x,y): " << bounds.getX() << ", " << bounds.getY();
		}
		break; 
	}
	case 'u':
	{

		ofLogNotice("ofxProjectionApp") << "unselect control points"; 

		for (int i = 0; i < warpController->getNumWarps(); i++)
		{
			auto warp = warpController->getWarp(i);
			warp->deselectControlPoints(); 
		}
	}
	default: break; 
	}
      
}


#pragma mark SAVING
void ofxProjectionApp::saveCurrentSettings()
{
    warpController->saveSettings(warpPath);
    saveCropJsonData(cropPath);
}

#pragma mark LOADING

void ofxProjectionApp::loadWarpSettings()
{
	ofLogNotice("ofxProjectionApp::loadWarpSettings") << "Loading warp settings: " << warpPath;

	warpController->loadSettings(warpPath);

	//! Update slider for edge blending values based on these loaded settings
	int numWarps = warpController->getNumWarps();


	//Set up edgeGuis
	for (int i = edgeGuis.size() - 1; i >= 0; i--) {

		EdgeBlend * deleteGui = edgeGuis[i];
		edgeGuis.erase(edgeGuis.begin() + i);
		delete deleteGui;
	}

	edgeGuis.clear();
    
	for (int i = 0; i < numWarps; i++)
	{
		auto warp = warpController->getWarp(i);

		EdgeBlend *temp = new EdgeBlend();
		temp->setup(i);

		//Edge Blending
		glm::vec4 edges = warp->getEdges();
		glm::vec3 gamma = warp->getGamma();
		float exp = warp->getExponent();

		temp->setEdges(edges);
		temp->setGamma(gamma);
		temp->setExponent(exp);

		edgeGuis.push_back(temp);
        
        // Update Projector Manager
        if(numWarps > ProjectorManager::one().numWarps)
        {
            //Update projector manager
            ProjectorManager::one().addProjector(ProjectorManager::one().projectors.size(), 1, warp->getSize());
        }
    }
    
    //Update data in projector manager
    int warpCounter = 0 ;
    for(int i = 0; i < ProjectorManager::one().projectors.size(); i++)
    {
        for(int j = 0; j < ProjectorManager::one().projectors[i]->numWarps; j++)
        {
            auto warp = warpController->getWarp(warpCounter);
            ProjectorManager::one().projectors[i]->size = warp->getSize();
            warpCounter++;
        }
    }
    
}

void ofxProjectionApp::loadCropSettings()
{
	ofLogNotice("ofxProjectionApp::loadCropSettings") << "Loading crop settings: " << cropPath;

#ifdef OFXPROJECTIONAPP_USE_OFXJSON

	ofxJSONElement newCropData;
	bool parseNewCropData = newCropData.open(cropPath);

	if (parseNewCropData)
	{
		try
		{
			//Resize crop data
			cropMan->resizeCropDataVector(newCropData.size());

			for (int i = 0; i < newCropData.size(); i++)
			{
				CroppingManager::CropInfo temp;

				temp.index = newCropData[i]["warpID"].asInt();
				temp.size.x = newCropData[i]["width"].asFloat();
				temp.size.y = newCropData[i]["height"].asFloat();
				temp.pos.x = newCropData[i]["xPos"].asFloat();
				temp.pos.y = newCropData[i]["yPos"].asFloat();
                
                if(newCropData[i].isMember("drawPosX"))
                {
                    temp.drawPos.x = newCropData[i]["drawPosX"].asFloat();
                    temp.drawPos.y = newCropData[i]["drawPosY"].asFloat();
                }
                
				cropMan->updateCropData(temp, i);
			}
            cropMan->resizeWarpsBasedOffCropData();
		}
		catch (exception exc)
		{
			ofLogError("ofxProjectionApp::loadNewSettings") << exc.what() << " While parsing " << newCropData.getRawString();
		}

		//! Notify CropManager & GUIManager to update the interfaces
		ofxNotificationCenter::Notification mnd;
		mnd.ID = IDManager::one().updateCropInterface_id;
		ofxNotificationCenter::one().postNotification(IDManager::one().updateCropInterface_id, mnd);
	}
	else
	{
		ofLogError("ofxProjectionApp::loadNewSettings") << "Unable to parse " << cropPath;
	}

#else

	ofJson newCropData;
	ofFile file;
	bool bFileLoaded = file.open(ofToDataPath(cropPath));

	if (bFileLoaded)
	{
		try
		{
			// Load the json
			file >> newCropData;

			//Resize crop data
			cropMan->resizeCropDataVector(newCropData.size());

			for (int i = 0; i < newCropData.size(); i++)
			{
				CroppingManager::CropInfo temp;

				temp.index = newCropData[i]["warpID"];
				temp.size.x = newCropData[i]["width"];
				temp.size.y = newCropData[i]["height"];
				temp.pos.x = newCropData[i]["xPos"];
				temp.pos.y = newCropData[i]["yPos"];

				if (newCropData[i].find("drawPosX") != newCropData[i].end())
				{
					temp.drawPos.x = newCropData[i]["drawPosX"];
					temp.drawPos.y = newCropData[i]["drawPosY"];
				}

				cropMan->updateCropData(temp, i);
			}
		}
		catch (exception exc)
		{
			ofLogError("ofxProjectionApp::loadNewSettings") << exc.what() << " While parsing " << newCropData.dump(1);
		}

		//! Notify CropManager & GUIManager to update the interfaces
		ofxNotificationCenter::Notification mnd;
		mnd.ID = IDManager::one().updateCropInterface_id;
		ofxNotificationCenter::one().postNotification(IDManager::one().updateCropInterface_id, mnd);
	}
	else
	{
		ofLogError("ofxProjectionApp::loadNewSettings") << "Unable to parse " << cropPath;
	}

#endif
}

void ofxProjectionApp::loadNewSettings()
{
    ofLogNotice("ofxProjectionApp::loadNewSettings") << "Loading settings: " << warpPath << " and " << cropPath;
    
	loadWarpSettings();
	
    loadCropSettings();
}

#pragma mark WARPS

void ofxProjectionApp::toggleEditingWarpsOff(){
	warpController->turnEditingOff(); 
}

void ofxProjectionApp::enterWarpMode(){
	warpController->turnEditingOn();
}

bool ofxProjectionApp::isWarpMode(){
	return warpController->areWarpsInEditMode();
}

void ofxProjectionApp::setGuiStateToWarp(){
	guiMan->setCurrentState("WARP_CONFIGURATION");
}

//Add warp to the warp controller on the fly and cascade change to other classes
void ofxProjectionApp::addWarp(ofxWarp::WarpBase::Type type, ofVec2f size, ofVec2f pos)
{
    std:shared_ptr<ofxWarpBase> warp;
    
    switch (type)
    {
        case ofxWarp::WarpBase::TYPE_BILINEAR:
        {
            warp = warpController->buildWarp<ofxWarpBilinear>();
            break;
        }
        case ofxWarp::WarpBase::TYPE_PERSPECTIVE:
        {
            warp = warpController->buildWarp<ofxWarpPerspective>();
            break;
        }
        case ofxWarp::WarpBase::TYPE_PERSPECTIVE_BILINEAR:
        {
            warp = warpController->buildWarp<ofxWarpPerspectiveBilinear>();
            break;
        }
        default: break;
    }
	
	warp->setSize(size);
	warp->setEdges(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	warp->setGamma(glm::vec3(2.0f, 2.0f, 2.0f));

    /*
     Make sure all values are normalized (i.e 0 to 1) and not the actualy pixel height and pixel width.
     */
    // Top left control point
    
    
    float xPos_l = pos.x / ofGetWidth();
    float xPos_r = pos.x / ofGetHeight();
    
    
    glm::vec2 tl = glm::vec2(xPos_l, 0.0f);
    glm::vec2 tr = glm::vec2(xPos_l, size.y / ofGetHeight());
    glm::vec2 bl = glm::vec2(xPos_r, 0.0f);
    glm::vec2 br = glm::vec2(xPos_r, size.y / ofGetHeight());
    
    warp->setControlPoint(ControlPoints::TOP_LEFT, tl);
    warp->setControlPoint(ControlPoints::TOP_RIGHT, bl);
    warp->setControlPoint(ControlPoints::BOTTOM_LEFT, br);
    warp->setControlPoint(ControlPoints::BOTTOM_RIGHT, tr);
    
    ofLogNotice("ofxProjectionApp::addWarp") << "warpNum-#" << warpController->getNumWarps() << "-xPos_l, xPos_r: " << xPos_l << " ," << xPos_r;
    
    //Update projector manager
    ProjectorManager::one().addProjector(ProjectorManager::one().projectors.size(),
                                         1,
                                         warp->getSize());
    
    //Update cropping manager
    cropMan->addCrop(size);
    
    
}

//Remove warp to the warp controller on the fly and cascade change to other classes
void ofxProjectionApp::removeWarp(ofxWarp::WarpBase::Type type)
{
    
}
