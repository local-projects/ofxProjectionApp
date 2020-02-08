//
//  EdgeBlend.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/26/19.
//
//

/*
 This class will act as an interface for each edge of our system
 */

#pragma once
#include "ofMain.h"
#include "ofxNotificationCenter.h"
#include "IDManager.h"

class EdgeBlend
{
public:
    EdgeBlend();
    ~EdgeBlend();
    
    void setup(int _index);

	void runGui(float retina);

    
    /*
     Gui Object
     */
	void setVisible(bool v);
	void setPosition(float x, float y);
    
    /*
     Get edge blending values
     */
    glm::vec4 getEdges();
    glm::vec3 getGamma();
    float getExponent();
    
    /*
     Set edge blending values
     */
    void setEdges(glm::vec4 edges);
    void setGamma(glm::vec3 gamma);
    void setExponent(float _exponent);
    
    /*
     Attributes
     */
    int getIndex();
    
private:
    
    /*
     Gui object
     */

	bool visibleGui = false;
	ofVec2f winPos;
    
    /*
     Attributes
     */
    int index = 0;
    

	float gamma = 1;
	glm::vec4 edge;
	float exponent = 1.0;

};
