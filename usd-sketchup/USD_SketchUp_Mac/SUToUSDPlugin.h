//
// Copyright 2015 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
//  SUToUSDPlugin.h
//
//  Created by Michael B. Johnson on 1/9/18.
//

// This code is specific to the Mac version of the plug-in.
// The idea here is we have an Obj-C++ class that holds on to a Mac-specific
// subclass of our USDExporterPlugin class, which is itself a subclass of
// SketchUp's API's SketchUpModelExporterInterface abstract base class.
//

// A simple connector class that lets us connect our shared c++ plugin class to
// the UI in this obj-c class. We forward declare it here, and implement it
// inside our .mm file
#import <Cocoa/Cocoa.h>
#include <SketchUpAPI/import_export/modelexporterplugin.h>

class USDExporterPluginMac;

@interface SUToUSDPlugin:NSObject<SketchUpModelExporterPlugin> {
    
    IBOutlet NSPanel* optionsPanel;
    IBOutlet NSButton* exportNormalsCheck;
    IBOutlet NSTextField* aspectRatioEntry;
    IBOutlet NSButton* exportCurvesCheck;
    IBOutlet NSButton* exportLinesCheck;
    IBOutlet NSButton* exportEdgesCheck;
    IBOutlet NSButton* exportSingleFileCheck;
    IBOutlet NSButton* exportMaterialsCheck;
    IBOutlet NSButton* exportMeshesCheck;
    IBOutlet NSButton* exportCamerasCheck;
    IBOutlet NSButton* exportARKitCompatibleCheck;
    IBOutlet NSButton* exportDoubleSidedCheck;
    IBOutlet NSPanel* summaryPanel;
    IBOutlet NSTextView* summaryText;
    
    // Delegates most everything to our shared c++ plugin class.
    USDExporterPluginMac* _plugin;
}

// Implementation of the SketchUpModelExporterPlugin protocol
- (SketchUpModelExporterInterface*)sketchUpModelExporterInterface;

// Gathers options from the user.
- (void)showOptionsDialog:(bool)model_has_selection;

// Shows a summary of the export.
- (void)showSummaryDialog:(const std::string&)summary;

// Used to properly close a model dialog
- (IBAction)closePanel:(id)sender;

@end
