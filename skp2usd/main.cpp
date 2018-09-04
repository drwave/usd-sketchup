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
#include "USDExporter.h"
#include "pxr/base/arch/systemInfo.h"
#include "pxr/base/plug/registry.h"

int
main(int argc, const char * argv[]) {
    if (argc < 3) {
        std::cerr   << "USAGE: " << argv[0]
        << " <in.skp> <out.usd[a,z]>" << std::endl;
        return -1;
    }
    const std::string skpFile = std::string(argv[1]);
    const std::string usdFile = std::string(argv[2]);

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

    try {
        auto myExporter = USDExporter();
        myExporter.SetExportToSingleFile(false);
        myExporter.SetExportARKitCompatibleUSDZ(true);
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

