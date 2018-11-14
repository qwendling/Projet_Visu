#include "AssetLoader.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <stdexcept>


using namespace std;
bool AssetLoader::import(const std::string filename){
    std::ifstream fin(filename);
    if(!fin.fail()){
        fin.close();
    }else{
        this->_importer->GetErrorString();
        //std::cout << err_Importer << std::endl;
        std::cout << "erreur lecture du fichier "<< strerror(errno) << std::endl;
        return false;
    }

    try{
        _scene = const_cast<aiScene*>(_importer->ReadFile(filename,aiProcessPreset_TargetRealtime_Quality));
    }catch(std::runtime_error& err){
        std::cout << err.what();
        return false;
    }

    if(!_scene){
        std::cout << _importer->GetErrorString() << std::endl;
        return false;
    }
    return true;
}

bool AssetLoader::loadData(vector<MeshTri>& pMesh){
    if(!_scene->HasMeshes()){
        return false;
    }

    /*pVertices.resize(_scene->mNumMeshes);
    pNormales.resize(_scene->mNumMeshes);
    pIndices.resize(_scene->mNumMeshes);*/
    pMesh.resize(_scene->mNumMeshes);

    aiMaterial** mat = _scene->mMaterials;

    for(unsigned int m=0;m < _scene->mNumMeshes;++m){
        /*vector<glm::vec3>& vertices = pVertices[m];
        vector<glm::vec3>& normals = pNormales[m];
        vector<unsigned int>& indices = pIndices[m];*/
        MeshTri& Tmesh = pMesh[m];

        const aiMesh* mesh = _scene->mMeshes[m];
        if(mesh->HasPositions()){
            //vertices.resize(mesh->mNumVertices);
            for(unsigned int v=0;v<mesh->mNumVertices;++v){
                const aiVector3D& vertex = mesh->mVertices[v];
                Tmesh.add_vertex(glm::vec3(vertex.x,vertex.y,vertex.z));
            }
            //indices.resize(mesh->mNumFaces * 3);
            for(unsigned int f=0;f< mesh->mNumFaces;++f){
                const struct aiFace& face = mesh->mFaces[f];
                assert(face.mNumIndices == 3);
                Tmesh.add_tri(face.mIndices[0],face.mIndices[1],face.mIndices[2]);
            }
            if(mesh->HasNormals()){
                //normals.resize(mesh->mNumVertices);
                for(unsigned int n=0;n<mesh->mNumVertices;++n){
                    const aiVector3D& normal = mesh->mNormals[n];
                    Tmesh.add_normal(glm::vec3(normal.x,normal.y,normal.z));
                    //normals[n] = glm::vec3(normal.x,normal.y,normal.z);
                }
            }
        }

        Tmesh.set_DiffuseColor(mat[m]);
        Tmesh.set_list_triangle();
    }



    return true;
}
