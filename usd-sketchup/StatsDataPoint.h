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

#ifndef StatsDataPoint_h
#define StatsDataPoint_h

#include <stdio.h>
#include "pxr/usd/usdGeom/mesh.h"
#include "pxr/usd/usdGeom/xform.h"
#include "pxr/usd/usdShade/shader.h"
#include "pxr/usd/usdShade/material.h"

// A Stats data point is just a convenient way of keeping track of various
// info as we're exporting the scene. For example, when we're exporting a
// component definition, we'll want to track how many faces, triangles,
// materials and shaders were defined in this component.
// Then when we actually write out the instances of these components we'll
// use those to track the total number of these that will be expected to be
// live in the scene graph when the file is read in.

class StatsDataPoint {

public:
    StatsDataPoint();
    ~StatsDataPoint();

    unsigned long long GetMeshesCount();
    unsigned long long GetLinesCount();
    unsigned long long GetEdgesCount();
    unsigned long long GetCurvesCount();
    unsigned long long GetShadersCount();
    unsigned long long GetTrianglesCount();
    unsigned long long GetMaterialsCount();
    unsigned long long GetGeomSubsetsCount();
    unsigned long long GetOriginalFacesCount();

    void SetMeshesCount(unsigned long long value);
    void SetLinesCount(unsigned long long value);
    void SetEdgesCount(unsigned long long value);
    void SetCurvesCount(unsigned long long value);
    void SetShadersCount(unsigned long long value);
    void SetTrianglesCount(unsigned long long value);
    void SetMaterialsCount(unsigned long long value);
    void SetGeomSubsetsCount(unsigned long long value);
    void SetOriginalFacesCount(unsigned long long value);

private:
    unsigned long long _meshesCount;
    unsigned long long _linesCount;
    unsigned long long _edgesCount;
    unsigned long long _curvesCount;
    unsigned long long _shadersCount;
    unsigned long long _trianglesCount;
    unsigned long long _materialsCount;
    unsigned long long _geomSubsetsCount;
    unsigned long long _originalFacesCount;
};

#endif /* StatsDataPoint_h */
