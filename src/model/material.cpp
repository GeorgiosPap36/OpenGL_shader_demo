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
}

void Material::bindFloat(const std::string& name, float f) {
    uniforms[name] = f;
}

void Material::bindMat4(const std::string& name,  glm::mat4& mat4) {
    uniforms[name] = mat4;
}