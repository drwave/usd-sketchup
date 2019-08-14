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

#ifndef MeshSubset_h
#define MeshSubset_h

#include <pxr/usd/usdGeom/mesh.h>

class MeshSubset {
public:
    MeshSubset(std::string materialTextureName,
               pxr::GfVec3f rgb, float opacity,
               pxr::VtArray<int> faceIndices);
    ~MeshSubset();
    //MeshSubset(const MeshSubset&) = delete;
    
    const std::string GetMaterialTextureName();
    const pxr::GfVec3f GetRGB();
    const float GetOpacity();
    const pxr::VtArray<int> GetFaceIndices();
    pxr::SdfPath GetMaterialPath();
    void SetMaterialPath(pxr::SdfPath path);
    
private:
    std::string _materialTextureName;
    pxr::VtArray<int> _faceIndices;
    pxr::SdfPath _materialPath;
    pxr::GfVec3f _rgb;
    float _opacity;
};



#endif /* MeshSubset_h */
