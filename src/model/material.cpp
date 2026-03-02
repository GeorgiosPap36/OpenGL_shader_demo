#include "material.h"

Material::Material(Shader* shader) : shader(shader) {
    
}

void Material::bindUniforms() {
    shader->use();

    for (const auto& [name, value] : uniforms) {
        std::visit([&](auto&& v) {
            shader->set(name.c_str(), v);
        }, value);
    }

    // Bind textures of all the meshes in the render batch
    // Dont start from 0, start from another (changes per scene, pass it as parameter)
    // for (unsigned int i = 0; i < textures.size(); ++i) {
    //     GLuint unit = MATERIAL_TEXTURE_BASE + i;
    //     glActiveTexture(GL_TEXTURE0 + unit);
    //     glBindTexture(GL_TEXTURE_2D, textures[i].id);
    // }
}

void Material::bindInt(const std::string& name, int i) {
    uniforms[name] = i;
}

void Material::bindFloat(const std::string& name, float f) {
    uniforms[name] = f;
}

void Material::bindVec3(const std::string& name, glm::vec3& vec3) {
    uniforms[name] = vec3;
}

void Material::bindMat4(const std::string& name,  glm::mat4& mat4) {
    uniforms[name] = mat4;
}