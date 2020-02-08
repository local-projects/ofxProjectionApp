//
//  MainGUI.cpp
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//
//

#include "MainGUI.h"
#include "DefaultTheme.h"

MainGUI::MainGUI()
{
    
}

MainGUI::~MainGUI()
{
    
}

void MainGUI::setup(vector<string> &appStates, string _currentDirectory)
{

	currentState = appStates[0];
	currentDirectory = ofFilePath::getBaseName(_currentDirectory);

    states = appStates;

	ofxNotificationCenter::one().addObserver(this, &MainGUI::onNewCropData, IDManager::one().crop_startInfo_id);
	ofAddListener(ofEvents().keyPressed, this, &MainGUI::onKeyPressed);

	setupImGui();
}

void MainGUI::setupImGui(){

	retinaDpi = ((ofAppGLFWWindow *)ofGetWindowPtr())->getPixelScreenCoordScale();
	ImGui::CreateContext();

	static const ImWchar glyphRanges[] = {	0x0020, 0x2600, 0, }; // less
	auto io = &ImGui::GetIO();

	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.GlyphExtraSpacing.x = 0;
	font_config.PixelSnapH = true; //avoid fuzzy mess fonts
	font_config.RasterizerMultiply = 1.0;

	int baseFontSize = 15 * retinaDpi;
	string path = ofToDataPath("fonts/VeraMono-Bold.ttf", true);
	auto unicodeFont = io->Fonts->AddFontFromFileTTF(&path[0], baseFontSize, &font_config, &glyphRanges[0]);
	ImGui::GetIO().FontDefault = io->Fonts->Fonts[0];

	imgui = new ofxImGui::Gui();
	ofxImGui::DefaultTheme* defaultTheme = new ofxImGui::DefaultTheme();
	imgui->setup(defaultTheme, true);

	ImGuiStyle* style = &ImGui::GetStyle();
	ImGuiStyle darkStyle;
	ImGui::StyleColorsDark(&darkStyle);
	*style = darkStyle;
	style->FrameRounding = 5;

	ImGui::GetIO().MouseDrawCursor = false;
}

void MainGUI::onKeyPressed(ofKeyEventArgs &args){
}


void MainGUI::update(){
}

void MainGUI::draw(){

	if(imgui) imgui->begin();

	if(showImgui){

		ImGui::SetNextWindowSize(ImVec2(600 * retinaDpi, 260 * retinaDpi), ImGuiCond_FirstUseEver);
		ImGui::Begin("Mapping", &showImgui); /////////////////////////////////////////////

		ImGui::PushItemWidth(-200 * retinaDpi);

		if (ImGui::BeginCombo("Load Mapping", currentState.c_str(), ImGuiComboFlags_NoPreview)){
			for (int n = 0; n < savedConfigs.size(); n++){
				bool is_selected = (currentConfig == savedConfigs[n]);
				if (ImGui::Selectable(savedConfigs[n].c_str(), is_selected)){ //user selecetd one
					currentConfig = savedConfigs[n];
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().loadProjSetting_id;
					mnd.data["directory"] = currentConfig;
					ofxNotificationCenter::one().postNotification(IDManager::one().loadProjSetting_id, mnd);
				}
			}
			ImGui::EndCombo();
		}

		static char configName[128];
		strcpy(configName, currentDirectory.c_str());
		ImGui::InputTextWithHint("Mapping Name", "Enter Mapping Name", configName, IM_ARRAYSIZE(configName));
		currentDirectory = configName;

		if(ImGui::Button("Save Mapping")){
			//Add to the drop down menu
			string path = ofFilePath::join(projectionDirectory, currentDirectory);
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().saveProjSetting_id;
			mnd.data["directory"] = path;
			auto it = std::find(savedConfigs.begin(), savedConfigs.end(), path);
			if(it == savedConfigs.end()){
				savedConfigs.push_back(path);
			}
			ofxNotificationCenter::one().postNotification(IDManager::one().saveProjSetting_id, mnd);
		}
		ImGui::SliderFloat("Crop Interface Size", &cropInterfaceSize, 0.0f, 5.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().croppingInterfaceScale_id;
			mnd.data["percent"] = cropInterfaceSize;
			ofxNotificationCenter::one().postNotification(IDManager ::one().croppingInterfaceScale_id, mnd);
		}

		ImGui::SliderFloat("Crop Width", &cropWidth_, 0.0f, 1.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().cropWidth_id;
			mnd.data["width"] = cropWidth_;
			ofxNotificationCenter::one().postNotification(IDManager::one().cropWidth_id, mnd);
		}

		ImGui::SliderFloat("Crop Height", &cropHeight_, 0.0f, 1.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().cropWidth_id;
			mnd.data["height"] = cropHeight_;
			ofxNotificationCenter::one().postNotification(IDManager::one().cropHeight_id, mnd);
		}

		ImGui::SliderFloat("Crop XPos", &cropX, 0.0f, 1.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().cropXpos_id;
			mnd.data["cropXpos"] = cropX;
			ofLogNotice("MainGUI::onSliderEvent") << "Updated crop cropXpos scale to: " << cropX;
			ofxNotificationCenter::one().postNotification(IDManager::one().cropXpos_id, mnd);
		}

		ImGui::SliderFloat("Crop YPos", &cropY, 0.0f, 1.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().cropYpos_id;
			mnd.data["cropYpos"] = cropY;
			ofLogNotice("MainGUI::onSliderEvent") << "Updated crop cropYpos scale to: " << cropY;
			ofxNotificationCenter::one().postNotification(IDManager::one().cropYpos_id, mnd);
		}

		ImGui::SliderFloat("X Pos of Crop in Warp", &cropPosInWarp_X, 0.0f, 1.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().posOfCropInWarp_X_id;
			mnd.data["xPos"] = cropPosInWarp_X;
			ofxNotificationCenter::one().postNotification(IDManager::one().posOfCropInWarp_X_id, mnd);
		}

		ImGui::SliderFloat("Y Pos of Crop in Warp", &cropPosInWarp_Y, 0.0f, 1.0f, "%.6f");
		if(ImGui::IsItemEdited()){
			ofxNotificationCenter::Notification mnd;
			mnd.ID = IDManager::one().posOfCropInWarp_Y_id;
			mnd.data["yPos"] = cropPosInWarp_Y;
			ofxNotificationCenter::one().postNotification(IDManager::one().posOfCropInWarp_Y_id, mnd);
		}


		if (ImGui::BeginCombo("Config State", currentState.c_str())){
			for (int n = 0; n < states.size(); n++){
				bool is_selected = (currentState == states[n]);
				if (ImGui::Selectable(states[n].c_str(), is_selected)){ //user selecetd one
					currentState = states[n];
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().appState_id;
					mnd.data["appState"] = n;
					ofxNotificationCenter::one().postNotification(IDManager::one().appState_id, mnd);
				}
				if (is_selected){
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
			}
			ImGui::EndCombo();
		}

		for (int i = 0; i < warpVisible.size(); i++){
			ImGui::PushID(i);
			string num = ofToString(i);

			bool val = warpVisible[i];
			ImGui::Checkbox(num.c_str(), &val);
			warpVisible[i] = val;

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(5 * retinaDpi, 5));
				ImGui::SameLine();
			ImGui::PopID();
		}
		ImGui::Dummy(ImVec2(20 * retinaDpi, 5));
		ImGui::SameLine();

		ImGui::Text("Warp Display State");

		ImGui::PopItemWidth();
		ImGui::End();
	}

	ImGui::ShowDemoWindow();

	if(imgui) imgui->end();

	//if(imgui) gui->draw(); //actually render
}

void MainGUI::updateNumWarps(int n){

	if(n < 0) n = 0;
	if(warpVisible.size() == n) return;

	if(warpVisible.size() < n){ //we add bools
		while(warpVisible.size() != n){
			warpVisible.push_back(true);
		}
	}else{ //we remove bools
		while(warpVisible.size() != n){
			warpVisible.erase(warpVisible.begin() + warpVisible.size() - 1);
		}
	}
}

bool MainGUI::getCaptureMouse(){
	return ImGui::GetIO().WantCaptureMouse;
}

#pragma mark GUI OBJECT

void MainGUI::toggleGuiVisiblity()
{
	showImgui ^= true;
}


#pragma mark CROPPING INTERFACE
float MainGUI::getCropSize()
{
    return cropInterfaceSize;
}

#pragma mark OFXNOTIFCATION CALLBACKS

void MainGUI::onNewCropData(ofxNotificationCenter::Notification& n)
{
    
    float width = n.data["width"];
    float height = n.data["height"];
    float xPos = n.data["xPos"];
    float yPos = n.data["yPos"];
    
	cropX = xPos;
	cropY = yPos;
	cropWidth_ = width;
	cropHeight_ = height;
}

#pragma mark DIRECTORY 

string MainGUI::getCurrentDirectory()
{
    return currentDirectory;
}
