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
//  USDSketchUpUtilities.h
//
//  Created by Michael B. Johnson on 11/24/17.
//
// These are helper functions and classes that should work on
// both Mac and Windows. We also have a Mac-specific set of code
// in SUtoUSDPlugin.h/mm that we'll need to implement an equivalent
// for on Windows.

#ifndef USDSketchUpUtilities_h
#define USDSketchUpUtilities_h

#include <string.h>

#include <SketchUpAPI/sketchup.h>
// for some reason, the import_export headers aren't included in SketchUp's global one
#include <SketchUpAPI/import_export/pluginprogresscallback.h>
#include <SketchUpAPI/import_export/modelexporterplugin.h>

#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/rotation.h>
#include <pxr/base/gf/vec3f.h>

#include "USDExporter.h"

#define SU_CALL(func) if ((func) != SU_ERROR_NONE) throw std::exception()

std::string SafeNameFromExclusionList(const std::string& initialName,
									  const std::set<std::string>& namesToExclude);

pxr::GfMatrix4d usdTransformFromSUTransform(SUTransformation t);

std::string GetComponentDefinitionName(SUComponentDefinitionRef comp_def);
std::string GetComponentInstanceName(SUComponentInstanceRef comp_inst);
std::string GetGroupName(SUGroupRef group);
std::string GetSceneName(SUSceneRef scene);

// Set progress percent & msg, if progress callback is available.
void SU_HandleProgress(SketchUpPluginProgressCallback* callback,
					   double percent_done, std::string message);


// Note: In SUToUSDPlugin (on the Mac) we will subclass this and
// put in our platform specific UI code that can show an options
// panel and a summary panel.
// Someone will need to implement their own subclass on Windows.

class USDExporterPlugin : public SketchUpModelExporterInterface {

public:
	USDExporterPlugin();
	~USDExporterPlugin();
	
	std::string GetIdentifier() const;
	int GetFileExtensionCount() const;
	std::string GetFileExtension(int index) const;
	std::string GetDescription(int index) const;
	bool SupportsOptions() const;
	double GetAspectRatio();
	bool GetExportNormals();
	bool GetExportEdges();
	bool GetExportLines();
	bool GetExportCurves();
	bool GetExportToSingleFile();
	bool GetExportMaterials();
	bool GetExportMeshes();
	bool GetExportCameras();
	bool GetExportARKitCompatible();
	bool GetExportDoubleSided();

	void SetAspectRatio(double ratio);
	void SetExportNormals(bool flag);
	void SetExportEdges(bool flag);
	void SetExportLines(bool flag);
	void SetExportCurves(bool flag);
	void SetExportToSingleFile(bool flag);
	void SetExportMaterials(bool flag);
	void SetExportMeshes(bool flag);
	void SetExportCameras(bool flag);
	void SetExportARKitCompatible(bool flag);
	void SetExportDoubleSided(bool flag);

	// The dialogs are platform dependent and should be
	// implemented by the subclass on Mac and Windows
	virtual void ShowOptionsDialog(bool model_has_selection) = 0;
	virtual void ShowSummaryDialog();
	
	bool ConvertFromSkp(const std::string& inputSU,
						const std::string& outputUSD,
						SketchUpPluginProgressCallback* callback,
						void* reserved);
	
protected:
	// this method will get overridden in the platform specific
	// (Mac or Windows) code. On the Mac, this is in SUToUSDPlugin
	// On Windows this is USDPluginWin.h
	virtual void ShowSummaryDialog(const std::string& summary) = 0;

	// Should be used by the ShowSummaryDialog method.
	std::string _summaryStr;
	void _updateSummaryFromExporter(USDExporter& exporter);

private:
	double _aspectRatio;
	bool _exportNormals;
	bool _exportCurves;
	bool _exportEdges;
	bool _exportLines;
	bool _exportToSingleFile;
	bool _exportMaterials;
	bool _exportMeshes;
	bool _exportCameras;
	bool _exportARKitCompatible;
	bool _exportDoubleSided;
};

#endif /* USDSketchUpUtilities_h */
