#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <filesystem>

#include "../../dependencies/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <../../dependencies/stb_image/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

#include "../shaders/shader.h"

unsigned int textureFromFile(const char *path, const std::string &directory);

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

class Model 
{
    public:
    Transform transform;
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;

    Model(char const* path);
    void draw(Shader &shader);

    private:
    void loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif