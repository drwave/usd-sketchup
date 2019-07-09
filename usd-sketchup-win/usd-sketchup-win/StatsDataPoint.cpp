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

#include "pch.h"
#include "StatsDataPoint.h"

StatsDataPoint::StatsDataPoint() :  _meshesCount(0), _linesCount(0),
                                    _edgesCount(0), _curvesCount(0),
                                    _originalFacesCount(0),
                                    _geomSubsetsCount(0), _materialsCount(0),
                                    _shadersCount(0), _trianglesCount(0) {
    
}

StatsDataPoint::~StatsDataPoint() { };

unsigned long long
StatsDataPoint::GetMeshesCount() {
    return _meshesCount;
}

unsigned long long
StatsDataPoint::GetLinesCount() {
    return _linesCount;
}

unsigned long long
StatsDataPoint::GetEdgesCount() {
    return _edgesCount;
}

unsigned long long
StatsDataPoint::GetCurvesCount() {
    return _curvesCount;
}

unsigned long long
StatsDataPoint::GetShadersCount() {
    return _shadersCount;
}

unsigned long long
StatsDataPoint::GetTrianglesCount() {
    return _trianglesCount;
}

unsigned long long
StatsDataPoint::GetMaterialsCount() {
    return _materialsCount;
}

unsigned long long
StatsDataPoint::GetGeomSubsetsCount() {
    return _geomSubsetsCount;
}

unsigned long long
StatsDataPoint::GetOriginalFacesCount() {
    return _originalFacesCount;
}

void
StatsDataPoint::SetMeshesCount(unsigned long long value) {
    _meshesCount = value;
}

void
StatsDataPoint::SetLinesCount(unsigned long long value) {
    _linesCount = value;
}

void
StatsDataPoint::SetEdgesCount(unsigned long long value) {
    _edgesCount = value;
}

void
StatsDataPoint::SetCurvesCount(unsigned long long value) {
    _curvesCount = value;
}

void
StatsDataPoint::SetShadersCount(unsigned long long value) {
    _shadersCount = value;
}

void
StatsDataPoint::SetTrianglesCount(unsigned long long value) {
    _trianglesCount = value;
}

void
StatsDataPoint::SetMaterialsCount(unsigned long long value) {
    _materialsCount = value;
}

void
StatsDataPoint::SetGeomSubsetsCount(unsigned long long value) {
    _geomSubsetsCount = value;
}

void
StatsDataPoint::SetOriginalFacesCount(unsigned long long value) {
    _originalFacesCount = value;
}

