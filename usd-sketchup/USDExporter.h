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
    
    void SetSkpFileName(const std::string name);
    void SetUSDFileName(const std::string name);
    void SetExportNormals(bool flag);
    void SetExportEdges(bool flag);
    void SetExportLines(bool flag);
    void SetExportCurves(bool flag);
    void SetExportToSingleFile(bool flag);
    void SetExportARKitCompatibleUSDZ(bool flag);

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

    bool _exportNormals;
    bool _exportEdges;
    bool _exportLines;
    bool _exportCurves;
    bool _exportToSingleFile;
    bool _exportARKitCompatibleUSDZ;
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
    std::vector<SUComponentInstanceRef> instances;
   
    pxr::VtArray<pxr::GfVec3f> _points;
    pxr::VtArray<pxr::GfVec3f> _vertexNormals;
    pxr::VtArray<pxr::GfVec3f> _vertexFlippedNormals;
    pxr::VtArray<int> _faceVertexCounts;
    pxr::VtArray<int> _flattenedFaceVertexIndices;

    bool _hasFrontFaceMaterial;
    std::string _frontFaceTextureName;
    pxr::VtArray<pxr::GfVec2f> _frontUVs;
    pxr::GfVec4d _frontRGBA;
    pxr::VtArray<pxr::GfVec3f> _frontFaceRGBs;
    pxr::VtArray<float> _frontFaceAs;

    bool _hasBackFaceMaterial;
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

    std::string _baseFileName;
    std::string _zipFileName;

    bool _exportingUSDZ;
    std::set<std::string> _filePathsForZip;

    void _updateFileNames();

    void _writeMenvFile();

    void _ExportTextures(const pxr::SdfPath parentPath);
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

    void _ExportMaterial(const pxr::SdfPath parentPath,
                         std::string texturePath);
    bool _ExportMaterials(const pxr::SdfPath parentPath);
    void _ExportFaces(const pxr::SdfPath parentPath, SUEntitiesRef entities);
    bool _gatherFaceInfo(const pxr::SdfPath parentPath, SUFaceRef face);
    void _clearFacesExport();
    void _addFaceAsTexturedTriangles(SUFaceRef face);
    std::string _textureFileName(SUTextureRef textureRef);
    bool _addFrontFaceMaterial(SUFaceRef face);
    bool _addBackFaceMaterial(SUFaceRef face);
    void _exportMesh(pxr::SdfPath path, pxr::SdfPath materialPath,
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
