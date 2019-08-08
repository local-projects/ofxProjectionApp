//
//  AppSettings.cpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/19/19.
//
//

#include "AppSettings.h"


AppSettings::AppSettings()
{
    
}

AppSettings::~AppSettings()
{
    
}

void AppSettings::setup()
{
    /*
     Parse Json
     */
    
    //bool parsingJson = jsonRef.open(configPath);
    
	ofFile file(configPath);
    if(file.exists())
    {
		file >> jsRef;
        ofLogNotice("App_Settings::setup") << "Succesfully initial parse of " << configPath << " with data: " << jsRef.dump(1);
        
        try
        {
            ofJson appSettings = jsRef["Configurable"];
            
            /*
             App Section
             */
            ofJson app = appSettings["app"];
			testing = app["testing"];
			logToFile = app["logToFile"];
            
            /*
             Window section
             */
            ofJson window = appSettings["window"];
			fullScreen = window["fullscreen"];
			windowMode = window["windowMode"];
			appSize.set(window["appWidth"], window["appHeight"]);
			scaleDenominator = window["scale-denominator"];
            
            /*
             Canvas
             */
            ofJson _canvasSize = appSettings["canvasSize"];
            canvasSize.set(_canvasSize["width"], _canvasSize["height"]);
            
            /*
             Warp Settings
             */
            ofJson warpSettings = appSettings["warpSettings"];
			warpDirectoryPath = warpSettings["directoryPath"];
			loadWarpSettingsFromFile = warpSettings["loadFromFile"];
            
            
            /*
             Projectors
             */
            ofJson projectorsList = appSettings["projectors"];
            
            for(int i=0; i < projectorsList.size(); i++)
            {
                App::Projector temp;
                projectors.push_back(temp);
            }
            
            for(int i=0; i < projectors.size(); i++)
            {
                ofJson proj = projectorsList[i];
				int order = proj["order"];
                float width = proj["width"];
                float height = proj["height"];
                int numWarps = proj["warps"];
             
                if(order < projectors.size())
                {
                    
                    projectors[order].order = order;
                    projectors[order].size.set(width, height);
                    projectors[order].numWarps = numWarps;
                    numCrops += numWarps;
                    
                    ofLogNotice("AppSettings::setup") << "Adding projector # " << order << " with (width, height): " << projectors[order].size << " & " << numWarps << " warps";
                    
                    /*
                     Need to do:
                     -- make sure that all projectors are ordered correctly
                     -- make sure canvas size accounts for the entire width of each projector 
                     */
                }
            }
            
            
            
        }catch(exception exc){
            ofLogError("App_Settings::setup") << exc.what() << " While parsing " << configPath;
        }
    }
    else
    {
        ofLogError("App_Settings::setup") << "Failed to parse " << configPath;
    }
}
