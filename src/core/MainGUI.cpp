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

	ofDirectory dir;
	dir.listDir(projectionDirectory);
	int numFiles = dir.size();
	for(int i = 0; i < numFiles; i++){
		string path = dir.getPath(i);
		ofLogNotice("MainGUI") << "Setting up " << path << " in drop down.";
		savedConfigs.push_back(path);
	}

	states = appStates;

	ofxNotificationCenter::one().addObserver(this, &MainGUI::onNewCropData, IDManager::one().crop_startInfo_id);
	ofAddListener(ofEvents().keyPressed, this, &MainGUI::onKeyPressed);

	setupImGui();
}

void MainGUI::setCurrentState(string state){
	auto it = find(states.begin(), states.end(), state);
	if(it != states.end()){
		currentState = state;
	}
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

	int baseFontSize = 19 * retinaDpi;
	string path = ofToDataPath("fonts/VeraMono-Bold.ttf", true);
	auto unicodeFont = io->Fonts->AddFontFromFileTTF(&path[0], baseFontSize, &font_config, &glyphRanges[0]);
	ImGui::GetIO().FontDefault = io->Fonts->Fonts[0];

	imgui = new ofxImGui::Gui();
	ofxImGui::DefaultTheme* defaultTheme = new ofxImGui::DefaultTheme();
	imgui->setup(defaultTheme, false);

	ImGuiStyle* style = &ImGui::GetStyle();
	ImGuiStyle darkStyle;
	ImGui::StyleColorsDark(&darkStyle);
	*style = darkStyle;
	style->FrameRounding = 5;

	ImGui::GetIO().MouseDrawCursor = false;
}

void MainGUI::onKeyPressed(ofKeyEventArgs &args){
}


void MainGUI::runGui(){

	if(showImgui){

		ImGui::SetNextWindowContentWidth(500 * retinaDpi);

		ImGui::Begin("Mapping", &showImgui, ImGuiWindowFlags_AlwaysAutoResize); /////////////////////////////////////////////

		ImGui::PushItemWidth(-230 * retinaDpi);

		if (ImGui::BeginCombo("App State", currentState.c_str())){
			for (int n = 0; n < states.size(); n++){
				bool is_selected = (currentState == states[n]);
				if (ImGui::Selectable(states[n].c_str(), is_selected)){ //user selecetd one
					currentState = states[n];
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().appState_id;
					mnd.data["appState"] = n;
					ofxNotificationCenter::one().postNotification(mnd.ID, mnd);
				}
				if (is_selected){
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Dummy(ImVec2(2,2 * retinaDpi));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(2,2 * retinaDpi));

		if (ImGui::BeginCombo("Load Mapping", currentConfig.c_str())){
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

		if(currentState == "CROP_CONFIGURATION"){ //TODO smelly

			ImGui::Dummy(ImVec2(2,2 * retinaDpi));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(2,2 * retinaDpi));

			bool open = ImGui::CollapsingHeader("Crop Interface Params", ImGuiTreeNodeFlags_DefaultOpen);
			if(open){
				ImGui::Indent();

				ImGui::SliderFloat("Crop Interface Size", &cropInterfaceSize, 0.0f, 2.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().croppingInterfaceScale_id;
					mnd.data["percent"] = cropInterfaceSize;
					ofxNotificationCenter::one().postNotification(IDManager ::one().croppingInterfaceScale_id, mnd);
				}

				ImGui::Dummy(ImVec2(2,2 * retinaDpi));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(2,2 * retinaDpi));

				static float inc = 0.0001;
				ImGui::SliderFloat("Slider Increment", &inc, 0.00000001f, 0.0003, "%.6f");

				ImGui::DragFloat("Crop Width", &cropWidth_, inc,  0.0f, 1.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().cropWidth_id;
					mnd.data["width"] = cropWidth_;
					ofxNotificationCenter::one().postNotification(IDManager::one().cropWidth_id, mnd);
				}

				ImGui::DragFloat("Crop Height", &cropHeight_, inc, 0.0f, 1.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().cropWidth_id;
					mnd.data["height"] = cropHeight_;
					ofxNotificationCenter::one().postNotification(IDManager::one().cropHeight_id, mnd);
				}

				ImGui::DragFloat("Crop XPos", &cropX, inc, 0.0f, 1.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().cropXpos_id;
					mnd.data["cropXpos"] = cropX;
					ofLogNotice("MainGUI::onSliderEvent") << "Updated crop cropXpos scale to: " << cropX;
					ofxNotificationCenter::one().postNotification(IDManager::one().cropXpos_id, mnd);
				}

				ImGui::DragFloat("Crop YPos", &cropY, inc, 0.0f, 1.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().cropYpos_id;
					mnd.data["cropYpos"] = cropY;
					ofLogNotice("MainGUI::onSliderEvent") << "Updated crop cropYpos scale to: " << cropY;
					ofxNotificationCenter::one().postNotification(IDManager::one().cropYpos_id, mnd);
				}

				ImGui::DragFloat("X Pos of Crop in Warp", &cropPosInWarp_X, inc, 0.0f, 1.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().posOfCropInWarp_X_id;
					mnd.data["xPos"] = cropPosInWarp_X;
					ofxNotificationCenter::one().postNotification(IDManager::one().posOfCropInWarp_X_id, mnd);
				}

				ImGui::DragFloat("Y Pos of Crop in Warp", &cropPosInWarp_Y, inc, 0.0f, 1.0f, "%.6f");
				if(ImGui::IsItemEdited()){
					ofxNotificationCenter::Notification mnd;
					mnd.ID = IDManager::one().posOfCropInWarp_Y_id;
					mnd.data["yPos"] = cropPosInWarp_Y;
					ofxNotificationCenter::one().postNotification(IDManager::one().posOfCropInWarp_Y_id, mnd);
				}
				ImGui::Unindent();
			}
		}

		ImGui::Dummy(ImVec2(2,2 * retinaDpi));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(2,2 * retinaDpi));

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Visible Warps: ");
		ImGui::SameLine();

		for (int i = 0; i < warpVisible.size(); i++){
			ImGui::PushID(i);
			string num = ofToString(i);

			bool val = warpVisible[i];
			ImGui::Checkbox(num.c_str(), &val);
			warpVisible[i] = val;

			if(i < warpVisible.size() - 1){
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(5 * retinaDpi, 5));
				ImGui::SameLine();
			}
			ImGui::PopID();
		}

		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(2,2 * retinaDpi));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(2,2 * retinaDpi));

		ImGui::TextDisabled("(?) App Help");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 60.0f);
			ImGui::Text(
				"## APP COMMANDS ######################\n"
				"\n"
				"   press 't' to toggle fullscreen mode\n"
				"   press '.' in windowed mode to correct the window aspect ratio.\n"
				"   press 'x' to toggle Mapping window\n"
				"   press 'z' to toggle debug info\n"
				"   press 'Z' to toggle assets & players debug info\n"
				"   press 'h' to hide all media objects\n"
				"   press 'H' to show all media objects\n"
				"   press ' ' to save a frame to disk (export.png)\n"
				"   press 'Q' to identify projections\n"
				"   press 'L' to toggle logs on screen\n"
				"   press 's' to start all animations\n"
				"   press 'p' to pause all animations\n"
				"   press 'r' to reset all animations\n"
				"   press '$' to toggle edit RemoteUI parameters\n"
				"\n"
			);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::TextDisabled("(?) Mapping Help");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 60.0f);
			ImGui::Text(
				"## MAPPING COMMANDS ######################\n"
				"\n"
				"   Press 'w' to toggle edit warps\n"
				"   alt + left-click to select a warp\n"
			   	"   left-click near a ctrl point to select it\n"
				"   hold shift while selecting a ctrl point to extend selection\n"
				"   hold shift and left-click on a ctrl point to deselect it\n"
				"   left-click and drag near a selected ctrl-point to move it\n"
				"   left-click + alt + drag on a ctrl point to select and move at once\n"
				"   use arrow keys to move selected control points\n"
				"   hold shift while using arrow keys while to move points faster\n"
			   	"   alt + shift + left-click on a warp to select all its ctrl points\n"
				"   press 'm' to toggle linear warps (faster edits)\n"
				"   press 'F1-F2' to remove / add horizontal ctrl points\n"
				"   press 'F3-F4' to remove / add vertical ctrl points\n"
				"   press '+' and '-' to set brightness of selected warp\n"
				"   press 'r' to reset selected warp\n"
				"   ctrl-click on any text field to input exact numerical values\n"
				"   press 'g' to toggle calibration grid\n"
			   	"\n"
			);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}


		ImGui::End();
	}

	//ImGui::ShowDemoWindow();
}

void MainGUI::guiBegin(){
	if(imgui) imgui->begin();
}

void MainGUI::guiEnd(){
	if(imgui) imgui->end();
}

void MainGUI::guiDraw(){
	if(imgui) imgui->draw();
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

bool MainGUI::getImguiTextFocus(){
	return ImGui::GetIO().WantTextInput;;
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
