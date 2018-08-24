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
