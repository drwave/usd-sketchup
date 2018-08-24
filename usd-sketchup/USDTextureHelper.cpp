#include <set>
#include <string>
#include <vector>

#include "USDTextureHelper.h"

#include <iostream>
#include <string>
#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST
#if defined(_WIN32)
#include <direct.h>   // _mkdir
#endif

static bool
_isDirExist(const std::string& path)
{
#if defined(_WIN32)
    struct _stat info;
    if (_stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & _S_IFDIR) != 0;
#else
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
#endif
}

static bool
_makePath(const std::string& path)
{
#if defined(_WIN32)
    int ret = _mkdir(path.c_str());
#else
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
#endif
    if (ret == 0)
        return true;
    
    switch (errno)
    {
        case ENOENT:
            // parent didn't exist, try to create it
        {
            int pos = (int)path.find_last_of('/');
            if (pos == std::string::npos)
#if defined(_WIN32)
                pos = path.find_last_of('\\');
            if (pos == std::string::npos)
#endif
                return false;
            if (!_makePath( path.substr(0, pos) ))
                return false;
        }
            // now, try to create again
#if defined(_WIN32)
            return 0 == _mkdir(path.c_str());
#else
            return 0 == mkdir(path.c_str(), mode);
#endif
            
        case EEXIST:
            // done!
            return _isDirExist(path);
            
        default:
            return false;
    }
}

USDTextureHelper::USDTextureHelper() {
}

USDTextureHelper::~USDTextureHelper() {
}

bool
USDTextureHelper::MakeTextureDirectory(const std::string& directory) {
    return _makePath(directory);
}


size_t
USDTextureHelper::LoadAllTextures(SUModelRef model, SUTextureWriterRef texture_writer, bool textures_from_layers) {
    if (SUIsInvalid(texture_writer)) {
        return 0;
    }
    
    if (textures_from_layers) {
        // Layers only
        size_t num_layers;
        SUResult result = SUModelGetNumLayers(model, &num_layers);
        if ((result == SU_ERROR_NONE) && (num_layers > 0)) {
            std::vector<SULayerRef> layers(num_layers);
            result = SUModelGetLayers(model, num_layers, &layers[0], &num_layers);
            if (result == SU_ERROR_NONE) {
                for (size_t i = 0; i < num_layers; i++) {
                    SULayerRef layer = layers[i];
                    long texture_id = 0;
                    result = SUTextureWriterLoadEntity(texture_writer,
                                                       SULayerToEntity(layer),
                                                       &texture_id);
                }
            }
        }
    } else {
        // Get the root component
        SUEntitiesRef model_entities;
        if (SU_ERROR_NONE == SUModelGetEntities(model, &model_entities)) {
            LoadEntities(texture_writer, model_entities);
            
            // Next, all the component definitions
            size_t num_definitions = 0;
            SUModelGetNumComponentDefinitions(model, &num_definitions);
            if (num_definitions > 0) {
                std::vector<SUComponentDefinitionRef> component_definitions(num_definitions);
                SUModelGetComponentDefinitions(model, num_definitions,
                                               &component_definitions[0],
                                               &num_definitions);
                
                for (size_t i = 0; i < num_definitions; i++) {
                    SUComponentDefinitionRef component_definition =
                    component_definitions[i];
                    LoadComponent(texture_writer, component_definition);
                }
            }
        }
    }
    // Return the number of textures
    size_t count = 0;
    if (SU_ERROR_NONE != SUTextureWriterGetNumTextures(texture_writer, &count)) {
        count = 0;
    }
    return count;
}

void
USDTextureHelper::LoadComponent(SUTextureWriterRef texture_writer, SUComponentDefinitionRef component) {
    SUEntitiesRef entities = SU_INVALID;
    SUComponentDefinitionGetEntities(component, &entities);
    LoadEntities(texture_writer, entities);
}

void
USDTextureHelper::LoadEntities(SUTextureWriterRef texture_writer, SUEntitiesRef entities) {
    // Top level faces, instances, groups, and images
    LoadFaces(texture_writer, entities);
    LoadComponentInstances(texture_writer, entities);
    LoadGroups(texture_writer, entities);
    LoadImages(texture_writer, entities);
}

void
USDTextureHelper::LoadFaces(SUTextureWriterRef texture_writer, SUEntitiesRef entities) {
    if (SUIsInvalid(entities)) {
        return ;
    }
    size_t num_faces = 0;
    SUEntitiesGetNumFaces(entities, &num_faces);
    if (!num_faces) {
        return ;
    }
    SUResult hr;
    std::vector<SUFaceRef> faces(num_faces);
    SUEntitiesGetFaces(entities, num_faces, &faces[0], &num_faces);
    for (size_t i = 0; i < num_faces; i++) {
        SUFaceRef face = faces[i];
        long front_texture_id = 0;
        long back_texture_id = 0;
        hr = SUTextureWriterLoadFace(texture_writer, face,
                                     &front_texture_id,
                                     &back_texture_id);
    }
}

void
USDTextureHelper::LoadComponentInstances(SUTextureWriterRef texture_writer, SUEntitiesRef entities) {
    if (SUIsInvalid(entities)) {
        return ;
    }
    size_t num_instances = 0;
    SUEntitiesGetNumInstances(entities, &num_instances);
    if (!num_instances) {
        return ;
    }
    SUResult hr;
    std::vector<SUComponentInstanceRef> instances(num_instances);
    SUEntitiesGetInstances(entities, num_instances,
                           &instances[0], &num_instances);
    
    for (size_t i = 0; i < num_instances; i++) {
        SUComponentInstanceRef instance = instances[i];
        if (!SUIsInvalid(instance)) {
            long texture_id = 0;
            hr = SUTextureWriterLoadEntity(texture_writer,
                                           SUComponentInstanceToEntity(instance),
                                           &texture_id);
        }
    }
}

void
USDTextureHelper::LoadGroups(SUTextureWriterRef texture_writer, SUEntitiesRef entities) {
    if (SUIsInvalid(entities)) {
        return ;
    }
    size_t num_groups;
    SUEntitiesGetNumGroups(entities, &num_groups);
    if (!num_groups) {
        return ;
    }
    std::vector<SUGroupRef> groups(num_groups);
    SUEntitiesGetGroups(entities, num_groups, &groups[0], &num_groups);

    for (size_t i = 0; i < num_groups; i++) {
        SUGroupRef group = groups[i];
        if (!SUIsInvalid(group)) {
            // Get the component part of the group
            SUEntitiesRef group_entities = SU_INVALID;
            SUGroupGetEntities(group, &group_entities);
            LoadEntities(texture_writer, group_entities);
        }
    }
}

void
USDTextureHelper::LoadImages(SUTextureWriterRef texture_writer, SUEntitiesRef entities) {
  if (SUIsInvalid(entities)) {
      return ;
  }
    size_t num_images = 0;
    SUEntitiesGetNumImages(entities, &num_images);
    if (!num_images) {
        return ;
    }
    std::vector<SUImageRef> images(num_images);
    if (SU_ERROR_NONE == SUEntitiesGetImages(entities, num_images, &images[0], &num_images)) {
        for (size_t i = 0; i < num_images; i++) {
            SUImageRef image = images[i];
            if (!SUIsInvalid(image)) {
                long texture_id = 0;
                SUTextureWriterLoadEntity(texture_writer,
                                          SUImageToEntity(image),
                                          &texture_id);
            }
        }
    }
}

std::string
USDTextureHelper::_imageFileName(SUImageRef imageRef) {
    SUStringRef fileName;
    SUSetInvalid(fileName);
    SUStringCreate(&fileName);
    SUImageGetFileName(imageRef, &fileName);
    size_t length;
    SUStringGetUTF8Length(fileName, &length);
    std::string string;
    string.resize(length);
    size_t returned_length;
    SUStringGetUTF8(fileName, length, &string[0], &returned_length);
    return string;
}

std::vector<std::string>
USDTextureHelper::textureFileNames(SUTextureWriterRef texture_writer, SUEntitiesRef entities) {
    std::vector<std::string> names;
    if (SUIsInvalid(entities)) {
        return names;
    }
    size_t num_images = 0;
    SUEntitiesGetNumImages(entities, &num_images);
    if (!num_images) {
        return names;
    }

    // can we use SUTextureWriterGetTextureFilePath(SUTextureWriterRef writer, long texture_id, SUStringRef* file_path)
    
    std::vector<SUImageRef> images(num_images);
    if (SU_ERROR_NONE == SUEntitiesGetImages(entities, num_images, &images[0], &num_images)) {
        for (size_t i = 0; i < num_images; i++) {
            SUImageRef image = images[i];
            if (!SUIsInvalid(image)) {
                names.push_back(_imageFileName(image));
            }
        }
    }
    return names;
}

