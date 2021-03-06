//
//  WarpVisual.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 3/7/19.
//
//

#include "WarpVisual.h"

WarpVisual::WarpVisual()
{
    
}

WarpVisual::~WarpVisual()
{
    
}

void WarpVisual::setup(ofVec2f pos, ofVec2f size,  int _index)
{
    //Set up attributes
    setSize(size.x, size.y);
    setPosition(pos);
    index = _index;
    
    //Set up listeners for ofxInterface
    ofAddListener(eventTouchDown, this, &WarpVisual::onTouchDown);
    ofAddListener(eventTouchUp, this, &WarpVisual::onTouchUp);
    ofAddListener(eventTouchMove, this, &WarpVisual::onTouchMove);
    ofAddListener(eventClick, this, &WarpVisual::onClick);
  
}

void WarpVisual::update(float dt)
{
    
}

void WarpVisual::draw()
{
    
    ofSetColor(255.0f);
    
    /// \param x Draw position on the x axis.
    /// \param y Draw position on the y axis.
    /// \param w Draw width.
    /// \param h Draw height.
    /// \param sx Subsection x axis offset within the texture.
    /// \param sy Subsection y axis offset within the texture.

    ofVec2f newSize = ofVec2f(size.x - drawPos.x,
                                  size.y - drawPos.y);
    ofVec2f canvasSize = ofVec2f(canvasRef->getTexture().getWidth(),
                                 canvasRef->getTexture().getHeight());
    
    ofVec2f newCropSize = ofVec2f(cropSize.x - drawPos.x/size.x*canvasSize.x,
                                  cropSize.y - drawPos.y/size.y*canvasSize.y );
    
    canvasRef->getTexture().drawSubsection(drawPos.x, drawPos.y,
                                           newSize.x, newSize.y,
                                           cropPos.x,
                                           cropPos.y,
                                           newCropSize.x,
                                           newCropSize.y);

    
	if (active)
	{
		//Draw a border when the warp is actively changing the crop data
		drawOutline(ofVec2f(0.0f, 0.0f), ofVec2f(size.x, size.y));
	}
    
    
}

#pragma mark ATTRIBUTES

void WarpVisual::setActive(bool _active)
{
    active = _active;

	(active) ? setPlane(10) : setPlane(0); 
    
}

#pragma mark CANVAS
void WarpVisual::setCanvasReference(ofFbo * _canvasRef)
{
    canvasRef = _canvasRef;
}


#pragma mark CALLBACKS

void WarpVisual::onTouchDown(ofxInterface::TouchEvent& event)
{
    
}

void WarpVisual::onTouchUp(ofxInterface::TouchEvent& event)
{
    
}

void WarpVisual::onClick(ofxInterface::TouchEvent& event)
{
    //Send event via notification center
    ofxNotificationCenter::Notification mnd;
    mnd.ID = IDManager::one().warpVisual_click_id;
    mnd.data["index"] = index;
    
    ofxNotificationCenter::one().postNotification(IDManager::one().warpVisual_click_id, mnd);
}

void WarpVisual::onTouchMove(ofxInterface::TouchEvent& event)
{
    
}

#pragma mark CROP INFO

void WarpVisual::updateCropData(ofVec2f _cropPos, ofVec2f _cropSize, ofVec2f _drawPos)
{
    cropPos = _cropPos;
    cropSize = _cropSize;
    drawPos = _drawPos*getSize();
    
}

#pragma mark UTILS

void WarpVisual::drawOutline(ofVec2f pos, ofVec2f size)
{
	float delta = 20.0f;
	ofSetColor(ofColor::green);

	ofDrawRectangle(pos.x - delta, pos.y - delta, size.x + 2 * delta, delta);
	ofDrawRectangle(pos.x + size.x, pos.y - delta, delta, size.y + 2 * delta);
	ofDrawRectangle(pos.x - delta, pos.y - delta, delta, size.y + 2 * delta);
	ofDrawRectangle(pos.x - delta, pos.y + size.y, size.x + 2 * delta, delta);
}

