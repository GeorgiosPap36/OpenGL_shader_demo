#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <variant>

#include "../shaders/shader.h"

class Material {
    public:
    Shader* shader;
    using UniformValue = std::variant<
        float,
        glm::mat4
    >;
    std::map<std::string, UniformValue> uniforms;

    Material(Shader* shader);

    void bindUniforms();

    void bindFloat(const std::string& name, float f);
    void bindMat4(const std::string& name,  glm::mat4& mat4);
    
};

#endif