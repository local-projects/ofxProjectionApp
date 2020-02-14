//
//  EdgeBlende.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/26/19.
//
//

#include "EdgeBlend.h"
#include "ofxImGui.h"

EdgeBlend::EdgeBlend()
{
    
}

EdgeBlend::~EdgeBlend()
{
    
}

void EdgeBlend::setup(int _index)
{
    
    //The index serves of the UID
    index = _index;
}


void EdgeBlend::setVisible(bool v){
	visibleGui = v;
}


void EdgeBlend::setPosition(float x, float y){
	winPos.x = x;
	winPos.y = y;
}


void EdgeBlend::runGui(float retina){
	//imgui
	if(visibleGui){

		ImGui::SetNextWindowPos(ImVec2(winPos.x * retina, winPos.y * retina), ImGuiCond_FirstUseEver);
		string winName = "EdgeBlend " + ofToString(index);
		ImGui::Begin(winName.c_str(), &visibleGui, ImGuiWindowFlags_AlwaysAutoResize); /////////////////////////////////////////////

		//ImGui::PushItemWidth(-200 * retina);

		static float inc = 0.0001;
		static float in2 = 0.02;
		ImGui::Separator();
		ImGui::SliderFloat("Slider Increment Curve", &in2, 0.0001f, 0.01, "%.6f");
		ImGui::Separator();
		ImGui::DragFloat("Gamma", &gamma, in2,  0.01f, 10.0f, "%.7f");
		ImGui::DragFloat("Exponent", &exponent, in2,  0.6f, 3.5f, "%.7f");
		ImGui::Separator();
		ImGui::SliderFloat("Slider Increment", &inc, 0.00000001f, 0.0003, "%.6f");
		ImGui::Separator();
		ImGui::DragFloat("Left Edge", &edge.x, inc,  0.0f, 1.0f, "%.7f");
		ImGui::DragFloat("Right Edge", &edge.z, inc,  0.0f, 1.0f, "%.7f");
		ImGui::DragFloat("Top Edge", &edge.y, inc,  0.0f, 1.0f, "%.7f");
		ImGui::DragFloat("Bottom Edge", &edge.w, inc,  0.0f, 1.0f, "%.7f");

		ImGui::End();
	}
}


#pragma mark GET EDGE BLENDING VALUES

glm::vec4 EdgeBlend::getEdges(){
	return edge;
}

glm::vec3 EdgeBlend::getGamma(){
    return glm::vec3(gamma, gamma, gamma);
}

float EdgeBlend::getExponent(){
	return exponent;
}

#pragma mark SET EDGE BLENDING VALUES

void EdgeBlend::setEdges(glm::vec4 edges)
{
	edge = edges;
}

void EdgeBlend::setGamma(glm::vec3 gamma_){
	gamma = gamma_.x;
}

void EdgeBlend::setExponent(float _exponent){
	exponent = _exponent;
}

#pragma mark ATTRIBUTES
int EdgeBlend::getIndex(){
    return index;
}
