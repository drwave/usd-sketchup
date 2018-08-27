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
#ifndef SKPTOUSD_COMMON_USDTEXTUREHELPER_H
#define SKPTOUSD_COMMON_USDTEXTUREHELPER_H

#include <SketchUpAPI/sketchup.h>

bool
makeTextureDirectory(const std::string& directory);

class USDTextureHelper {
 public:
    USDTextureHelper();
    ~USDTextureHelper();

  // Load all textures, return number of textures
  // Input: model to load textures from
  // Input: texture writer
  // Input: layers or entities
  size_t LoadAllTextures(SUModelRef model, SUTextureWriterRef texture_writer,
                         bool textures_from_layers);

    bool MakeTextureDirectory(const std::string& directory);
    std::vector<std::string> textureFileNames(SUTextureWriterRef texture_writer, SUEntitiesRef entities);


private:
  // Load textures from all of the entities that have textures
  void LoadComponent(SUTextureWriterRef texture_writer,
                     SUComponentDefinitionRef component);
  void LoadEntities(SUTextureWriterRef texture_writer,
                    SUEntitiesRef entities);
  void LoadComponentInstances(SUTextureWriterRef texture_writer,
                              SUEntitiesRef entities);
  void LoadGroups(SUTextureWriterRef texture_writer,
                  SUEntitiesRef entities);
  void LoadFaces(SUTextureWriterRef texture_writer,
                 SUEntitiesRef entities);
  void LoadImages(SUTextureWriterRef texture_writer,
                  SUEntitiesRef entities);
    
    std::string _imageFileName(SUImageRef imageRef);
};


#endif // SKPTOUSD_COMMON_USDTEXTUREHELPER_H
