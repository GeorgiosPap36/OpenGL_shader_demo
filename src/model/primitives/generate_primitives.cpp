#include "generate_primitives.h"

Model GeneratePrimitives::generateCube(glm::vec3 size, std::vector<Texture> textures) {
    std::vector<Vertex> vertices; 
    std::vector<unsigned int> indices;

    // set vertices
    // set indices


    Mesh mesh(vertices, indices, textures);

    Model model;
    model.meshes.push_back(mesh);
    model.directory = "";
    model.texturesLoaded = mesh.textures;


    return model;
}