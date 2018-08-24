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
