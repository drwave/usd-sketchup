//
// Copyright 2018 Pixar
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

#include <iostream>
#include <string>
#include <unistd.h>

#include "USDExporter.h"
#include "pxr/base/arch/systemInfo.h"
#include "pxr/base/plug/registry.h"

/*
 We'd like to have arguments with defaults:
 --exportMeshes 1
 --exportCameras 1
 --exportMaterials 1
 --arKitCompatible 1
 --exportDoubleSided 1
 --singleFile 0
 --exportNormals 0
 --exportCurves 0
 --exportLines 0
 --exportEdges 0
 */

void
_findUSDPlugins() {
    // we're assuming that this executable has been installed in the
    // USDExporter.plugin/Contents/MacOS/ that is itself installed inside
    // of a SketchUp app bundle. Note that this is almost certainly a
    // different place when this code gets ported to Windows, so will have
    // to deal with this then.
    std::string dir = pxr::TfGetPathName(pxr::ArchGetExecutablePath());
    std::string pluginDir = pxr::TfStringCatPaths(dir,
                                                  "../Contents/Resources/usd/");
    std::cerr << "using USD plugins directory: " << pluginDir << std::endl;
    pxr::PlugRegistry::GetInstance().RegisterPlugins(pluginDir);
}


int
main(int argc, const char * argv[]) {
    _findUSDPlugins();

    if (argc < 3) {
        std::cerr   << "USAGE: " << argv[0]
        << " [opts] <in.skp> <out.usd[a,z]>" << std::endl;
        return -1;
    }
    const std::string skpFile = std::string(argv[1]);
    const std::string usdFile = std::string(argv[2]);

    // these are the same defaults as the plug-in
    // we might want to have these returned in a JSON dict or something
    // so if they changed, we could just call a function to get them.
    // But for now, hard code them here...
    bool exportMeshes = true;
    bool exportCameras = true;
    bool exportMaterials = true;
    bool exportDoubleSided = true;
    bool exportToSingleFile = false;
    bool exportARKitCompatibleUSDZ = true;
    bool exportNormals = false;
    bool exportCurves = false;
    bool exportLines = false;
    bool exportEdges = false;

    // we should handle command line args here:
    
    try {
        auto myExporter = USDExporter();
        myExporter.SetExportMeshes(exportMeshes);
        myExporter.SetExportCameras(exportCameras);
        myExporter.SetExportMaterials(exportMaterials);
        myExporter.SetExportDoubleSided(exportDoubleSided);
        myExporter.SetExportToSingleFile(exportToSingleFile);
        myExporter.SetExportARKitCompatibleUSDZ(exportARKitCompatibleUSDZ);
        myExporter.SetExportNormals(exportNormals);
        myExporter.SetExportCurves(exportCurves);
        myExporter.SetExportLines(exportLines);
        myExporter.SetExportEdges(exportEdges);
        if (!myExporter.Convert(skpFile, usdFile, NULL)) {
            std::cerr << "Failed to save USD file " << usdFile << std::endl;
            return -2;
        }
    } catch (...) {
        std::cerr << "Failed to save USD file " << usdFile
        << " (Exception was thrown)" << std::endl;
        return -3;
    }
    std::cerr << "Wrote USD file " << usdFile << std::endl;
    return 0;
}

