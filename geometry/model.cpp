#include "model.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../logger.h"

Model::Model()
{

}

bool Model::Load(const std::string &path)
{
    Clear();

    bool Ret = false;

    const aiScene* pScene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
    LOG(verbose) << "Model loaded " << pScene->HasAnimations() << pScene->HasCameras() <<
                    pScene->HasLights() << pScene->HasMaterials() << pScene->HasMeshes() <<
                    pScene->HasTextures();

    if (pScene) {
        Ret;
    }
    else {
       LOG(fatal) << "Error parsing " << path.c_str() << " ";
    }

    delete pScene;
    return true;
}

void Model::Clear()
{

}

