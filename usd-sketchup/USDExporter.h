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
//  USDExporter.hpp
//  skp2usd
//
//  Created by Michael B. Johnson on 11/22/17, based on previous work from 2015.
//
// This version of the SketchUp to USD exporter writes out either a single file
// or a set of three USD files:
//
// - <foo>.components.usd:
//   - one that contains the component definitions
// - <foo>.geom.usd:
//   - one defining all the entities in the scene, where each instance
//     references its master in first file
// - <foo>.usd
//   - Sublayers in the <foo>.geom.usd file and defines all the
//     SketchUp "scenes" as USD cameras

#ifndef USDExporter_h
#define USDExporter_h

#include <SketchUpAPI/sketchup.h>
// for some reason, this header is not included in SketchUp's global one
#include <SketchUpAPI/import_export/pluginprogresscallback.h>

#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usd/timeCode.h"
#include "pxr/usd/usdGeom/camera.h"
#include "pxr/usd/usdGeom/mesh.h"
#include "pxr/usd/usdGeom/xform.h"
#include "pxr/usd/usdShade/shader.h"
#include "pxr/usd/usdShade/material.h"

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


class USDExporter {

public:    
    USDExporter();
    ~USDExporter();
    
    bool Convert(const std::string& skpFileName,
                 const std::string& usdFileName,
                 SketchUpPluginProgressCallback* callback);

    const std::string GetSkpFileName() const;
    const std::string GetUSDFileName() const;
    bool GetExportNormals() const;
    bool GetExportEdges() const;
    bool GetExportLines() const;
    bool GetExportCurves() const;
    bool GetExportToSingleFile() const;
    bool GetExportARKitCompatibleUSDZ() const;
    bool GetExportMaterials() const;
    bool GetExportMeshes() const;
    bool GetExportCameras() const;

    void SetSkpFileName(const std::string name);
    void SetUSDFileName(const std::string name);
    void SetExportNormals(bool flag);
    void SetExportEdges(bool flag);
    void SetExportLines(bool flag);
    void SetExportCurves(bool flag);
    void SetExportToSingleFile(bool flag);
    void SetExportARKitCompatibleUSDZ(bool flag);
    void SetExportMaterials(bool flag);
    void SetExportMeshes(bool flag);
    void SetExportCameras(bool flag);

    double GetAspectRatio() const;
    double GetSensorHeight() const;
    double GetStartFrame() const;
    double GetFrameIncrement() const;

    void SetAspectRatio(double aspectRatio);
    void SetSensorHeight(double sensorHeight);
    void SetStartFrame(double frame);
    void SetFrameIncrement(double frame);

    unsigned long long GetComponentDefinitionCount();
    unsigned long long GetComponentInstanceCount();
    unsigned long long GetMeshCount();
    unsigned long long GetCurvesCount();
    unsigned long long GetLinesCount();
    unsigned long long GetEdgesCount();
    unsigned long long GetCamerasCount();
    unsigned long long GetMaterialsCount();
    unsigned long long GetGeomSubsetsCount();
    std::string GetExportTimeSummary();

private:
    bool _performExport(const std::string& skpFileName,
                        const std::string& usdFileName);

    SUModelRef _model;
    SUTextureWriterRef _textureWriter;

    pxr::UsdStageRefPtr _stage;
    
    SketchUpPluginProgressCallback* _progressCallback;
    unsigned long long _componentDefinitionCount;
    unsigned long long _componentInstanceCount;
    unsigned long long _meshCount;
    unsigned long long _edgesCount;
    unsigned long long _linesCount;
    unsigned long long _curvesCount;
    unsigned long long _camerasCount;
    unsigned long long _materialsCount;
    unsigned long long _geomSubsetsCount;
    std::string _exportTimeSummary;

    bool _exportNormals;
    bool _exportEdges;
    bool _exportLines;
    bool _exportCurves;
    bool _exportToSingleFile;
    bool _exportARKitCompatibleUSDZ;
    bool _exportMaterials;
    bool _exportMeshes;
    bool _exportCameras;
    double _aspectRatio;
    double _sensorHeight;
    double _startFrame;
    double _frameIncrement;

    pxr::TfToken _rotateAvarToken;
    pxr::VtArray<std::string> _rotateAvarNames;
    pxr::TfToken _scaleAvarToken;
    pxr::VtArray<std::string> _scaleAvarNames;
    pxr::TfToken _transformToken;
    pxr::VtValue _transformValue;
    pxr::TfToken _translateAvarToken;
    pxr::VtArray<std::string> _translateAvarNames;
    pxr::TfToken _visToken;
    pxr::VtValue _visValue;

    std::set<std::string> _instancedComponentNames;
    std::map<uintptr_t, std::string> _componentPtrSafeNameMap;
    std::map<std::string, std::string> _textureNameSafeNameMap;
    std::map<std::string, std::string> _originalComponentNameSafeNameDictionary;
    std::map<std::string, int> _instanceCountPerClass;
    std::vector<SUComponentInstanceRef> instances; // wave: this should start with _, right?
   
    pxr::VtArray<pxr::GfVec3f> _points;
    pxr::VtArray<pxr::GfVec3f> _vertexNormals;
    pxr::VtArray<pxr::GfVec3f> _vertexFlippedNormals;
    pxr::VtArray<int> _faceVertexCounts;
    pxr::VtArray<int> _flattenedFaceVertexIndices;
    // SketchUp allows multiple materials per mesh, so in order to accomodate
    // that, we need to use USD's UsdGeomSubset API. We use these variables
    // to hold the indices front and back material names and indices for
    // each mesh as its being constructed. Note that the points, uvs, display
    // color and opacity are all pan-mesh, so we don't need to track them.
    // This is only for material assignment
    std::vector<MeshSubset> _meshFrontFaceSubsets;
    std::vector<MeshSubset> _meshBackFaceSubsets;
    // Many SketchUp models seem to reuse the same texture on different faces,
    // so we cache the material that we make from each texture so we only define
    // it once per mesh.
    std::map<std::string, pxr::SdfPath> _texturePathMaterialPath;
    
    std::string _frontFaceTextureName;
    pxr::VtArray<pxr::GfVec2f> _frontUVs;
    pxr::GfVec4d _frontRGBA;
    pxr::VtArray<pxr::GfVec3f> _frontFaceRGBs;
    pxr::VtArray<float> _frontFaceAs;

    std::string _backFaceTextureName;
    pxr::VtArray<pxr::GfVec2f> _backUVs;
    pxr::GfVec4d _backRGBA;
    pxr::VtArray<pxr::GfVec3f> _backFaceRGBs;
    pxr::VtArray<float> _backFaceAs;

    pxr::VtArray<pxr::GfVec3f> _edgePoints;
    pxr::VtArray<int> _edgeVertexCounts;
    
    pxr::VtArray<pxr::GfVec3f> _curvePoints;
    pxr::VtArray<int> _curveVertexCounts;
    
    pxr::VtArray<pxr::GfVec3f> _polylinePoints;
    pxr::VtArray<int> _polylineVertexCounts;
    
    std::set<std::string> _usedCameraNames;
    int _currentVertexIndex;
    SUComponentDefinitionRef _currentComponentDefinition;
    bool _isBillboard;
    
    std::string _skpFileName;
    std::string _usdFileName;
    std::string _textureDirectory;
    bool _useSharedFallbackMaterial;
    pxr::SdfPath _fallbackDisplayMaterialPath;

    std::string _baseFileName;
    std::string _zipFileName;

    bool _exportingUSDZ;
    std::set<std::string> _filePathsForZip;

    void _updateFileNames();

    void _writeMenvFile();

    void _ExportTextures(const pxr::SdfPath parentPath);
    void _ExportFallbackDisplayMaterial(const pxr::SdfPath parentPath);
    void _ExportGeom(const pxr::SdfPath parentPath);
    void _ExportEntities(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    void _prepAvars();
    void _addAvars(pxr::UsdPrim prim);

    std::string _componentDefinitionsFileName;
    std::set<std::string> _usedComponentNames;
    void _ExportComponentDefinitions(const pxr::SdfPath parentPath);
    void _ExportComponentDefinition(const pxr::SdfPath parentPath,
                                    SUComponentDefinitionRef component);
    int _countComponentDefinitionsActuallyUsed();
    int _countEntities(SUEntitiesRef entities);

    std::string _geomFileName;

    void _ExportInstances(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    bool _ExportInstance(const pxr::SdfPath parentPath,
                         SUComponentInstanceRef instance);

    void _ExportGroups(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    std::string _ExportGroup(const pxr::SdfPath parentPath, SUGroupRef group,
                             std::set<std::string>& usedGroupNames);

    pxr::SdfPath _defaultMaterialPath;
    void _exportRGBAShader(const pxr::SdfPath path,
                           pxr::UsdShadeOutput materialSurface,
                           pxr::GfVec3f rgb, float opacity);
    // returns diffuseColor & opacity inputs
    std::pair<pxr::UsdShadeInput, pxr::UsdShadeInput>
    _exportPreviewShader(const pxr::SdfPath path,
                         pxr::UsdShadeOutput materialSurface);
    pxr::UsdShadeOutput _exportSTPrimvarShader(const pxr::SdfPath path);
    pxr::UsdShadeOutput _exportDisplayColorPrimvarShader(const pxr::SdfPath path);
    pxr::UsdShadeOutput _exportDisplayOpacityPrimvarShader(const pxr::SdfPath path);
    void _exportTextureShader(const pxr::SdfPath path,
                              std::string texturePath,
                              pxr::UsdShadeOutput result,
                              pxr::UsdShadeInput diffuseColor);
    void _ExportTextureMaterial(const pxr::SdfPath parentPath,
                                std::string texturePath);
    void _ExportRGBAMaterial(const pxr::SdfPath path,
                             pxr::GfVec3f rgb, float opacity);
    void _ExportDisplayMaterial(const pxr::SdfPath parentPath);
    bool _someMaterialsToExport();
    bool _ExportMaterials(const pxr::SdfPath parentPath);
    void _ExportFaces(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    size_t _gatherFaceInfo(const pxr::SdfPath parentPath, SUFaceRef face);
    void _clearFacesExport();
    size_t _addFaceAsTexturedTriangles(const pxr::SdfPath parentPath, SUFaceRef face);
    std::string _textureFileName(SUTextureRef textureRef);
    bool _addFrontFaceMaterial(SUFaceRef face);
    bool _addBackFaceMaterial(SUFaceRef face);
    int _cacheDisplayMaterial(pxr::SdfPath path, MeshSubset& subset, int index);
    void _cacheRGBAMaterial(pxr::SdfPath path, MeshSubset& subset);
    int _cacheTextureMaterial(pxr::SdfPath path, MeshSubset& subset, int index);
    bool _bothDisplayColorAreEqual();
    bool _bothDisplayOpacityAreEqual();

    void _exportMesh(pxr::SdfPath path,
                     std::vector<MeshSubset> _meshSubsets,
                     pxr::TfToken const orientation,
                     pxr::VtArray<pxr::GfVec3f>& rgb, pxr::VtArray<float>& a,
                     pxr::VtArray<pxr::GfVec2f>& uv,
                     pxr::VtArray<pxr::GfVec3f>& extent,
                     bool flipNormals,
                     bool doubleSided);
    void _ExportMeshes(const pxr::SdfPath parentPath);
    void _ExportDoubleSidedMesh(const pxr::SdfPath parentPath);

    void _ExportEdges(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    void _gatherEdgeInfo(SUEdgeRef edge);

    void _ExportCurves(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    void _gatherCurveInfo(SUCurveRef curve);
    
    void _ExportPolylines(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    void _gatherPolylineInfo(SUPolyline3dRef polyline);
    
    void _ExportCameras(const pxr::SdfPath parentPath);
    void _ExportCamera(const pxr::SdfPath parentPath, SUSceneRef scene);
    
    void _ExportUSDShaderForBillboard(const pxr::SdfPath parentPath);
    void _ExportUSDShaderForTextureFile(const pxr::SdfPath parentPath);
};

#endif /* USDExporter_h */
