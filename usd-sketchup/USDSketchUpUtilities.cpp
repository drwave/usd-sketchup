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
//  USDSketchUpUtilities.cpp
//
//  Created by Michael B. Johnson on 11/24/17.
//

#include "USDSketchUpUtilities.h"

#include "pxr/base/tf/stringUtils.h"

#pragma mark conversion utilities

static double inchesToCM = 2.54; // SketchUp thinks in inches, we want cms

pxr::GfMatrix4d
usdTransformFromSUTransform(SUTransformation t) {
    // need to scale the translate part of the 4x4 by whatever conversion factor we're using:
    t.values[12] *= inchesToCM;
    t.values[13] *= inchesToCM;
    t.values[14] *= inchesToCM;
    return pxr::GfMatrix4d(t.values[0], t.values[1], t.values[2], t.values[3],
                           t.values[4], t.values[5], t.values[6], t.values[7],
                           t.values[8], t.values[9], t.values[10], t.values[11],
                           t.values[12], t.values[13], t.values[14], t.values[15]);
}


#pragma mark SketchUp string to std string:

// A simple SUStringRef wrapper class which makes usage simpler from C++.
class CSUString {
public:
    CSUString() {
        SUSetInvalid(su_str_);
        SUStringCreate(&su_str_);
    }
    
    ~CSUString() {
        SUStringRelease(&su_str_);
    }
    
    operator SUStringRef*() {
        return &su_str_;
    }
    
    std::string utf8() {
        size_t length;
        SUStringGetUTF8Length(su_str_, &length);
        std::string string;
        string.resize(length);
        size_t returned_length;
        SUStringGetUTF8(su_str_, length, &string[0], &returned_length);
        return string;
    }
    
private:
    // "Disallow copying for simplicity." I have no idea what this means.
    CSUString(const CSUString& copy);
    CSUString& operator= (const CSUString& copy);
    
    SUStringRef su_str_;
};

#pragma mark USD name generation helpers

std::string
SafeName(const char* name) {
    std::string safeName;
    
    if (isdigit(name[0])) {        // USD doesn't like names starting with digits
        safeName.assign ("_");
        safeName.append (name);
    } else {
        safeName.assign(name);
    }
    for (size_t i = 0; i < safeName.length(); i++) {
        if (!isalnum(safeName[i])) {
            safeName.replace(i, 1, 1, '_');
        }
    }
    return safeName;
}

std::string
SafeName(const std::string& strName) {
    return SafeName(strName.c_str());
}

std::string
SafeNameFromExclusionList(const std::string& initialName,
                          const std::set<std::string>& namesToExclude) {
    std::string newName = pxr::TfMakeValidIdentifier(initialName);
    auto search = namesToExclude.find(newName);
    if (search == namesToExclude.end()) {
        // we're not already using it, so we're good:
        return newName;
    }
    newName = newName + "_";
    return SafeNameFromExclusionList(newName, namesToExclude);
}

#pragma mark SketchUp names to std::string helpers :

std::string
GetComponentDefinitionName(SUComponentDefinitionRef comp_def) {
    CSUString name;
    SU_CALL(SUComponentDefinitionGetName(comp_def, name));
    return name.utf8();
}

std::string
GetComponentInstanceName(SUComponentInstanceRef comp_inst) {
    CSUString name;
    SU_CALL(SUComponentInstanceGetName(comp_inst, name));
    return name.utf8();
}

std::string
GetGroupName(SUGroupRef group) {
    CSUString name;
    SU_CALL(SUGroupGetName(group, name));
    return name.utf8();
}

std::string
GetSceneName(SUSceneRef scene) {
    CSUString name;
    SU_CALL(SUSceneGetName(scene, name));
    return name.utf8();
}

#pragma mark Progress callback

void
SU_HandleProgress(SketchUpPluginProgressCallback* callback,
                  double percent_done, std::string message) {
    if (callback != NULL) {
        if (callback->HasBeenCancelled()) {
            // Throw an exception to be caught by the top-level handler.
            throw std::exception();
        } else {
            callback->SetPercentDone(percent_done);
            callback->SetProgressMessage(message.c_str());
        }
    } else {
        std::cout << message << " : (" << percent_done << "% done)\n";
    }
}

#pragma mark Portable (between Mac & Win) class to be subclassed

USDExporterPlugin::USDExporterPlugin(): _exportNormals(false),
                                        _aspectRatio(1.33),
                                        _exportEdges(false),
                                        _exportLines(false),
                                        _exportCurves(false),
                                        _exportToSingleFile(false),
                                        _exportMaterials(true) {
}

USDExporterPlugin::~USDExporterPlugin() {
}

std::string
USDExporterPlugin::GetIdentifier() const {
    return "com.sketchup.exporters.usd";
}

int
USDExporterPlugin::GetFileExtensionCount() const {
    return 3;
}

std::string
USDExporterPlugin::GetFileExtension(int index) const {
    if (index == 0) {
        return "usdz";
    }
    if (index == 1) {
        return "usd";
    }
    return "usda";
}

std::string
USDExporterPlugin::GetDescription(int index) const {
    if (index == 0) {
        return "Pixar USDZ File (*.usdz)";
    }
    if (index == 1) {
        return "Pixar USD binary File (*.usd)";
    }
    return "Pixar USD ASCII File (*.usda)";
}

bool
USDExporterPlugin::SupportsOptions() const {
    return true;
}

double USDExporterPlugin::GetAspectRatio(){
    return _aspectRatio;
}

bool USDExporterPlugin::GetExportNormals() {
    return _exportNormals;
}

bool USDExporterPlugin::GetExportEdges() {
    return _exportEdges;
}

bool USDExporterPlugin::GetExportCurves() {
    return _exportCurves;
}

bool USDExporterPlugin::GetExportLines() {
    return _exportLines;
}

bool USDExporterPlugin::GetExportToSingleFile() {
    return _exportToSingleFile;
}

bool USDExporterPlugin::GetExportMaterials(){
    return _exportMaterials;
}

void
USDExporterPlugin::SetAspectRatio(double ratio) {
    _aspectRatio = ratio;
}

void
USDExporterPlugin::SetExportNormals(bool flag) {
    _exportNormals = flag;
}

void
USDExporterPlugin::SetExportEdges(bool flag) {
    _exportEdges = flag;
}

void
USDExporterPlugin::SetExportLines(bool flag) {
    _exportLines = flag;
}

void
USDExporterPlugin::SetExportCurves(bool flag) {
    _exportCurves = flag;
}

void
USDExporterPlugin::SetExportToSingleFile(bool flag) {
    _exportToSingleFile = flag;
}

void
USDExporterPlugin::SetExportMaterials(bool flag) {
    _exportMaterials = flag;
}

void
USDExporterPlugin::ShowSummaryDialog()  {
    if (!_summaryStr.empty()) {
        ShowSummaryDialog(_summaryStr);
    }
    _summaryStr.clear();
}

bool
USDExporterPlugin::ConvertFromSkp(const std::string& inputSU,
                                  const std::string& outputUSD,
                                  SketchUpPluginProgressCallback* callback,
                                  void* reserved) {
    bool converted = false;
    USDExporter exporter;
    try {
        exporter.SetAspectRatio(_aspectRatio);
        exporter.SetExportNormals(_exportNormals);
        exporter.SetExportEdges(_exportEdges);
        exporter.SetExportLines(_exportLines);
        exporter.SetExportCurves(_exportCurves);
        exporter.SetExportToSingleFile(_exportToSingleFile);
        exporter.SetExportMaterials(_exportMaterials);
        converted = exporter.Convert(inputSU, outputUSD, callback);
    } catch (...) {
        converted = false;
    }
    _updateSummaryFromExporter(exporter);
    
    return converted;
}

void
USDExporterPlugin::_updateSummaryFromExporter(USDExporter& exporter) {
    unsigned long long count = exporter.GetComponentDefinitionCount();
    std::string msg;
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " Component Definition\n";
        } else {
            msg += " Component Definitions\n";
        }
    }
    count = exporter.GetComponentInstanceCount();
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " Component Instance\n";
        } else {
            msg += " Component Instances\n";
        }
    }
    count = exporter.GetMeshCount();
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " Mesh\n";
        } else {
            msg += " Meshes\n";
        }
    }
    count = exporter.GetEdgesCount();
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " Edge\n";
        } else {
            msg += " Edges\n";
        }
    }
    count = exporter.GetLinesCount();
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " PolyLine\n";
        } else {
            msg += " PolyLines\n";
        }
    }
    count = exporter.GetCurvesCount();
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " Curve\n";
        } else {
            msg += " Curves\n";
        }
    }
    count = exporter.GetCamerasCount();
    if (count) {
        msg += std::string("Exported ") + std::to_string(count);
        if (count == 1) {
            msg += " Camera\n";
        } else {
            msg += " Cameras\n";
        }
        msg += "  (w/aspect ratio of ";
        msg += std::to_string(_aspectRatio);
        msg += ")\n";
    }
    _summaryStr = msg;
}
